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

var Tokens = {
    AND: 'and',
    BEGIN: 'begin',
    BINARY: '^#b[01]+$',
    COND: 'cond',
    DECIMAL: '^(#d)?([+-])?([0-9]+)?[.]?[0-9]+([eE][+-]?[0-9]+)?$',
    DEFINE: 'define',
    DELAY: 'delay',
    DOT: '.',
    ELSE: 'else',
    EVAL: 'eval',
    HEX: '^#x[0-9a-fA-F]+$',
    IDENTIFIER: '^[^\\\',\\"\\s\\(\\)]+$',
    IF: 'if',
    LAMBDA: 'lambda',
    LET: 'let',
    LETREC: 'letrec',
    LET_STAR: 'let*',
    L_PAREN: '(',
    NEWLINE: '\n',
    OCTAL: '^#o[0-7]+$',
    OR: 'or',
    QUOTE: 'quote',
    R_PAREN: ')',
    SEMI_COLON: ';',
    SET: 'set!',
    SINGLE_QUOTE: '\'',
    SPACE: ' ',
    STRING: '^[\\"](([^\\"\\\\]|([\\\\].))*)[\\"]'
};

function HashTable(object) {
     this._table = Utils.isHash(object) ? object.toObject() : Utils.Clone(object);
}

HashTable.prototype.get = function(key) { 
    return this._table[key]; 
}
    
HashTable.prototype.set = function(key ,value) {
	this._table[key] = value;
	return this._table[key];
}

/* ----------------------- */
function JSString(string) {
    this.string = string;
}

JSString.prototype.toString = function() {
    return this.string;
}

/* ----------------------- */
    
function Utils() {
    var OR = '|';
    var JSCMLibs = new HashTable();

    this.createMatcher = function(regex) {
	return function(expr) {
	    return new RegExp(regex).test(expr);
	};
    }
}

Utils._isIdentifier = function() {
    return this.createMatcher(Tokens.IDENTIFIER);
}

Utils.isString = function() {
    return this.createMatcher(Tokens.STRING);
}

Utils.isBinary = function() {
    return this.createMatcher(Tokens.BINARY);
}

Utils.isDecimal = function() {
    return this.createMatcher(Tokens.DECIMAL);
}

Utils.isHex = function() {
    return this.createMatcher(Tokens.HEX);
}

Utils.isOctal = function() {
    return this.createMatcher(Tokens.OCTAL);
}

Utils.isNumber = function() { 
    return this.createMatcher(Tokens.BINARY + OR + Tokens.DECIMAL +
			      OR + Tokens.HEX + OR + Tokens.OCTAL);
}

Utils.isElement = function(object) {
    return object && object.nodeType == 1;
}

Utils.isArray = function(object) {
    return object != null && typeof object == "object" &&
    'splice' in object && 'join' in object;
}

Utils.isHash = function(object) {
    return object instanceof Hash;
}

Utils.isFunction = function(object) {
    return typeof object == "function";
}

Utils.isString = function(object) {
    return typeof object == "string";
}

Utils.isNumber = function(object) {
    return typeof object == "number";
}

Utils.isUndefined = function(object) {
    return typeof object == "undefined";
}

Utils.isIdentifier = function(expr) {
    return !this.isNumber(expr) && !this.isString(expr) &&
    this._isIdentifier(expr);
}

Utils.extend = function(dest ,src) {
    for (var property in src)
	dest[property] = src[property];
    return dest;
}

Utils.clone = function(obj) {
    return Utils.extend({} ,obj);
}

Utils.car = function(list) {
    return list[0];
}

Utils.cdr = function(list) {
    return list.slice(1);
}
    
Utils.cons = function(x ,y) {
    var tmp = [x].concat(y);
    return tmp;
}

