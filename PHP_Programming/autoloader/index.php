<?php
/*
;;  Copyleft (C) 2012
;;     "b.tag" <bb.qnyd@gmail.com>
;;
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
*/

require_once 'autoloader.php';
$load = new Say();
$load->sayHello();

// new badegg

$bad = new Badegg();
$bad->bad("\nIt's bad.");

?>
