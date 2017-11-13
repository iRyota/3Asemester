(load "wing.l")


(defclass fan
	:super cascaded-link
)
(defmethod fan
	(:init nil
		(let (b)
			(send-super :init)
			(load "cover.l")
			(setq front
				(instance bodyset-link
					:init
						(make-cascoords)
						:bodies (list (make-cover))
						:name 'front
				)
			)
			(load "cover.l")
			(setq back
				(instance bodyset-link
					:init
						(make-cascoords)
						:bodies (list (make-cover))
						:name 'back
				)
			)
			(send back :rotate pi :x)
			(send back :locate #f(0 0 0.4))
			(load "wing.l")
			(setq w (make-wing))
			(send w :set-color #f(0 0.7 1))
			(setq wing
				(instance bodyset-link
					:init
						(make-cascoords)
						:bodies (list w)
						:name 'wing
				)
			)

			(send back :assoc front)
			(send back :assoc wing)

			(setq link1
				(instance bodyset-link
					:init
						(make-cascoords)
						:bodies (list (make-cube 25 25 25))
						:name 'link1
				)
			)
			(send link1 :locate #f(0 0 -48.9))
			(send link1 :assoc back)

			(setq link2
				(instance bodyset-link
					:init
						(make-cascoords)
						:bodies (list (make-cylinder 10 200))
						:name 'link2
				)
			)
			(send link2 :rotate (/ pi 2) :y)
			(send link2 :locate #f(48.9 0 12.5))
			(send link2 :assoc link1)

			(setq cy0 (make-cylinder 16 200))
			(setq cy1 (make-cylinder 10 180))
			(send cy1 :locate #f(0 0 30))
			(setq cy0 (body- cy0 cy1))
			(send cy0 :rotate (* -1 (/ pi 2)) :y)
			(send cy0 :locate #f(-48.9 0 -325))
			(load "base.l")
			(setq base (make-base))
			(send base :rotate (* -1 (/ pi 2)) :y)
			(send base :locate #f(-48.9 0 -330))

			(setq link3
				(instance bodyset-link
					:init
						(make-cascoords)
						:bodies (list cy0)
						:name 'link3
				)
			)
			(send link3 :assoc link2)

			(setq base
				(instance bodyset-link
					:init
						(make-cascoords)
						:bodies (list base)
						:name 'base
				)
			)
			(send base :assoc link3)

			(setq
				j0 (instance rotational-joint
					:init
						:name 'j0
						:parent-link back
						:child-link wing
						:axis :z :max 1000000 :min -1000000
				)
				j1 (instance rotational-joint
					:init
						:name 'j1
						:parent-link link2
						:child-link link1
						:axis :x :max 60 :min -60
				)
				j2 (instance linear-joint
					:init
						:name 'j2
						:parent-link link3
						:child-link link2
						:axis :z :max 25 :min -25
				)
			)

			(send base :rotate (/ pi 2) :y)
			(send base :locate #f(-300 0 48.9))

			(setq links
				(list back front wing link1 link2 link3 base)
			)
			(setq joint-list (list j0 j1 j2))

			(send self :init-ending)

			self
		)
	)
)

(setq fan (instance fan :init))
(objects (list fan))

