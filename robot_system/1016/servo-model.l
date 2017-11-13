(defun make-servo nil
	(let (b1 b2)
		(setq b1 (make-cube 35 20 46))
		(send b1 :locate #f(9.5 0 0))
		(setq b2 (make-cylinder 3 60))
		(send b2 :locate #f(-4 0 0))
		(setq b1 (body+ b1 b2))
		(send b1 :set-color :gray20)
		b1
	)
)

(defun make-hinji nil
	(let (
			(b2 (make-cube 22 16 58))
			(b1 (make-cube 26 20 54))
		)
		(send b2 :locate #f(-4 0 0))
		(setq b2 (body- b2 b1))
		(send b1 :set-color :gray80)
		b2
	)
)

(defclass servo-model
	:super cascaded-link
	:slots (h1 s1 j1)
)
(defmethod servo-model
	(:init ()
		(let ()
			(send-super :init)
			(setq h1
				(instance bodyset-link
					:init
						(make-cascoords)
						:bodies (list (make-hinji))
				)
			)
			(setq s1
				(instance bodyset-link
					:init
						(make-cascoords)
						:bodies (list (make-servo))
				)
			)
			(setq j1
				(instance rotational-joint
					:init
						:parent-link h1
						:child-link s1
						:axis :z
				)
			)
			;; instance cascaded coords
			(setq links (list h1 s1))
			(setq joint-list (list j1))
			;;
			(send self :assoc h1)
			(send h1 :assoc s1)
			;;
			(send self :init-ending)
			self
		)
	)
	(:j1
		(&rest args)
		(forward-message-to j1 args)
	)
)

(setq r (instance servo-model :init))