Utils.getNumber = function(expr) {
    expr = expr.toString();
    if (this.isBinary(expr)) {
	var res = 0 ,pow = 0;
	for (var i = expr.length - 1 ;i > 1 ;i--) {
	    res += parseInt(expr[i]) * Math.pow(2 ,expr.length - i - 1);
	}
	return res;
    } else if (this.isDecimal(expr)) {
	if (expr.indexOf('.') != -1) {
	    return parseFloat(expr.replace('#d' ,''));
	} else {
	    return parseInt(expr.replace('#d' ,''));
	}
    } else if (this.isHex(expr)) {
	return parseInt(expr.replace('#' ,'0') ,16);
    } else if (this.isOctal(expr)) {
	return parseInt(expr.replace('#o' ,'') ,8);
    } else {
	throw new TypeError(expr + " is not a number");
    }
}

Utils.getString = function(expr) {
    if (this.isString(expr)) {
	return new JSString(new RegExp(Tokens.STRING).exec(expr)[1]);
    } else {
	throw new TypeError(expr + " is not a string");
    }
}

Utils.inspect = function(object) {
    try {
      if (Utils.isUndefined(object)) return 'undefined';
      if (object === null) return 'null';
      return object.inspect ? object.inspect() : String(object);
    } catch (e) {
      if (e instanceof RangeError) return '...';
      throw e;
    }
}

Utils.isAtom = function(expr) {
    return !Utils.isArray(expr);
}

Utils.isNull = function(expr) {
    return Utils.isArray(expr) && expr.length == 0;
}

Utils.format = function(expr) {
    if (typeof expr == 'function') {
	return expr.name === undefined ? '#<procedure>' : expr.name;
    } else if (expr === true) {
	return '#t';
    } else if (expr === false) {
	return '#f';
    } else if (expr instanceof Promise) {
	return expr.toString();
    } else if (expr instanceof JSString) {
	return '"' + expr + '"';
    } else if (Utils.isArray(expr) && expr[0] instanceof Pair) {
	var cpy = expr.clone();
	for (var i = 0; i < cpy.length; i++) {
	    cpy[i] = this.format(cpy[i]);
	}
	return Utils.inspect(cpy).gsub('[\\[]','(').gsub(']',')').gsub(',','').gsub('\'','');
    } else if (Utils.isArray(expr)) {
	var str = '(';
	for (var i = 0 ;i < expr.length ;i++) {
	    str += (i > 0 ? ' ' : '') + this.format(expr[i]);
	}
	str += ')';
	return str;
    } else {
	return Utils.inspect(expr).gsub('[\\[]','(').gsub(']',')').gsub(',','').gsub('\'','');
    }
}

Utils.map = function(op ,args) {
    var res = [];
    for (var i = 0 ;i < args.length ;i++) {
	res.push(op(args[i]));
    }
    return res;
}

Utils.mapCmp = function(op ,args) {
    for (var i = 1 ;i < args.length ;i++) {
	if (op(this.car(args) ,args[i])) {
	    return false;
	}
    }
    return true;
}
    
Utils.mapOp = function(op ,initial ,args ,func) {
    var ans = this.getNumber(initial);
    if (!this.isNumber(ans))
	throw IllegalArgumentTypeError(func ,ans ,1);
    for (var i = 0 ;i < args.length ;i++) {
	if (!this.isNumber(args[i]))
	    throw IllegalArgumentTypeError(func ,args[i] ,i+1);
	ans = op(ans ,this.getNumber(args[i]));
    }
    return ans;
}

Utils.validateNumberArg = function(proc ,args) {
    if (args.length != 1) {
	throw IllegalArgumentCountError(proc ,'exactly' ,1 ,args.length);
    } else if (!Util.isNumber(args[0])) {
	throw IllegalArgumentTypeError(proc ,args[0] ,1);
    } else {
	return true;
    }
}

/* --------------------- */

function Escape(cc ,args) {
    this.cc = cc === undefined ? new Function() : cc;
    this.args = args;
}

