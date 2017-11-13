(defun make-lin-arm nil
	(let (b1 b2 b3)
		(setq b1 (make-cube 20 20 40))
		(setq b2 (make-cube 4 4 42))
		(setq b3 (make-cube 4 4 42))
		(send b2 :locate #f(-8 0 0))
		(send b3 :locate #f(8 0 0))
		(setq b1 (body- b1 b2))
		(setq b1 (body- b1 b3))
		(send b1 :set-color :gray20)
		b1
	)
)

(defun make-lin nil
	(let (b1 b2)
		(setq b1 (make-lin-act))
		(setq b2 (make-cube 24 24 40))
		(send b1 :locate #f(0 0 2))
		(setq b2 (body- b2 b1))
		(send b2 :set-color :gray20)
		b2
	)

)

(defclass linear-arc
	:super cascaded-link
	:slots (l1 l2 j1)
)
(defmethod linear-arc
	(:init ()
		(let ()
			(send-super :init)
			(setq b (make-cube 20 20 40))
			(send b :locate #f(0 0 20))
			(send b :set-color :red)
			(setq l1
				(instance bodyset-link
					:init
						(make-cascoords)
						:bodies (list b)
				)
			)
			(setq b1 (make-cube 20 20 42))
			(setq b2 (make-cube 24 24 40))
			(setq b (body- b2 b1))
			(send b :set-color :blue)
			(setq l2
				(instance bodyset-link
					:init
						(make-cascoords)
						:bodies (list b)
				)
			)
			(setq j1
				(instance linear-joint
					:init
						:parent-link l1
						:child-link l2
						:axis :z
				)
			)
			;; instance cascaded coords
			(setq links (list l1 l2))
			(setq joint-list (list j1))
			;;
			(send self :assoc l1)
			(send l1 :assoc l2)
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

(setq r (instance linear-arc :init))
(objects (list r))
(setq i 0)
(do-until-key
	(send r
		:angle-vector (
			float-vector (*
				18
				(sin (/ i 100.0))
			)
		)
	)
	(send *irtviewer* :draw-objects)
	(incf i)
)









