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

(use-modules (web client)
	     (web response)
	     (web request)
	     (web http)
	     (web uri)
	     (rnrs bytevectors)
	     (rnrs))

(define* (open-socket-for-ip ip #:key (port 80))
  (let ([s (socket PF_INET SOCK_STREAM 0)]
	[ip-addr (inet-pton AF_INET ip)]
	)
    (connect s AF_INET ip-addr port)
    s
    ))

(define* (http-get-uri-head uri #:key (sock (open-socket-for-uri uri)))
  (let ((rq (build-request uri #:method 'HEAD)))
    (write-request rq sock)
    (force-output sock)
    (let ((head (read-response sock)))
      (close sock)
      head)))

(define* (http-client-get-block-from-uri uri #:key (block #f) (start 0) 
					 (head (http-get-uri-head uri)))
  (let* ((s (open-socket-for-uri uri))
	 (end (if block (+ start block) (response-content-length head)))
	 (range-str (format #f "bytes=~a-~a" start end)) 
	 (range (parse-header 'range range-str))
	 (rq (build-request uri #:headers `((range ,@range)))))
    (write-request rq s)
    (force-output s)
    (read-response-body (read-response s))))

(define (http-client-get-check-string path)
  (let ((target (string-append path ".etag")))
    (if (file-exists? target)
	(call-with-input-file target
	  (lambda (port)
	    (get-string-all port)))
	"")))

(define* (http-client-checkout-etag uri #:key (path (uri-path uri)) 
			(head (http-get-uri-head uri)))
  (let* ((etag (car (response-etag head)))
	 (chk-str (http-client-get-check-string path))
	 )
    (string=? etag chk-str))) ;; checkout ETag

(define* (http-client-remove-check-file path #:key (ext ".etag"))
  (let ((chk-file (string-append path ext)))
    (and (file-exists? chk-file) (delete-file chk-file))))

(define* (http-client-etag-stamp uri #:key (head (http-get-uri-head uri))
				 (path (uri-path uri))
				 (ext ".etag"))
  (let ((chk-file (string-append path ext))
	(etag (car (response-etag head))))
    (and (file-exists? chk-file) (delete-file chk-file))
    (call-with-output-file chk-file
      (lambda (port)
	(format port "~a" etag)
	(close port)))))

(define* (http-client-get-ready-to-continue uri #:key (path (uri-path uri))
				     (head (http-get-uri-head uri)))
  (if (http-client-checkout-etag uri #:path path #:head head) ;; checkout ETag
      (let* ((fp (open-file path "a"))
	     (pos (stat:size (stat path))))
	(seek fp pos SEEK_SET)
	(http-client-remove-check-file path)
	(values pos fp))
      (let* ((fp (open-file path "w"))
	     (pos 0))
	(values pos fp))))

(define* (http-client-retrive-file-continue uri #:key (path (uri-path uri))
					    (try 5))
  (let ([head (http-get-uri-head uri)])
    (call-with-values 
	(lambda ()
	  (http-client-get-ready-to-continue uri #:path path #:head head))
      (lambda (pos port)
	(catch #t
	       (lambda ()
		 (if (zero? pos)
		     (begin
		       (display "download from beginning")
		       (http-client-etag-stamp uri #:path path))
		     (format #t "continue from position ~a~%" pos))
		 (let lp ([data (http-client-get-block-from-uri 
			      uri #:start pos #:head head #:block 4096)]
			  [pos pos])
		   (if data
		       (let* ([dl (bytevector-length data)]
			      [new-pos (+ pos dl)]
			      )
			 (put-bytevector port data)
			 (force-output port)
			 (format #t "~a-~a~%" pos new-pos)
			 (lp (http-client-get-block-from-uri
			      uri #:start new-pos #:head)
			     new-pos))
		       (format #t "~a has already been done!~%" path))))
	       (lambda e
		 (case (car e)
		   ((system-error)
		    (let ([E (system-error-errno e)])
		      (if (or (= E ECONNABORTED)
			      (= E ECONNREFUSED)
			      (= E ECONNRESET))
			  (begin
			    (format #t "~a, try again!~%left ~a times to try~%"
				    (car (cadddr e)) try)
			    (format #t "~a~%" e)
			    ;;(display (car (last-pair e)) port)
			    (close port)
			    (http-client-retrive-file-continue uri #:path path
							       #:try (1- try))))))
		   (else
		    (display "some error occured!\n")(newline)
		    (format #t "~a : ~a~%" (car e) (cdr e)))))
	       )))))


(define (test)
  (http-client-retrive-file-continue 
   (string->uri "http://mirrors.kernel.org/gnu/gcc/gcc-4.6.2/gcc-4.6.2.tar.bz2") 
   #:path "mmr.tar.bz2" #:try 10))