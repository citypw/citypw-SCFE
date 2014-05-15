#!/bin/sh
":"; exec /usr/local/plt/bin/mzscheme -rq $0 "$@"

;; Above two statements start a process its shell.and load the interpreter

;; Copyleft(c)2009 HackerFellowship. All lefts reserved.
;; Shawn the R0ck <citypw@gmail.com>
;;
;; This is a CGI program written in scheme language,to achive the goal which send a
;; Request to a simple tcp server which ask for server to save some infos of this 
;; Server on file name of "server.info".It's a simple hacks for scheme newbie to learn.
;; May Lord Bless!Happy Hacking!
;;
;; This program is free software;you can redistribute it and /or modify it under  
;; the terms of the GNU General Public License as published by the Free Software  
;; Foundataion;either version 2 of the License,or (at your option) any later version.
;;
;; You should have read the GNU General Public License before start "RTFSC".
;; If not,see <http://www.gnu.org/licenses/>

(display "content-type: text/plain")
(newline)
(newline)

(define output-list (lambda (output-file list-name)
                      (let loop((out list-name))
			  (if (null? (cdr out)) (write (car out) output-file)
			      (begin
				(write (car out) output-file)
				(newline output-file)
				(loop (cdr out)))))
(close-output-port output-file)))
 
(define output-port (open-output-file "server.info" #:exists 'truncate))

(define new-display (lambda (n)
		      (begin
			(display n)
			(newline))))
(display "The following environment variables are not request-specific and are set for all requests:SERVER_SOFTWARE,SERVER_NAME,GATEWAY_INTERFACE")
(newline)
(define non-request (cons (getenv "SERVER_SOFTWARE")
			  (cons (getenv "SERVER_NAME")
				(cons (getenv "GATEWAY_INTERFACE") '()))))
(for-each new-display non-request)

(newline)
(display "The following environment variables are specific to the request being fulfilled by the gateway 
program: SERVER_PROTOCOL,SERVER_PORT,SERVER_METHOD,PATH_INFO,PATH_TRANSLATED,SCRIPT_NAME,QUERY_STRING,
REMOTE_HOST,REMOTE_ADDR,AUTH_TYPE,REMOTE_USER_REMOTE_INDENT,CONTENT_TYPE,CONTENT_LENGTH")
(newline)
(define request (cons (getenv "SERVER_PROTOCOL")
		      (cons (getenv "SERVER_PORT")
		      (cons (getenv "SERVER_METH0D")
		      (cons (getenv "PATH_INFO")
		      (cons (getenv "PATH_TRANSLATED")
		      (cons (getenv "SCRIPT_NAME")
		      (cons (getenv "QUERY_STRING")
		      (cons (getenv "REMOTE_HOST")
		      (cons (getenv "REMOTE_ADDR")
		      (cons (getenv "AUTH_TYPE")
		      (cons (getenv "REMOTE_USER")
		      (cons (getenv "REMOTE_INDENT")
		      (cons (getenv "CONTENT_TYPE")
		      (cons (getenv "CONTENT_LENGTH") '())))))))))))))))
(for-each new-display request)

(newline)
(display "In addition to these, the header lines received from the client, if any, are placed into the environment
 with the prefix HTTP_ followed by the header name. Any - characters in the header name are changed to _ characters.
 The server may exclude any headers which it has already processed, such as Authorization, Content-type, and 
Content-length. If necessary, the server may choose to exclude any or all of these headers if including them 
would exceed any system environment limits.")
(newline)

(define HTTP-spec (cons (getenv "HTTP_ACCEPT")
		   (cons "HTTP_USER_AGENT" '())))
(for-each new-display HTTP-spec)

;(output-list output-port (append non-request (append request HTTP-spec)))

;; Client will try to connect the server,and port is 5566.

(define-values (server->me me->server)
        (tcp-connect "localhost" 5656))

;; Send the server infos to tcp server

(write (append non-request (append request HTTP-spec)) me->server)

;; remember that if you are not going to close the port,your infos still
;; in buffers that will not send

(close-output-port me->server)