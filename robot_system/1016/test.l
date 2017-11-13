(load "wing.l")

(defun make-cover nil
	(let (c)
		(setq tmp0 (make-cylinder 95 0.2 :segments 64))
		(setq tmp1 (make-cylinder 88 2 :segments 64))
		(send tmp1 :locate #f(0 0 -1))
		(setq c (body- tmp0 tmp1))
		(send c :locate #f(0 0 -0.4))
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		;;
		(setq c
			(body+ c
				(let (c0 c1 li)
					(setq li (list #f(10 35 0)))
					(dotimes (i 90)
						(setq li
							(cons
								(float-vector
									(+ (* 83 (cos (* (/ pi 180) (- 89 i)))) 10)
									(* 35 (sin (* (/ pi 180) (- 89 i))))
									0
								)
								li
							)
						)
					)
					(dotimes (i 91)
						(setq li
							(cons
								(float-vector
									(+ (* 80 (cos (* (/ pi 180) i))) 10)
									(* 32 (sin (* (/ pi 180) i)))
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
			)
		)
		(send c :rotate (/ pi 10) :z)
		c
	)
)


(setq cover (make-cover))
