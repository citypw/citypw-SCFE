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

var $break = { };

function HashTable(object) {
     this._table = Object.isHash(object) ? object.toObject() : Object.Clone(object);
}

HashTable.prototype.get = function(key) { 
    return this._table[key]; 
}
    
HashTable.prototype.set = function(key ,value) {
	this._table[key] = value;
	return this._table[key];
}

HashTable.prototype.inject = function(memo ,iterator ,context) {
    iterator = iterator.bind(context);
    this.each(function(value, index) {
	    memo = iterator(memo, value, index);
	});
    return memo;
}

HashTable.prototype.each = function(iterator ,context) {
    var index = 0;
    iterator = iterator.bind(context);
    try {
      this._each(function(value) {
        iterator(value, index++);
      });
    } catch (e) {
      if (e != $break) throw e;
    }
    return this;
}

/* ----------------------- */
function JSString(string) {
    this.string = string;
}

JSString.prototype.toString = function() {
    return this.string;
}

/* ----------------------- */
    
function createMatcher(regex) {
	return function(expr) {
	    return new RegExp(regex).test(expr);
	};
    }

var OR = '|';
var Utils = {
    _isIdentifier: createMatcher(Tokens.IDENTIFIER),
    isBinary: createMatcher(Tokens.BINARY),
    isDecimal: createMatcher(Tokens.DECIMAL),
    isOctal: createMatcher(Tokens.OCTAL),

    isNumber: createMatcher(Tokens.BINARY + OR + Tokens.DECIMAL +
			    OR + Tokens.HEX + OR + Tokens.OCTAL),
    isHex: createMatcher(Tokens.HEX),
    isString: createMatcher(Tokens.STRING),

    isIdentifier: function(expr) {
	return !this.isNumber(expr) && !this.isString(expr) &&
	this._isIdentifier(expr);
    },

    car: function(list) {
	return list[0];
    },

    cdr: function(list) {
	return list.slice(1);
    },
    
    cons: function(x ,y) {
	var tmp = [x].concat(y);
	return tmp;
    },

    getNumber: function(expr) {
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
    },

    getString: function(expr) {
	if (this.isString(expr)) {
	    return new JSString(new RegExp(Tokens.STRING).exec(expr)[1]);
	} else {
	    throw new TypeError(expr + " is not a string");
	}
    },


    isAtom: function(expr) {
	return !Object.isArray(expr);
    },

    isNull: function(expr) {
	return Object.isArray(expr) && expr.length == 0;
    },

    format: function(expr) {
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
	} else if (Object.isArray(expr) && expr[0] instanceof Pair) {
	    var cpy = expr.clone();
	    for (var i = 0; i < cpy.length; i++) {
		console.log(cpy[i]);
		cpy[i] = this.format(cpy[i]);
	    }
	    return Object.inspect(cpy).gsub('[\\[]','(').gsub(']',')').gsub(',','').gsub('\'','');
	} else if (Object.isArray(expr)) {
	    var str = '(';
	    for (var i = 0 ;i < expr.length ;i++) {
		str += (i > 0 ? ' ' : '') + this.format(expr[i]);
	    }
	    str += ')';
	    return str;
	} else {
	    return Object.inspect(expr).gsub('[\\[]','(').gsub(']',')').gsub(',','').gsub('\'','');
	}
    },

    map: function(op ,args) {
	var res = [];
	for (var i = 0 ;i < args.length ;i++) {
	    res.push(op(args[i]));
	}
	return res;
    },

    mapCmp: function(op ,args) {
	for (var i = 1 ;i < args.length ;i++) {
	    if (op(this.car(args) ,args[i])) {
		return false;
	    }
	}
	return true;
    },
    
    mapOp: function(op ,initial ,args ,func) {
	var ans = this.getNumber(initial);
	if (!this.isNumber(ans))
	    throw IllegalArgumentTypeError(func ,ans ,1);
	for (var i = 0 ;i < args.length ;i++) {
	    if (!this.isNumber(args[i]))
		throw IllegalArgumentTypeError(func ,args[i] ,i+1);
	    ans = op(ans ,this.getNumber(args[i]));
	}
	return ans;
    },

    validateNumberArg: function(proc ,args) {
	if (args.length != 1) {
	    throw IllegalArgumentCountError(proc ,'exactly' ,1 ,args.length);
	} else if (!Utils.isNumber(args[0])) {
	    throw IllegalArgumentTypeError(proc ,args[0] ,1);
	} else {
	    return true;
	}
    },

    Serializers: {
	input: function(element, value) {
	    switch (element.type.toLowerCase()) {
	    case 'checkbox':
	    case 'radio':
	    return Utils.Serializers.inputSelector(element, value);
	    default:
	    return Utils.Serializers.textarea(element, value);
	    }
	},

	inputSelector: function(element, value) {
	    if (Object.isUndefined(value)) 
		return element.checked ? element.value : null;
	    else 
		element.checked = !!value;
	},

	textarea: function(element ,value) {
	    if (Object.isUndefined(value)) 
		return element.value;
	    else 
		element.value = value;
	},

	select: function(element ,index) {
	    if (Object.isUndefined(index))
		return this[element.type == 'select-one' ?
			    'selectOne' : 'selectMany'](element);
	    else {
		var opt, value, single = !Object.isArray(index);
		for (var i = 0, length = element.length; i < length; i++) {
		    opt = element.options[i];
		    value = this.optionValue(opt);
		    if (single) {
			if (value == index) {
			    opt.selected = true;
			    return;
			}
		    }
		    else opt.selected = index.include(value);
		}
	    }
	},

	selectOne: function(element) {
	    var index = element.selectedIndex;
	    return index >= 0 ? this.optionValue(element.options[index]) : null;
	},

	selectMany: function(element) {
	    var values, length = element.length;
	    if (!length) return null;

	    for (var i = 0, values = []; i < length; i++) {
		var opt = element.options[i];
		if (opt.selected) values.push(this.optionValue(opt));
	    }
	    return values;
	},

	optionValue: function(opt) {
	    // extend element because hasAttribute may not be native
	    return opt.text;
	}
    },
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
    this.parens = (parens === undefined ? false : parens);
}

