;;  Copyright (C) 2011  
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

;; Matrix Manipulation
;; These code tested in GNU/Guile
;; Provide dot:+-*/ and cross*

(use-modules (srfi srfi-1))

(define (matrix:+ m1 m2)
  (matrix:linear-op '+))
(define (matrix:+ m1 m2)
  (matrix:linear-op '-))
(define (matrix:+ m1 m2)
  (matrix:linear-op '*))
(define (matrix:+ m1 m2)
  (matrix:linear-op '/))


(define vector-fold
  (lambda (v r proc)
    (let ([len (vector-length v)])
      (let lp ([i 0] [p r])
	(if (= i len)
	    p
	    (lp (1+ i)
		(proc (vector-ref v i) p)))))))

(define-syntax-rule (matrix-dimrow m)
  (car (array-dimensions m)))

(define-syntax-rule (matrix-dimcol m)
  (cadr (array-dimensions m)))

(define-syntax-rule (vector-* v1 v2)
  (vector-map * v1 v2))

(define (vector:cross* v1 v2)
  (vector-fold (vector-* v1 v2) 
	       0 
	       (lambda (e p) (+ e p))))

(define-syntax-rule (list:cross* l1 l2)
  (apply + ((list:linear-op *) l1 l2)))

(define (list:linear-op op)
  (lambda (l1 l2)
    (map op l1 l2)))

(define matrix-column
  (lambda (m n)
    (let lp ([i 0] [result '()])
      (if (= i (matrix-dimrow m))
	  (list->vector (reverse result))
	  (lp (1+ i) (cons (array-ref m i n) result))))))

(define matrix-row
  (lambda (m n)
    (let* ([col (matrix-dimcol m)]
	   [row (matrix-dimrow m)]
	   [v2 (make-vector col)]
	   [c (array-contents m)]
	   [s (* n col)]
	   [e (+ s col)] 
	   )
      (if (> n row)
	  (error matrix-row "invalid row!~"))
      (vector-move-left! c s e v2 0)
      v2)))

(define *row-op-table*
  `((+ ,(list:linear-op +))
    (- ,(list:linear-op -))
    (* ,(list:linear-op *))
    (/ ,(list:linear-op /))
    (cross* ,vector:cross*)
    ))
    
(define get-row-op
  (lambda (op)
    (car (assoc-ref *row-op-table* op))))

(define matrix:*
  (lambda (a b)
    (let ([ar (matrix-dimrow a)]
	  [ac (matrix-dimcol a)]
	  [br (matrix-dimrow b)]
	  [bc (matrix-dimcol b)]
	  )
      (if (not (= ac br))
	  (error matrix:* "wrong matrix multiply!~%"))
      (let lp ([arr ar] [bcc bc] [tmp '()] [result '()])
	(cond
	 ((= arr 0)
	  (list->array 2 result))
	 ((= bcc 0)
	  (lp (1- arr) bc '() (cons tmp result)))
	 ((> bcc 0)
	  (let ([arow (matrix-row a (1- arr))]
		[bcol (matrix-column b (1- bcc))]
		)
	    (lp arr (1- bcc) 
		(cons (vector:cross* arow bcol) tmp)
		result)))
	 (else
	  (error matrix:* "matrix mutiply error!~%")))))))

(define (matrix:linear-op op)
  (lambda (a b)
    (let ([ad (array-dimensions a)]
	  [bd (array-dimensions b)]
	  [row-op (get-row-op op)]
	  [r (matrix-dimrow a)]
	  )
      (if (not (equal? ad bd))
	  (error matrix:linear-op "two matrix's dimension must equal!~%"))
      (let lp ([r r] [result '()])
	(cond
	 ((= r 0)
	  (list->array 2 result))
	 ((> r 0)
	  (let ([arow (vector->list (matrix-row a (1- r)))]
		[brow (vector->list (matrix-row b (1- r)))]
		)
	    (lp (1- r) 
		(cons (row-op arow brow) result))))
	 (else
	  (error matrix:* "matrix op error!~%")))))))
      
