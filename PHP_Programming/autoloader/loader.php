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

class Loader {
       public static function __autoload($file,$dirs=NULL){
               if (class_exists($file, false) || interface_exists($file, false)) {
                       return;
               }
               $file = basename($file).'.php';
               if (!empty($dirs)) {
                       // use the autodiscovered path
                       $dirPath = dirname($file);
                       if (is_string($dirs)) {
                               $dirs = explode(PATH_SEPARATOR, $dirs);
                       }
                       foreach ($dirs as $key => $dir) {
                               if ($dir == '.') {
                                       $dirs[$key] = $dirPath;
                               } else {
                                       $dir = rtrim($dir, '\\/');
                                       $dirs[$key] = $dir . DIRECTORY_SEPARATOR . $dirPath;
                               }
                       }

                       self::loadFile($file, $dirs, true);
               } else {

                       self::loadFile($file, null, true);
               }
       }

       public static function loadFile($filename, $dirs = null, $once = false)
       {
               if ($once) {
                       include_once $filename;
               } else {
                       include $filename;
               }
               return true;
       }
}

?>