Pair.prototype.isEmpty = function() {
    return this.car === undefined && this.cdr === undefined;
}

Pair.prototype.isNullTerminated = function() {
    if (Utils.isNull(this.cdr)) {
	return true;
    } else if (Object.isArray(this.cdr) && this.cdr.length == 1 &&
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

    return (this.parens ? '(' : '') + Utils.format(this.car) + ' . ' + Utils.format(this.cdr) + (this.parens ? ')' : ''); 
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

Function.prototype.inheritsFrom = function(parent){ 
    if ( parent.constructor == Function ) 
	{ 
	    //Normal Inheritance 
	    this.prototype = new parent;
	    this.prototype.constructor = this;
	    this.prototype.parent = parent.prototype;
	} 
    else 
	{ 
	    //Pure Virtual Inheritance 
	    this.prototype = parent;
	    this.prototype.constructor = this;
	    this.prototype.parent = parent;
	} 
    return this;
} 
/* -------------------- */

function $(id) {
    return document.getElementById(id);
}

function $F(element) {
    element = $(element);
    var method = element.tagName.toLowerCase();
    return Utils.Serializers[method](element);
}	

function $A(iterable) {
  if (!iterable) return [];
  if (iterable.toArray) return iterable.toArray();
  var length = iterable.length || 0, results = new Array(length);
  while (length--) results[length] = iterable[length];
  return results;
}

/* ---------String-------- */

String.interpret = function(value) {
    return value == null ? '' : String(value);
}

String.specialChar = {
    '\b': '\\b',
    '\t': '\\t',
    '\n': '\\n',
    '\f': '\\f',
    '\r': '\\r',
    '\\': '\\\\'
}

String.prototype.clone = function() {
    return this.valueOf();
}

String.prototype.strip = function() {
    return this.replace(/^\s+/, '').replace(/\s+$/, '');
}

String.prototype.gsub = function(pattern ,replacement) {
    var result = '', source = this, match;

    while (source.length > 0) {
      if (match = source.match(pattern)) {
        result += source.slice(0, match.index);
        result += String.interpret(replacement(match));
        source  = source.slice(match.index + match[0].length);
      } else {
        result += source, source = '';
      }
    }
    return result;
}

/* ---------String-------- */

/* ----------Array-------- */
Array.prototype.clone = function() {
    return this.slice(0);
}
/* ----------Array-------- */

Object.prototype.updateHTML = function(content) {
    this.innerHTML = content;
    return this;
}

function $A(iterable) {
  if (!iterable) return [];
  if (iterable.toArray) return iterable.toArray();
  var length = iterable.length || 0, results = new Array(length);
  while (length--) results[length] = iterable[length];
  return results;
}

Object.prototype._each = function(iterator) {
    for (var i = 0, length = this.length; i < length; i++)
	iterator(this[i]);
}

Object.isElement = function(object) {
    return object && object.nodeType == 1;
}

Object.isArray = function(object) {
    return object != undefined && object != null && typeof object == "object" &&
	'splice' in object && 'join' in object;
}

Object.isHash = function(object) {
    return object instanceof HashTable;
}

Object.isFunction = function(object) {
    return typeof object == "function";
}

Object.isString = function(object) {
    return typeof object == "string";
}

Object.isNumber = function(object) {
    return typeof object == "number";
}

Object.isUndefined = function(object) {
    return typeof object == "undefined";
}

Object.prototype.inject = function(memo ,iterator ,context) {
    iterator = iterator.bind(context);
    this.each(function(value, index) {
	    memo = iterator(memo, value, index);
	});
    return memo;
}

Object.prototype.bind = function() {
    if (arguments.length < 2 && Object.isUndefined(arguments[0]))
	return this;
    var args = $A(arguments) ,obj = args.shift();
    return this.apply(obj ,args.concat(args));
}


Object.prototype.each = function(iterator ,context) {
    var index = 0;
    iterator = iterator.bind(context);
    try {
	this._each(function(value) {
		iterator(value, index++);
	    });
    } catch (e) {
	if (e != $break) throw e;
    }
    return this;
}


Object.prototype.uniq = function(sorted) {
    return this.inject([], function(array ,value ,index) {
	    if (0 == index || (sorted ? array.last() != value : !array.include(value)))
		array.push(value);
	    return array;
	});
}

Object.prototype.toHTML = function(object) {
    return object && object.toHTML ? object.toHTML() : String.interpret(object);
}	

Object.extend = function(dest ,src) {
    for (var property in src)
	dest[property] = src[property];
    return dest;
}

Object.Clone = function(obj) {
    return Object.extend({} ,obj);
}

Object.inspect = function(object) {
    try {
	if (Object.isUndefined(object)) return 'undefined';
	if (object === null) return 'null';
	return object.inspect != undefined ? object.inspect(object) : String(object);
    } catch (e) {
	if (e instanceof RangeError) return '...';
	throw e;
    }
}

/* ---Element--- */
Element.prototype.hasClassName = function(element ,className) {
    if (!(element = $(element))) return;
    var elementClassName = element.className;
    return (elementClassName.length > 0 && (elementClassName == className ||
					    new RegExp("(^|\\s)" + className + "(\\s|$)").test(elementClassName)));
}

Element.prototype.addClassName = function(element ,className) {
    if (!(element = $(element))) return;
    if (!element.hasClassName(className))
	element.className += (element.className ? ' ' : '') + className;
    return element;
}


