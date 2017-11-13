(defun make-cover nil
	(let (cy)
	(setq cy (make-cylinder 10 5 :segments 16))
	(send cy :locate #f(0 0 31))
	(setq cy (body+ cy
	(let (tmp0 tmp1 c)
		(setq tmp0 (make-cylinder 95 0.2 :segments 16))
		(setq tmp1 (make-cylinder 88 2 :segments 16))
		(send tmp1 :locate #f(0 0 -1))
		(setq c (body- tmp0 tmp1))
		(send c :locate #f(0 0 -0.4))
		(setq tmp '())
		(dotimes (j 16)
			(setq tmp
				(cons
					(let (c0 c1 li)
						(setq li (list #f(10 35 0)))
						(dotimes (i 18)
							(setq li
								(cons
									(float-vector
										(+ (* 83 (cos (* (/ pi 36) (- 17 i)))) 10)
										(* 35 (sin (* (/ pi 36) (- 17 i))))
										0
									)
									li
								)
							)
						)
						(dotimes (i 19)
							(setq li
								(cons
									(float-vector
										(+ (* 80 (cos (* (/ pi 36) i))) 10)
										(* 32 (sin (* (/ pi 36) i)))
										0
									)
									li
								)
							)
						)

						(setq c0
							(make-prism
								li
								3
							)
						)
						(send c0 :rotate (/ pi 2) :x)

						(send c0 :locate #f(0 0 -1.5))

						c0
					)
					tmp
				)
			)

		)
		(dotimes (j 16)
			(setq c
				(body+
					c
					(car tmp)
				)
			)
			(setq tmp (cdr tmp))
			(send c :rotate (/ pi 8) :z)
		)
		c
	)
	))
	cy
	)
)


