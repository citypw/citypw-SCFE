;;  Copyright (C) 2012
;;      "Mu Lei" known as "NalaGinrut" <NalaGinrut@gmail.com>
;;  Ragnarok is free software: you can redistribute it and/or modify
;;  it under the terms of the GNU General Public License as published by
;;  the Free Software Foundation, either version 3 of the License, or
;;  (at your option) any later version.

;;  Ragnarok is distributed in the hope that it will be useful,
;;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;  GNU General Public License for more details.

;;  You should have received a copy of the GNU General Public License
;;  along with this program.  If not, see <http://www.gnu.org/licenses/>.

;; ----------------------
;; environment definition
;; ----------------------

(include "error.scm")

(define (environment? env) (hash-table? (cdr env)))
(define (make-frame last-env symbol-table) (cons last-env symbol-table))
(define (get-symbol-table env) (cdr env))
(define (prev-frame env) (car env))
(define (set-symbol-value! ht k v) (hash-set! ht k v))
(define (set-symbol-value-from-env! sym val env)
  (hash-set! (get-symbol-table env) sym val))
(define (new-symbol-table) (make-hash-table))

(define extend-environment
  (lambda (base-env vars vals)
    (let ([new-symbol-table (new-symbol-table)])
      (for-each (lambda (var val)
		  (set-symbol-value! new-symbol-table var val))
		vars vals)
      (make-frame base-env new-symbol-table))))

(define get-sym-from-table 
  (lambda (sym-table sym)
    (if (hash-table? sym-table)
	(hash-ref sym-table sym)
	#f)))
    
;; search sym from current-env to prev-env till top-level-env which can be  
;; so-called lexical scope
(define lookup-symbol-value
  (lambda (sym env)
    (let lp ([env env])
      (if (not env) (throw 'mmr-error "unbound variable: ~a~%" sym
			   '*error-unbound-variable*)
       (let* ([sym-table (get-symbol-table env)]
	      [r (get-sym-from-table sym-table sym)]
	      )
	 (if r 
	     r
	     (lp (prev-frame env))))))))

(define symbol-value-set!
  (lambda (sym val env)
    (let ([t (get-symbol-table env)])
      (set-symbol-value! t sym val))))
  
(define define-variable!
  (lambda (sym val env)
    (symbol-value-set! sym val env)))

(define set-variable-value!
  (lambda (sym val env)
    (symbol-value-set! sym val env)))

(define list-bisect
  (lambda (ll)
    (let lp ([rest ll] [a '()] [b '()])
      (if (null? rest)
	  (cons a b) ;; maybe 'values' better?
	  (lp (cdr rest) 
	      (cons (caar rest) a) 
	      (cons (cadar rest) b))))))

