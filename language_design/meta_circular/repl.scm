#! /usr/local/bin/guile \
-e main
!#
;;  Copyright (C) 2012
;;      "Mu Lei" known as "NalaGinrut" <NalaGinrut@gmail.com>
;;  This program is free software: you can redistribute it and/or modify
;;  it under the terms of the GNU General Public License as published by
;;  the Free Software Foundation, either version 3 of the License, or
;;  (at your option) any later version.

;;  This program is distributed in the hope that it will be useful,
;;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;  GNU General Public License for more details.

;;  You should have received a copy of the GNU General Public License
;;  along with this program.  If not, see <http://www.gnu.org/licenses/>.

(include "meta_circular.scm")

(define prompt-input "?-> ")
(define prompt-output " ==| ")

(define* (read-sexp #:optional (port (current-input-port)))
  ;; well, now we just use primitive 'read'
  ;; we'll extend it when we have a parser
  (read port))
  
(define (print-prompt)
  (format #t "~%~a" prompt-input))

(define (print-result obj)
  (format #t "~a" prompt-output)
  (if (compound-procedure? obj)
      (display (list 'compound-procedure
                     (procedure-parameters obj)
                     (procedure-body obj)
                     '<procedure-env>))
      (display obj)))

(define (show-env)
  (let ([t (cdr (*top-level*))])
    (newline)
    (hash-for-each (lambda (k v)
		     (format #t "~a:~a~%" k v))
		   t)))

(define* (run-repl #:key (debug #f))
  (let prompt-loop ()
    (print-prompt)
    (let* ([exp (read-sexp)]
	   [out (if (eof-object? exp)
                    (primitive-exit) 
                    (my-eval exp *top-level*))]
	   )
      (print-result out))
    (and debug (show-env))
    (prompt-loop)))

(define (main args)
  (run-repl))
      
		