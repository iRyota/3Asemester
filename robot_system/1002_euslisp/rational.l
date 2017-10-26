(defclass rat
	:super propertied-object
	:slots (numer denom)
)
(defmethod rat
	(:init
		(n d)
		(send self :set-numer n)
		(send self :set-denom d)
		self
	)
	(:print-rat
		()
		(format t "~A/~A~%" numer denom)
	)
	(:set-numer (n) (setq numer n))
	(:set-denom (d)	(setq denom d))
	(:get-numer () numer)
	(:get-denom () denom)
	(:add-rat
		(x)
		(setq ans
			(instance rat :init
				(+ (* numer (send x :get-denom)) (* denom (send x :get-numer)))
				(* denom (send x :get-denom))
			)
		)
		ans
	)
)
