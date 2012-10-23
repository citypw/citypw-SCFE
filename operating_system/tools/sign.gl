;;  Copyright (C) 2012
;;	"Mu Lei" known as "NalaGinrut" <NalaGinrut@gmail.com>
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

(define main
  (lambda (args)
    (and (< (length args) 2) (error "usage: ~a filename ~%" (car args)))
    (let* ((filename (cadr args))
	   (fp (open-file filename "r+"))) ;; use "r+", or we can't do 'seek trick'.
      (unless (<= (stat:size (stat filename)) 510) 
	(format #t "File: ~s size is larger than 510B!~%" filename) (primitive-exit))
      (copy-file filename (string-append filename ".bak")) ;; A backup for debug
      (seek fp 510 SEEK_SET) ;; pad zero automatically
      (format fp "~a" "\x55\xAA") (close fp)
      (rename-file filename (string-append filename ".out"))
      (display "Seems the MBR has been generated successfully!!!\n"))))



