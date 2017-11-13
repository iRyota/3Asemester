(load "6dof.l")

(setq r (instance 6dof :init))
(objects (list r))

(setq i 0)
(setq j 0)
(do-until-key
	(setq c
		(make-cascoords
			:pos (float-vector
				(* 50 (- 1 (cos (/ i 100.0))))
				0
				(+ (* 50 (- 1 (sin (/ i 100.0)))) 2)
			)
		)
	)
	(incf i)
	(send r :inverse-kinematics c
		:link-list (send r :link-list (send r :end-coords :parent))
		:move-target (send r :end-coords) :rotation-axis nil
	)
	(send *irtviewer* :draw-objects)
	(send c :draw-on)
)