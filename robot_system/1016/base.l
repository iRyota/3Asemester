(defun make-base nil
	(let (b)
		(setq b
			(make-solid-of-revolution
				'(
					#f(0 0 30)
					#f(70 0 30)
					#f(100 0 0)
				)
				:segments 16
			)
		)
	)
)