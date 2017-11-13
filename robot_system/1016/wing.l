(defun make-wing nil
	(let (w0 w1 w2 w3 h b b1)

		(setq w0
			(make-prism
				'(#f(0 10 0) #f(60 40 0) #f(60 0 0) #f(0 0 0))
				1
			)
		)
		(setq w1
			(make-prism
				'(#f(-10 0 0) #f(-40 60 0) #f(0 60 0) #f(0 0 0))
				1
			)
		)
		(setq w2
			(make-prism
				'(#f(0 -10 0) #f(-60 -40 0) #f(-60 0 0) #f(0 0 0))
				1
			)
		)
		(setq w3
			(make-prism
				'(#f(10 0 0) #f(40 -60 0) #f(0 -60 0) #f(0 0 0))
				1
			)
		)

		(send w0 :locate #f(0 0 -0.1))
		(send w1 :locate #f(0 0 -0.1))
		(send w2 :locate #f(0 0 -0.1))
		(send w3 :locate #f(0 0 -0.1))

		(send w0 :rotate 10.0 :x)
		(send w1 :rotate 10.0 :y)
		(send w2 :rotate -10.0 :x)
		(send w3 :rotate -10.0 :y)

		(setq h
			(make-solid-of-revolution
				'(
					#f(0 0 20)
					#f(10 0 20)
					#f(20 0 0)
				)
				:segments 16
			)
		)

		(setq b
			(make-cube
				20 20 12
			)
		)
		(send b :locate #f(0 0 -5))

		(setq b1
			(make-cylinder
				15 31.4
				:segments 16
			)
		)
		(send b1 :locate #f(0 0 -41.4))

		(setq b (body+ h b))
		(setq b (body+ b w0))
		(setq b (body+ b w1))
		(setq b (body+ b w2))
		(setq b (body+ b w3))
		(setq b (body+ b b1))
		(send b :locate #f(0 0 10))
		b
	)
)
