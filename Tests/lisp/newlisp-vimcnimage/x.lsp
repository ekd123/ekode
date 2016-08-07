#!/usr/bin/env newlisp
(constant 'dest-url "img.vim-cn.com")

(set 'opts (main-args))
(dotimes (i 2) (pop opts))

(dolist (item opts)
    (print (string item ": "))
    (! (string "curl -F 'name=@" item "' " dest-url)))

(exit)
