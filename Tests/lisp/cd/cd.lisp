;; This file is an example from Practical Lisp Programming

;; generate a cd object
(defun mk-cd (title artist rating ripped) 
  (list :title title :artist artist :rating rating :ripped ripped))

;; define the database variable
(defvar *db* nil)

;; add a record to *db*
(defun add-record (cd)
  (push cd *db*))

;; Dump this db
(defun dump-db ()
  (dolist (cd *db*)
    (format t "~{~a: ~10t~a~%~}~%" cd)))

;; print a "prompt: " and return what user input
(defun prompt-read (prompt)
  (format *query-io* "~a: " prompt)
  (force-output *query-io*)
  (read-line *query-io*))

;; ask user for a new cd item
(defun prompt-for-cd ()
  (mk-cd
   (prompt-read "Title")
   (prompt-read "Artist")
   (or (parse-integer (prompt-read "Rating") :junk-allowed t) 0)
   (y-or-n-p (prompt-read "Ripped [y/n]"))))

;; made easier to add cds
(defun add-cds ()
  (loop (add-record (prompt-for-cd))
     (if (not (y-or-n-p "Another? [y/n]: ")) (return))))

;;save database to a file
(defun save-db (filename)
  (with-open-file (out filename
                       :direction :output
                       :if-exists :supersede)
    (with-standard-io-syntax 
        (print *db* out))))

;; load a db from file
(defun load-db (filename)
  (with-open-file (in filename)
    (with-standard-io-syntax 
      (setf *db* (read in)))))

;; query
(defun select-by-artist (artist)
  (remove-if-not 
   #'(lambda (cd) (equal (getf cd :artist) artist))
   *db*))

(defun select (selector-fn)
  (remove-if-not selector-fn *db*))

(defun artist-selector (artist)
  #'(lambda (cd) (equal (getf cd :artist) artist)))





