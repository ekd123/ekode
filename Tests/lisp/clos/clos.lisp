(defclass clsAnimal ()
  ((name
    :initarg :name
    :initform (error "必须给个名字"))))
(defclass clsCat (clsAnimal)
  ())
(defclass clsDog (clsAnimal)
  ())
(defclass clsBone ()
  ())
(defclass clsMeat ()
  ())

;; generic，在emacs中选定按 M-; 取消注释
;; (defgeneric meow (animal)
;;   (:documentation "make the animal say meow"))

;; (defgeneric eat (animal food)
;;   (:documentation "animal eats food"))

;; method: meow
(defmethod meow ((animal clsCat))
  (format t "meow~%"))
(defmethod meow ((animal clsDog))
  (format t "狗不会喵喵叫。~%"))

;; method: eat
;; food: meat, bone
(defmethod eat ((animal clsCat) (food clsBone))
  (format t "猫不吃骨头！~%"))
(defmethod eat ((animal clsDog) (food clsBone))
  (format t "um um um~~~%"))
(defmethod eat ((animal clsCat) (food clsMeat))
  (format t "um um um~%"))
(defmethod eat ((animal clsDog) (food clsMeat))
  (format t "虽然喜欢，但更喜欢骨头~%"))

;; 定义对象
(defparameter supercat (make-instance 'clsCat :name "Super Cat"))
(defparameter lazydog (make-instance 'clsDog :name "Lazy Dog"))
(defparameter normal-cat (make-instance 'clsCat :name "Normal Cat"))

(defparameter bone (make-instance 'clsBone))
(defparameter meat (make-instance 'clsMeat))

;; 针对 supercat 再次进行针对对象的特化
(defmethod meow ((animal (eql supercat)))
  (format t "喵喵喵~%"))

;; 测试
(meow supercat)
(meow lazydog)
(meow normal-cat)

(eat supercat bone)
(eat supercat meat)

(eat normal-cat bone)
(eat normal-cat meat)

(eat lazydog bone)
(eat lazydog meat)

