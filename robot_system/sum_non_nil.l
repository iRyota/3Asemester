; 1)
;;(defun summit (lst)
;;	 (remove nil lst)
;;	 (apply #'+ lst'))

(defun summit_1 (lst)
	(setq lst (remove nil lst))
	(apply #'+ lst)
)



; 2)
;;(defun summit (lst)
;; 	 (let ((x (car lst)))
;;		 (if (null x)
;;			 (summit (cdr lst))
;;			 (+ x (summit (cdr lst))))))

(defun summit_2 (lst)
	(if (null lst)
		0
		(let ((x (car lst)))
			(if (null x)
				(summit_2 (cdr lst))
				(+ x (summit_2 (cdr lst)))
			)
		)
	)
)