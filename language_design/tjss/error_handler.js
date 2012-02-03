/*	
 *  Copyright (C) 2012  
 *	"Mu Lei" known as "NalaGinrut" <NalaGinrut@gmail.com>
 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

function JSError(message ,type ,isPrefix) {
    this.message = message;
    this.type = type === undefined ? '' : type;
    this.isPrefix = isPrefix === undefined ? true : isPrefix;
}

JSError.prototype.toString = function() {
    return this.type + (this.isPrefix ? 'Error' : '') + ': ' + this.message;
}

function UnboundVariableError(message) {
    return new JSError(message, 'UnboundVariable');
}

function IllegalArgumentError(message) {
    return new JSError(message, 'IllegalArgument');
}

function IllegalArgumentCountError(func ,how ,req ,cnt) {
    return IllegalArgumentError('The procedure ' + func + ' has been called' +
				' with ' + cnt + ' argument' + (cnt == 1 ? ';' : 's;')
				+ ' it requires ' + how + ' ' +  req + 
				' argument' + (req == 1 ? '.' : 's.'));
}

function IllegalArgumentTypeError(func ,arg ,cnt) {
    return IllegalArgumentError('The object ' + Util.format(arg) + ', passed as '+
				'argument ' + cnt + ' to ' + 
				func + ', is not the correct type.');
};

/* ------------------- */

function JSWarning(message ,type ,ignorable) {
    this.message = message;
    this.type = type === undefined ? '' : type;
    this.ignorable = ignorable === undefined ? false : ignorable;
}

JSWarning.prototype.isIgnorable = function() {
    return this.ignorable;
}

JSWarning.prototype.toString = function() {
    return this.type + 'Warning: ' + this.message;
}

function ParseWarning(message) {
    return new JSWarning(message ,'Parse');
}

function IgnorableParseWarning(message) {
    return new JSWarning(message ,'IgnorableParse' ,true);
}
