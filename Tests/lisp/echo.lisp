(require :asdf)
(asdf:load-system :cl-xmpp)

;; 
;; must write following code in it
;; (defvar username "USERNAME")
;; (defvar password "PASSWORD")
;; 
(load "./botinit.lisp")

;;;; 鏈接XMPP
(defparameter *connection* (xmpp:connect :hostname "jabber.org"))
(xmpp:auth *connection* username password "紅繁君的機器人")

(defclass 玩家 ()
  ((名字 :initarg :名字 :accessor 名字)
   (血 :initarg :血 :accessor 血)))
(defvar *random-to-say* #("這句話說完了！" "句號" "真是不錯吶" "呵呵" "還是洗洗睡吧" "不作死就不會死！" "有本事說句「喵」啊~" "其實我是貓形機器人"))
(defvar *遊戲玩家* ())

(defun vector-last (obj)
  (elt obj (1- (length obj))))

(defun random-select (vector)
  (elt vector (random (length vector))))

(defun 輸出幫助 (conn from)
  (xmpp:message conn from "
括號機器人告訴你

括號機器人的宗旨是反對把 （ 作爲句號，

至於遊戲什麼的，我想我不告訴你，你肯定不會啓動~"))

(defun 去除用戶名 (msg)
  "去除諸如「[username] message」裏面的「[username] 」。"
  (subseq msg (1+ (search #(#\Space) msg :test #'char=))))

(defun 去除命令 (msg cmd)
  "去除諸如「（cmd args」裏面的「（cmd」"
  (subseq msg (1+ (mismatch (format nil "（~a" cmd) msg :test #'char=))))

(defun 處理括號 (conn from msg)
  "當有以「（」結尾的，即刻增加一個），並加上自己的話語。"
  (if (or (string-equal (string (vector-last msg)) "（")
          (string-equal (string (vector-last msg)) "("))
      (xmpp:message conn from (format nil "~a）" (random-select *random-to-say*)))))

(defun 處理喵 (conn from msg)
  "遇到「喵」就以十個「喵」反擊。"
  (if (search #(#\喵) msg)
      (xmpp:message conn from "喵喵喵喵喵喵喵喵喵喵")))

(defun 尋找玩家 (name)
  (dolist (玩家 *遊戲玩家*)
    (if (string-equal (名字 玩家) name)
        (return-from 尋找玩家 玩家))))

(defun 遊戲-參戰 (conn from msg)
  (let ((玩家 (make-instance '玩家 :名字 (string-trim #(#\Space) msg) :血 (random 100))))
    (push 玩家 *遊戲玩家*)
    (xmpp:message conn from (format nil "玩家 ~a 已參戰！（隨機血量 ~a）" (名字 玩家) (血 玩家)))))

(defun 遊戲-攻擊 (conn from msg)
  (let ((玩家 (尋找玩家 (string-trim #(#\Space) msg)))
        (攻擊血 (random 10)))
    (if 玩家
        (progn 
          (decf (血 玩家) 攻擊血)
          (xmpp:message conn from (format nil "玩家 ~a 被命中 ~a 滴血，當前血量 ~a" (名字 玩家) 攻擊血 (血 玩家))))
        (progn 
          (xmpp:message conn from (format nil "玩家 ~a 未找到" msg))))))

(defun 遊戲-結束 (conn from)
  (xmpp:message conn from "遊戲結束，戰報如下")
  (dolist (玩家 *遊戲玩家*)
    (xmpp:message conn from (format nil "~a ~a" (名字 玩家) (血 玩家))))
  (setf *遊戲玩家* ()))

(defun 遊戲-直接殺死 (conn from msg)
  (let ((玩家 (尋找玩家 (string-trim #(#\Space) msg))))
    (setf (slot-value 玩家 '血) 0)
    (xmpp:message conn from (format nil "玩家 ~a 已被直接殺死……" (名字 玩家)))))

(defun 遊戲邏輯 (conn from msg)
  (cond
    ((equal (search "（幫助" msg) 0)
     (輸出幫助 conn from))
    ((equal (search "（參戰" msg) 0)
     (遊戲-參戰 conn from (去除命令 msg "參戰")))
    ((equal (search "（攻擊" msg) 0)
     (遊戲-攻擊 conn from (去除命令 msg "攻擊")))
    ((equal (search "（結束" msg) 0)
     (遊戲-結束 conn from))
    ((equal (search "（直接殺死" msg) 0)
     (遊戲-直接殺死 conn from msg))
    ((equal (search "（新段子" msg) 0)
     (vector-push msg *random-to-say*))))

(defmethod xmpp:handle ((connection xmpp:connection) (message xmpp:message))
  (let ((message (去除用戶名 (xmpp:body message)))
        (from (xmpp:from message)))
    (處理括號 connection from message)
    (處理喵 connection from message)
    (遊戲邏輯 connection from message)))

(xmpp:receive-stanza-loop *connection*)
(xmpp:disconnect *connection*)

