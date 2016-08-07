#! /usr/bin/env clisp

(defun cat (f)
  (let ((in (open f :if-does-not-exist nil)))
    (when in
      (loop for line = (read-line in nil)
         while line do (format t "~a~%" line))
      (close in))))

(cat *args*)
