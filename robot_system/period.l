(defun period_l (end)
	(do ((i 1 (+ i 1)))
		((> i end) 'done)
		(format t "~A~%" '.)))


(defun period_r (i end)
	(if (> i end)
		'done
		(progn (format t "~A~%" '.')
			(period_r (+ i 1) end))))