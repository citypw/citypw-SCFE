
;; Copyleft(c)2009 HackerFellowship. All lefts reserved.
;; Shawn the R0ck <citypw@gmail.com>
;;
;; This is a simple tcp server program which written by scheme.A few procedures will
;; Gave you a way to understand what is the manchism of tcp.
;; May Lord Bless!Happy Hacking!
;;
;; This program is free software;you can redistribute it and /or modify it under  
;; the terms of the GNU General Public License as published by the Free Software  
;; Foundataion;either version 2 of the License,or (at your option) any later version.
;;
;; You should have read the GNU General Public License before start "RTFSC".
;; If not,see <http://www.gnu.org/licenses/>

;; The lib we need
(require scheme/tcp)

;; Set a port for listening
(define tcp-port-listener (tcp-listen 5656))

;; This procedure is a dead loop.It always listening the clients who send the request
;; And response to them.
(define listening (lambda ()
		    (define-values (client->me me->client)
		      (tcp-accept tcp-port-listener))
		    (set! output-file (open-output-file "server.info" #:exists 'truncate))
                    (let loop((me (read client->me)))
                      (cond
		       ((eof-object? me) '())
		       ((list? me)
                        (begin
                        (display "running")
  			(let loop2((ls me))
                          (if (null? (cdr ls)) (write (car ls) output-file)                               
                             (begin
                               (display (car ls))
                               (newline)
                               (write (car ls) output-file)
                               (newline output-file)
                               (loop2 (cdr ls)))))))
                         (else 
                          (begin
                           (display me)
                           (newline)
                           (loop (read client->me))))))
                               (close-output-port output-file)
                               (close-input-port client->me)
                               (close-output-port me->client)
			       (sleep 3)
                    (listening)))

;; In scheme language,open the file as a port,which you can manipulate pretty easily.
 (define output-file (with-handlers 
		      ((exn:fail? (lambda (exn) "cant open file,please check it out!"))) 
		      (open-output-file "server.info" #:exists 'truncate)))