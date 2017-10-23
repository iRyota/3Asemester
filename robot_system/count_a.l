(defun count_a_l (x)
	(setq cnt 0)
	(do ((li x (setq li (cdr li))))
		((null li) 'done)
		(if (eql 'a (car li))
			(setq cnt (+ cnt 1)) ; if
			()					 ; else
		)
	)
	(format nil "\'a\' appeared ~A times" cnt)
)

(defun count_a_r (cnt x)
	(if (null x)
		(format nil "\'a\' appeared ~A times" cnt)
		(progn
			(if (eql 'a (car x))
				(setq cnt (+ cnt 1))
				()
			)
			(count_a_r cnt (cdr x))
		)
	)
)