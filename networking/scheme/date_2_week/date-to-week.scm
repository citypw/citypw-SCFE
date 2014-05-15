#!/bin/sh
":"; exec /usr/local/plt/bin/mzscheme -rq $0 "$@"

;; Above two statements start a process its shell.and load the interpreter

;; Copyleft(c)2009 HackerFellowship. All lefts reserved.
;; Shawn the R0ck <citypw@gmail.com>
;;
;; This is a CGI program written in scheme language,to achive the goal which convert the
;; date from user input to weekday.It's a simple hacks for scheme newbie to learn.
;; May Lord Bless!Happy Hacking!
;; The math model:wd=d+[(13m-1)/5]+y+[y/4]+[c/4]-2c mod 7
;;
;; This program is free software;you can redistribute it and /or modify it under  
;; the terms of the GNU General Public License as published by the Free Software  
;; Foundataion;either version 2 of the License,or (at your option) any later version.
;;
;; You should have read the GNU General Public License before start "RTFSC".
;; If not,see <http://www.gnu.org/licenses/>


(display "content-type: text/html")
(newline)
(newline)

;; Just transform the math model to computing model

(define date-to-weekday (lambda (year month day)
                          (let ((c (truncate (/ year 100)))
                                (y (remainder year 100))
                                (m (- month 2))
                                (d day))
                           (if (< year 1752) (display "input-wrong-year")
                            (modulo
                             (-
                              (+ d
                                 (truncate (/ (- (* 13 m) 1) 5))
                                 y
                                 (truncate (/ y 4))
                                 (truncate (/ c 4)))
                              (* 2 c)) 7)))))

;; We have to parse the parameters.If the http request were "post",we could get the
;; parameter from STDIN(read).If the http request were "GET",just do getenv "QUERY_STRING"

(define string-index
  (lambda (s c)
    (let ((n (string-length s)))
      (let loop ((i 0))
        (cond ((>= i n) #f)
              ((char=? (string-ref s i) c) i)
              (else (loop (+ i 1))))))))

;; Split breaks string s into substrings separated by character c

(define split
  (lambda (c s)
    (let loop ((s s))
      (if (string=? s "") '()
          (let ((i (string-index s c)))
            (if i (cons (substring s 0 i)
                        (loop (substring s (+ i 1)
                                         (string-length s))))
                (list s)))))))

;; Get length and STDIN

(define length (string->number (getenv "CONTENT_LENGTH")))
(define read-s (symbol->string (read)))

;; If client has not been sent anything to CGI,just quit!
(define is-s-empty (lambda (length)
		     (if (= length 0)
			 (begin
			   (display "length is 0,stoped!")
			   (exit))
			 '())))

;; display the result of which weekday

(define display-result (lambda (day)
			 (case day
			   ((0) 'Sunday)
			   ((1) 'Monday)
			   ((2) 'Tuesday)
			   ((3) 'Wednesday)
			   ((4) 'Thursday)
			   ((5) 'Friday)
			   ((6) 'Saturday))))

;; If the condition is true,quit.If not,copy the result of split to list l1

(is-s-empty length)

(define l1 (split #\& read-s))

(define args
  (map (lambda (par-arg)
         (split #\= par-arg))
       l1))

;; get strings of year,month and day from a associate list

(define y (cadr (assoc "year" args)))
(define m (cadr (assoc "month" args)))
(define d (cadr (assoc "day" args)))

(define y (string->number y))
(define m (string->number m))
(define d (string->number d))

(display "<html>")
(newline)
(display "<head></head")
(newline)
(display "<body>")
(display "The date of ")
(display y)
(display "-")
(display m)
(display "-")
(display d)
(display " is:")
(define tmp-day (date-to-weekday y m d))
(display (display-result tmp-day))
(display "</body>")