Escape.prototype.invoke = function() {
    return this.cc.apply(undefined, this.args);
}

/* --------------------- */

    
function Promise(e ,env) {
    
    if (Promise.instances === undefined)
	{ Promise.instances = 0; }
    
    this.promise = e;
    
    this.env = env;
    this.memoized = false;
    this.id = ++Promise.instances;

}

Promise.prototype.toString = function() {
    return '#[promise ' + this.id + ']';
}

Promise.prototype.force = function() {
    if (!this.memoized) {
	this.promise = jscm_eval(this.promise, this.env);
	this.memoized = true;
    }
    
    return this.promise;
}

/* ------------------ */

function Pair(car ,cdr ,parens) {
    this.car = car;
    this.cdr = cdr;
    this.parens = parens === undefined ? true : parens;
}

Pair.prototype.isEmpty = function() {
    return this.car === undefined && this.cdr === undefined;
}

Pair.prototype.isNullTerminated = function() {
    if (Utils.isNull(this.cdr)) {
	return true;
    } else if (Utils.isArray(this.cdr) && this.cdr.length == 1 &&
	       this.cdr[0] instanceof Pair) {
	return this.cdr[0].isNullTerminated();
    } else {
	return false;
    }
}

Pair.prototype.toStringList = function() {
    return Utils.format(this.car) + (Utils.isNull(this.cdr) ? '' : ' ' +
				    Utils.format(this.cdr[0]));
}	

Pair.prototype.toString = function() {
    if (this.isNullTerminated()) {
	return this.toStringList();
    }
    
    return 
    (this.parens ? '(' : '') + Utils.format(this.car) 
    + ' . ' + Utils.format(this.cdr) + (this.parens ? ')' : '');
}

/* ----------------- */

function Box(obj) {
    this.obj = obj;
}

Box.prototype.unbox = function() {
    return this.obj;
}

Box.prototype.setbox = function(obj) {
    this.obj = obj;
}

/* ----------------- */

function Environment(parent) {
    this.table = new Hash();
    this.parent = parent;
}

Environment.prototype.lookup = function(name) {
    name = name.toLowerCase();
    if (this.table.get(name) === undefined) {
	if (this.parent === undefined) {
	    var reserved = ReservedSymbolTable.get(name);
	    if (reserved === undefined) {
		throw UnboundVariableError(name);
	    } else {
		return new Box(reserved);
	    }
	} else {
	    return this.parent.lookup(name);
	}
    } else {
	return this.table.get(name);
    }
}

Environment.prototype.extend = function(name ,value) {
    name = name.toLowerCase();
    this.table.set(name ,value);
}

Environment.prototype.multiExtend = function(names ,values) {
    for (var i = 0 ;i < names.length ;i++) {
	this.extend(names[i] ,values[i]);
    }
}

Environment.extension = function() {
    return new Environment(this);
}
 
/* --------------------- */

function History(capacity) {
    this.capacity = capacity === undefined ? 100 : capacity;
    this.history = [];
}

History.prototype.get = function(index) {
    return this.history[index];
}

History.prototype.push = function(line) {
    if (this.history.length >= this.capacity - 1) {
	this.history = this.history.slice(0, this.capacity - 1);
    }
    this.history.unshift(line);
}

History.prototype.size = function() {
    return this.history.length;
}

/* -------------------- */

Function.prototype.inheritsFrom = function( parentClassOrObject ){ 
    if ( parentClassOrObject.constructor == Function ) 
	{ 
	    //Normal Inheritance 
	    this.prototype = new parentClassOrObject;
	    this.prototype.constructor = this;
	    this.prototype.parent = parentClassOrObject.prototype;
	} 
    else 
	{ 
	    //Pure Virtual Inheritance 
	    this.prototype = parentClassOrObject;
	    this.prototype.constructor = this;
	    this.prototype.parent = parentClassOrObject;
	} 
    return this;
} 
/* -------------------- */




