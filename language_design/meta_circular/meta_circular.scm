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

(include "env.scm")

;; seems a little complicated, but it's tail recursive	     
;; this proc is used to eval all the exps of the arg-list
;; besides, it's always left-to-right order
(define list-of-values
  (lambda (exps env)
    (if (null? exps)
	'()
	(map (lambda (e)
	       (if (application? e)
		   (list-of-values e env)
		   (my-eval e env)))
	     exps))))

(define (true? x)
  (not (eq? x #f)))
(define (false? x)
  (eq? x #f))

(define tagged-list? 
  (lambda (exp tag)
    (if (pair? exp)
	(eq? (car exp) tag)
	#f)))

(define (self-eval? exp)
  (or (number? exp) (string? exp)))

(define (self-eval-handler exp env)
  exp)

(define (variable? exp) (symbol? exp))
(define (variable-handler exp env) 
  (lookup-symbol-value exp env))
  

(define (quoted? exp) (tagged-list? exp 'quote))
(define (quote-handler exp env) (cadr exp))

(define (assignment? exp) (tagged-list? exp 'set!))
(define (assignment-variable e) (cadr e))
(define (assignment-value e) (caddr e))
(define (assignment-handler exp env)
  (set-variable-value! (assignment-variable exp)
                       (my-eval (assignment-value exp) env)
                       env)
  'ok) ;; don't return 'unspecified here, do it in set!

(define (definition? exp)
  (tagged-list? exp 'define))
(define (definition-variable exp)
  (if (symbol? (cadr exp))
      (cadr exp)
      (caadr exp)))
(define (definition-value exp)
  (if (symbol? (cadr exp))
      (caddr exp)
      (make-lambda (cdadr exp)   ; formal parameters
                   (cddr exp)))) ; body
(define (definition-handler exp env)
  (define-variable! (definition-variable exp)
                    (my-eval (definition-value exp) env)
                    env)
  'ok)

(define (lambda? exp) (tagged-list? exp 'lambda))
(define (lambda-parameters e) (cadr e))
(define (lambda-body e) (cddr e))
(define (make-lambda parameters body)
  (cons 'lambda (cons parameters body)))
(define (lambda-handler exp env)
  (make-procedure (lambda-parameters exp)
	       (lambda-body exp)
	       env))

(define (let? exp) (tagged-list? exp 'let))
(define (regular-let-binding-list e) (cadr e))
(define (regular-let-body e) (caddr e))
(define (named-let-name e) (cadr e))
(define (named-let-parameters e) (caddr e))
(define (named-let-body e) (cadddr e))
(define (regular-let? exp) (list? (cadr exp))) ;; (let ((...)) ...)
(define (named-let? exp) (not (list? (cadr exp)))) ;; (let name ((...)) ...)
(define (let->combination exp env)
  (cond
   ((null? exp) 
    '())
   ((named-let? exp)
    (define-variable! (named-let-name exp) 
                      (my-eval (named-let-body exp) env)
		      env))
   ((regular-let? exp)
    (let* ([binds (list-bisect (regular-let-binding-list exp))]
	   [vars (car binds)]
	   [vals (cdr binds)]
	   [body (make-procedure vars (regular-let-body exp) env)]
	   )
      (let lp ([vl vals] [result body])
	(if (null? vl)
	    result
	    (lp (cdr vl) 
		(make-application result vl))))))
   (else
    (throw 'mmr-error "invalid let syntax: ~a~%" exp))))
(define (let-handler exp env)
  (let->combination exp env))

(define (if? exp) (tagged-list? exp 'if))
(define (if-predicate e) (cadr e))
(define (if-consequent e) (caddr e))
(define (if-alternative exp)
  (if (not (null? (cdddr exp)))
      (cadddr exp)
      #f))
(define (make-if predicate consequent alternative)
  (list 'if predicate consequent alternative))
(define (if-handler exp env)
  (if (true? (my-eval (if-predicate exp) env))
      (my-eval (if-consequent exp) env)
      (my-eval (if-alternative exp) env)))

(define (begin? exp) (tagged-list? exp 'begin))
(define (begin-actions e) (cdr e))
(define (last-exp? seq) (null? (cdr seq)))
(define (first-exp e) (car e))
(define (rest-exps e) (cdr e))
(define (begin-handler exps env)
  (eval-sequence (begin-actions exps) env))

(define (make-procedure parameters body env)
  (list 'procedure parameters body env))
(define (compound-procedure? p)
  (tagged-list? p 'procedure))
(define (procedure-parameters e) (cadr e))
(define (procedure-body e) (caddr e))
(define (procedure-environment e) (cadddr e))
(define (sequence->exp seq)
  (cond 
   ((null? seq) seq)
   ((last-exp? seq) (first-exp seq))
   (else 
    (make-begin seq))))
(define (make-begin seq) (cons 'begin seq))

(define (application? exp) (pair? exp))
(define (operator exp) (car exp))
(define (operands exp) (cdr exp))
(define (no-operands? ops) (null? ops))
(define (first-operand ops) (car ops))
(define (rest-operands ops) (cdr ops))
(define (make-application proc args) (cons proc args))
(define (application-handler exp env)
  (my-apply (my-eval (operator exp) env)
	    (list-of-values (operands exp) env)))

(define (cond? exp) (tagged-list? exp 'cond))
(define (cond-clauses e) (cdr e))
(define (cond-predicate e) (car e))
(define (cond-actions e) (cdr e))
(define (cond-else-clause? clause)
  (eq? (cond-predicate clause) 'else))
(define (cond->if exp) ;; reverse for the later tail-recursive
  (expand-clauses (reverse (cond-clauses exp))))
(define (expand-clauses clauses)
  (if (null? clauses)
      #f
      (let lp ([rest clauses] [result '()])
	(let ([first (car rest)])
	(cond
	 ((cond-else-clause? first)  ;; else-clause
	  (if (null? (cdr rest))
	      (sequence->exp (cond-actions first)) ;; else === begin
	      (throw 'mmr-error "ELSE clause isn't last -- COND->IF" clauses)))
	 ((null? rest) ;; return result
	  result)
	 (else
	  (lp (cdr rest) (make-if (cond-predicate first)
				  (sequence->exp (cond-actions first))
				  result))))))))
(define (cond-handler exp env)
  (my-eval (cond->if exp) env))

(define (primitive-procedure? proc)
  (tagged-list? proc 'primitive))
	
(define (primitive-implementation e) (cadr e))

(define (show-author)
  (display "This program is written by NalaGinrut\n"))

(define primitive-procedures
  `((car ,car)
    (cdr ,cdr)
    (cons ,cons)
    (null? ,null?)
    (+ ,+)
    (- ,-)
    (* ,*)
    (/ ,/)
    (author ,show-author)
    ;; TODO: add more primitive-procedures
    ))

(define (apply-primitive-procedure proc args)
  ;; well, for a newbie sample, we just use 'apply' directly.
  ;; we'll extend it when we have an real interpretor backend.
  (apply
   (primitive-implementation proc) args))
    
;; well, this is obviously applicable-order
(define my-eval
  (lambda (exp env)
    (call/cc (lambda (return)
	       (for-each (lambda (x)
			   (let ([predicate? (car x)]
				 [handler (cadr x)]
				 )
			     (if (predicate? exp)
				 (return (handler exp env)))))
			 *proc-handler-list*)
	       (throw 'mmr-error 
		      "I don't know how to handler this grammar:~a~%"
		      exp env)))
    ))

;; evaluate exp-sequences one bye one
(define (eval-sequence exps env)
  (if (last-exp? exps) 
      (my-eval (first-exp exps) env)
      (begin
	(my-eval (first-exp exps) env)
	(eval-sequence (rest-exps exps) env))))

(define my-apply
  (lambda (proc args)
    (cond 
     ((primitive-procedure? proc)
      (apply-primitive-procedure proc args))
     ((compound-procedure? proc)
      (eval-sequence
       (procedure-body proc)
       (extend-environment
	(procedure-environment proc)
	(procedure-parameters proc)
	args)))
     (else
      (throw 'mmr-error "Unknown procedure:~a" proc)))))

;; meta-circular-init will do init work and return top-level-envrionment
(define (meta-circular-init)
  (let* ([ll (list-bisect primitive-procedures)]
	 [names (car ll)]
	 [procs (cdr ll)]
	 [pri-procs (map (lambda (p) (list 'primitive p)) procs)]
	 [top-level (extend-environment 
		     #f ;; null environment
		     names
		     pri-procs)]
	 )
    top-level))

(define *top-level* (meta-circular-init))

;; data-oriented proc-handle
;; (predicate? exp)
;; (proc-handler exp env)
;; TODO: enhance it with a hash-table
(define *proc-handler-list*
  `((,self-eval? ,self-eval-handler)
    (,variable? ,variable-handler)
    (,quoted? ,quote-handler)
    (,assignment? ,assignment-handler)
    (,definition? ,definition-handler)
    (,if? ,if-handler)
    (,lambda? ,lambda-handler)
    (,begin? ,begin-handler)
    (,cond? ,cond-handler)
    (,application? ,application-handler)
    (,let? ,let-handler)
    ))

