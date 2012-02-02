/* Tiny JavaScript Scheme interpreter
 * This project is inspired by js-scheme - http://code.google.com/p/js-scheme/
 */

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

var TJSS = {
    author: 'Nala Ginrut',
    version: '0.0.1',
    date: '1 Feb 2012'
};

var Document = {
    CONSOLE: 'console',
    INPUT: 'input',
    PREFIX: 'prefix',
    INTRO: 'intro',
    KEY_DOWN: 40,
    KEY_UP: 38
};

var JSCMLibs = new HashTable();

function JSCMLib(name) {
    JSCMLibs.set(name ,this);
}

function jscm_registerLib(name ,lib) {
    JSCMLibs.set(name ,lib);
}

function Lexer(expr) {
    if (expr === undefined)
	return this.constructor;
    
    var tokens = [];
    var open = 0;

    this.nextToken = function(expr) {
	if (expr[0] == Tokens.L_PAREN || expr[0] == Tokens.R_PAREN ||
	    expr[0] == Tokens.SINGLE_QUOTE) {
	    return expr[0];
	} else if (Utils.isString(expr)) {
	    return '"' + Utils.getString(expr) + '"';
	} else if (expr[0] == Tokens.SEMI_COLON) {
	    var comment = '';
	    for (var i = 0 ;i < expr.length ;i++) {
		if (expr[i] == Tokens.NEWLINE) {
		    break;
		} else {
		    comment += expr[i];
		}
	    }
	    return comment;
	} else {
	    var sexpr = '';
	    for (var i = 0 ;i < expr.length ;i++) {
		if (expr[i] == Tokens.L_PAREN || expr[i] == Tokens.R_PAREN ||
		    expr[i] == Tokens.SPACE || expr[i] == Tokens.NEWLINE) {
		    break;
		} else {
		    sexpr += expr[i];
		}
	    }
	    return sexpr;
	}
    }

    for (var i = 0; i < expr.length; i++) {
	if (expr[i] != Tokens.SPACE && expr[i] != Tokens.NEWLINE) {
	    var token = this.nextToken(expr.substring(i));
	    i += token.length - 1;
	    if (token.length != 0) {
		if (token == Tokens.L_PAREN) {
		    open++;
		} else if (token == Tokens.R_PAREN) {
		    open--;
		}
		if (token[0] != Tokens.SEMI_COLON) {
		    tokens.push(token);
		}
	    }
	}
    }
    if (open < 0) {
	throw ParseWarning("unbalanced parens");
    } else if (open > 0) {
	throw IgnorableParseWarning("unbalanced parens");
    } else {
	return tokens;
    }
}


function Parser() {
    this.lexer = new Lexer();
}

Parser.prototype.parse = function(expr) {
    var tokens = this.lexer.tokenize(expr);
    var stack = [];
    while (tokens.length > 0) {
	stack.push(this.nextSExpr(tokens));
    }
    if (stack.length == 0) {
	throw IgnorableParseWarning("empty");
    } else if (stack.length == 1) {
	return stack.pop();
    } else {
	throw ParseWarning("information overload!");
    }
}

Parser.prototype.nextSExpr = function(tokens) {
	    if (tokens.length == 0) {
		return [];
	    } else if (tokens[0] == Tokens.L_PAREN) {
		tokens.shift();
		return this.nextList(tokens);
	    } else if (tokens[0] == Tokens.SINGLE_QUOTE) {
		tokens.shift();
		return [Tokens.QUOTE, this.nextSExpr(tokens)];
	    } else {
		return tokens.shift();
	    }
	}

Parser.prototype.nextList = function(tokens) {
    var list = [];
    var next = this.nextSExpr(tokens);
    var pair = new Pair(undefined ,undefined ,false);
    var pp = (next instanceof Pair);

    if (next == Tokens.DOT) {
	throw ParseWarning("Ill-formed dotted list; car is undefined.");
    }

    while (tokens.length > 0 && next != Tokens.R_PAREN) {
	if (next != Tokens.DOT) {
	    list.push(next);
	}

	next = this.nextSExpr(tokens);
	
	if (pp && next != Tokens.R_PAREN) {
	    /* if the previous s-expression was a pair, it must either be nested
	     * with parens or be the last s-expression in the list
	     */
	    throw ParseWarning("Ill-formed dotted list.");
	}
	
	if (next == Tokens.DOT) {
	    if (pair.isEmpty()) {
		pair.car = list.pop();
		if (pair.car === undefined) {
		    throw new ParseWarning("Ill-formed dotted list; car is undefined.");
		} else if (pair.car instanceof Pair) {
		    throw ParseWarning("Ill-formed dotted list; car is a Pair.");
		}
	    } else {
		throw ParseWarning("Ill-formed dotted list.");
	    }
	} else if (pair.car && pair.cdr === undefined) {
	    pair.cdr = next;
	    if (pair.cdr === undefined) {
		throw ParseWarning("Ill-formed dotted list; cdr is undefined.");
	    }
	    next = pair;
	} else if (!pair.isEmpty() && next != Tokens.R_PAREN) {
	    throw ParseWarning("Ill-formed dotted list.");
	}
    }
    
    return list;
}

var Actions = {
    APPLICATION: function(expr, env) {
	var proc = jscm_eval(Utils.car(expr), env);
	if (proc instanceof SpecialForm) {
	    return proc.apply(expr, env);
	} else {
	    if (proc instanceof Builtin) {
		proc = proc.apply;
	    }
	    var args = jscm_evlis(Utils.cdr(expr), env);
	    if (typeof proc != 'function') {
		throw new JSError('The object ' + Utils.format(proc) +
				  ' is not applicable.', 'Type');
	    }
	    return proc(args);
	}
    },
    CONST: function(expr, env) {
	var exprl = expr.toLowerCase();
	if (Utils.isNumber(exprl)) {
	    return Utils.getNumber(exprl);
	} else if (Utils.isString(expr)) {
	    return Utils.getString(expr);
	} else {
	    throw new JSError(expr + " not recognized as CONST", "Value");
	}
    },
    IDENTIFIER: function(expr, env) {
	return env.lookup(expr.toLowerCase()).unbox();
    }
};

function Builtin(name ,apply ,doc ,argdoc) {
    Builtin.instances = Builtin.instances === undefined ? 0 : Builtin.instances;
    Builtin.instances++;
    this.name = name;
    this.apply = apply;
    this.doc = doc;
    this.argdoc = argdoc == undefined ? '' : argdoc;
}

Builtin.prototype.toString = function() {
    return '#<builtin-procedure-' + this.name + '>';
}

/* ---------------- */
function SpecialForm(name ,apply ,doc ,argdoc) {
    this.constructor.call(name ,apply ,doc ,argdoc);
}

/* Inherit from Builtin */
SpecialForm.inheritsFrom(Builtin);
	
SpecialForm.prototype.toString = function() {
    return '#<special-form:' + this.name + '>';
}
/* ---------------- */

var ReservedSymbolTable = new HashTable({
	'#t': true,
	'#f': false,
	'abs': new Builtin('abs' ,function(args) {
		if (args.length != 1)
		    throw IllegalArgumentCountError('abs', 'exactly', 1, args.length);
		if (!isNumber(args[0]))
		    throw IllegalArgumentTypeError('abs', args[0], 1);
		return Math.abs(args[0]);
	    }, 'Returns the absolute value of <em>number</em>.', 'number'),
	'acos': new Builtin('acos' ,function(args) {
		Utils.validateNumberArg('acos', args);
		return Math.acos(args[0]);
	    }, 'Returns the arc cosine (in radians) of <em>z</em>.', 'z'),
	'and': new Builtin('and' ,function(args) {
		var val = true;
		for (var i = 0; i < args.length; i++) {
		    val = args[i];
		    if (val == false)
			break;
		}
		return val;
	    }, '<p>The logical <em>and</em> returns the last element in its argument ' +
	    'list if no argument evaluates to #f.  Otherwise, returns #f.</p>' +
	    '<p>Note: <b>#f</b> is the <u>only</u> false value in conditional ' +
	    'expressions.</p>', 'obj<sub>1</sub> . obj<sub>n</sub>'),
	'append': new Builtin('append' ,function(args) {
		var res = [];
		if (args.length == 1) {
		    res = args[0];
		} else {
		    for (var i = 0 ;i < args.length ;i++) {
			if (Utils.isAtom(args[i]) && i < args.length - 1) {
			    throw IllegalArgumentTypeError('append' ,args[i] ,i + 1);
			} else if (Utils.isAtom(args[i])) {
			    res.push(new Pair(res.pop() ,args[i], false));
			} else {
			    for (var j = 0 ;j < args[i].length ;j++) {
				res.push(args[i][j]);
			    }
			}
		    }
		}
		return res;
	    }, '<p>Returns a list consisting of the elements of the first ' +
	    '<em>list</em> followed by the elements of the other <em>list</em>s.</p>' +
	    '<p>The last argument may be any object; an improper list results if the' +
	    ' last argument is not a proper list.</p>',
	    'list<sub>1</sub> . obj<sub>n</sub>'),
	'apply': new Builtin('apply', function(args) {
		if (args.length == 0 || args.length > 2)
		    throw IllegalArgumentCountError('apply', '', 'one or two', args.length);
		var proc = args[0];
		if (proc instanceof Builtin)
		    proc = proc.apply;
		return proc(args[1]);
	    }, 'Applies <em>proc</em> to elements of the list <em>args</em>.',
	    'proc args'),
	'asin': new Builtin('asin', function(args) {
		Utils.validateNumberArg('asin', args);
		return Math.asin(args[0]);
	    }, 'Returns the arc sin (in radians) of <em>z</em>.', 'z'),
	'atan': new Builtin('atan', function(args) {
		Utils.validateNumberArg('atan' ,args);
		return Math.atan(args[0]);
	    }, 'Returns the arc tangent (in radians) of <em>z</em>.', 'z'),
	'begin': new SpecialForm('begin' ,function(e, env) {
		if (e.length == 1) {
		    return undefined;
		} else {
		    return jscm_eval(Utils.cons(Utils.cons(Tokens.LAMBDA,
							 Utils.cons([], Utils.cdr(e))),
					       []) ,env);
		}
	    }, 'The expressions are evaluated from left to rigt, and the value of the ' +
	    'last expression is returned.',
	    'expression<sub>1</sub> . expression<sub>n</sub>'),
	'call/cc':
	new Builtin('call/cc', function(args) {
		if (args.length != 1) {
		    throw IllegalArgumentCountError('call/cc',
						    'exactly' ,1 ,args.length);
		}
		var proc = args[0];
		if (proc instanceof Builtin) {
		    proc = proc.apply;
		}
		if (typeof proc != 'function') {
		    throw IllegalArgumentTypeError('call/cc' ,proc ,1);
		}
		var ESCAPE = new Object();
		try {
		    return proc([function(value) {
				ESCAPE.value = value[0];
				throw ESCAPE;
			    }]);
		} catch (e) {
		    if (e === ESCAPE) {
			return ESCAPE.value;
		    } else {
			throw e;
		    }
		}
	    }, '<p>Calls <em>proc</em> with the current continuation.</p>' +
	    '<p>Note that the continuation may only be used as an escape procedure; ' +
	    'inwards continuations are not supported.</p>', 'proc'),
	'car': new Builtin('car', function(args) {
		var ans = undefined;
		if (args.length != 1) {
		    throw IllegalArgumentCountError('car', 'exactly', 1, args.length);
		} else if (args[0] instanceof Pair) {
		    ans = args[0].car;
		} else if (Utils.isAtom(args[0]) || Utils.isNull(args[0])) {
		    throw IllegalArgumentTypeError('car', args[0], 1);
		} else if (args[0][0] instanceof Pair) {
		    ans = args[0][0].car;
		} else {
		    ans = args[0][0];
		}
		return ans;
	    }, '<p>Returns the contents of the car field of <em>pair</em>.</p>' +
	    '<p>Note: it is an error to take the car of the empty list.</p>', 'pair'),
	'case': new SpecialForm('case', function(e, env) {
		if (e.length < 3) {
		    throw IllegalArgumentCountError('case', 'at least', 2, e.length - 1);
		}
		var key = jscm_eval(e[1], env);
		var lines = Utils.cdr(Utils.cdr(e));
		for (var i = 0; i < lines.length; i++) {
		    var keyset = lines[i][0];
		    var expr = [Utils.cons(Tokens.LAMBDA, Utils.cons([], Utils.cdr(lines[i])))];
		    if (keyset.toString().toLowerCase() === 'else') {
			return jscm_eval(expr, env);
		    }
		    for (var j = 0; j < keyset.length; j++) {
			if (keyset[j] == key) {
			    return jscm_eval(expr, env);
			}
		    }
		}
		return undefined;
	    },'<p>Each <em>clause</em> should be of the form: <br /> ' +
	    '((datum<sub>1</sub> . datum<sub>n</sub>) expression<em>(s)</em>)</p>' +
	    '<p>A <code>case</code> expression is evaluated as follows. ' +
	    '<em>Key</em> is evaluated and its result is compared against each ' +
	    '<em>datum</em>.  If the result of evaluating <em>key</em> is equivalent ' +
	    ' (in the sense of <code>eqv?</code>) to a <em>datum</em>, then the ' +
	    'expressions in the corresponding <em>clause</em> are evaluated from ' +
	    'left to right and the result of the last expression in the ' +
	    '<em>clause</em> is returned as the result of the <code>case</code> ' +
	    'expression.  <code>Else</code> may be used as a <em>datum</em>, as in ' +
	    '<code>cond</code>.</p>', 'key clause<sub>1</sub> . clause<sub>n</sub>'),
	'cdr': new Builtin('cdr', function(args) {
		var ans = undefined;
		if (args.length != 1) {
		    throw IllegalArgumentCountError('cdr', 'exactly', 1, args.length);
		} else if (args[0] instanceof Pair) {
		    ans = args[0].cdr;
		} else if (Utils.isAtom(args[0]) || Utils.isNull(args[0])) {
		    throw IllegalArgumentTypeError('cdr', args[0], 1);
		} else if (args[0][0] instanceof Pair) {
		    ans = args[0][0].cdr;
		} else {
		    ans = Utils.cdr(args[0]);
		}
		return ans;
	    },'<p>Returns the contents of the cdr field of <em>pair</em>.</p>' +
	    '<p>Note: it is an error to take the cdr of the empty list.</p>', 'pair'),
	'clear-console': new Builtin('clear-console', function(args) {
		var divs = $$('#' + Document.CONSOLE + ' > div');
		for (var i = 0; i < divs.length; i++) {
		    if (!divs[i].hasClassName(Document.INTRO)) {
			divs[i].remove();
		    }
		}
		$(Document.INPUT).value = '';
		$(Document.INPUT).focus();
		throw new Escape();
	    }, 'Clears the console display area.'),
	'cond': new SpecialForm('cond', function(e, env) {
		var lines = Utils.cdr(e);
		for (var i = 0; i < lines.length; i++) {
		    if (jscm_eval(lines[i][0], env)) {
			return jscm_eval(lines[i][1], env);
		    }
		}
		return undefined;
	    },'<p>Each <em>clause</em> is a pair where the car is a <em>test</em> ' +
	    'expression, and the cdr is the value of the cond expresion if the test ' +
	    'evaluates to a true value.</p><p>The value of the first clause whose ' +
	    'test is true is the value of the cond expression.</p>',
	    'clause<sub>1</sub> clause<sub>2</sub> . clause<sub>n</sub>'),
	'cons': new Builtin('cons', function(args) {
		if (args.length != 2)
		    throw IllegalArgumentCountError('cons', 'exactly', 2, args.length);
		if (Utils.isAtom(args[1])) {
		    return new Pair(args[0], args[1]);
		} else {
		    return Utils.cons(args[0], args[1]);
		}
	    }, 'Returns a newly allocated pair whose car is <em>obj<sub>1</sub></em> ' +
	    'and whose cdr is <em>obj<sub>2</sub></em>.',
	    'obj<sub>1</sub> obj<sub>2</sub>'),
	'cos': new Builtin('cos', function(args) {
		Utils.validateNumberArg('cos', args);
		return Math.cos(args[0]);
	    }, 'Returns the cosine (in radians) of <em>z</em>.', 'z'),
	'define': new SpecialForm('define' ,function(e ,env) {
		if (e.length < 2) {
		    throw new JSError(Utils.format(e) ,"Ill-formed special form" ,false);
		}
		var name = e[1];
		if (Utils.isAtom(name)) {
		    name = name.toLowerCase();
		    if (!Utils.isIdentifier(name)) {
			throw new JSWarning(name + ' may not be defined.');
		    } else if (ReservedSymbolTable.get(name) != undefined) {
			if (e.length == 2) {
			    ReservedSymbolTable.set(name, 0);
			    return name;
			} else {
			    var val = jscm_eval(e[2], env);
			    ReservedSymbolTable.set(name, val);
			    return name;
			}
		    } else {
			if (e.length == 2 || e.length == 3) {
			    if (e.length == 2) {
				env.extend(name, new Box(0));
				return name;
			    } else {
				var val = jscm_eval(e[2], env);
				env.extend(name, new Box(val));
				return name;
			    }
			} else {
			    throw new JSError(Utils.format(e), "Ill-formed special form", false);
			}
		    }
		} else if (!Utils.isNull(name)) {
		    name = e[1][0].toLowerCase();
		    if (!Utils.isIdentifier(name)) {
			throw new JSWarning(name + ' may not be defined.');
		    } else {
			var rhs = Utils.cons(Tokens.LAMBDA,
					    Utils.cons(Utils.cdr(Utils.car(Utils.cdr(e))),
						      Utils.cdr(Utils.cdr(e))));
			if (ReservedSymbolTable.get(name) != undefined) {
			    var val = jscm_eval(rhs ,env);
			    ReservedSymbolTable.set(name ,val);
			    return name;
			} else {
			    var val = jscm_eval(rhs ,env);
			    env.extend(name, new Box(val));
			    return name;
			}
		    }
		} else {
		    throw new JSError("I don't know what to do with that.", 'Syntax');
		}
	    }, '<p>Defines a variable in the current environment that refers to the ' +
	    'value of <em>expression</em>. The value of <em>expression</em> is not ' +
	    'evaluated during the definition.  If no <em>expression</em> is present, ' +
	    '0 will be used.</p><p>The alternative form of define may also be used to' +
	    ' define procedures. This form is:</p><p>(define (proc [formals]) body)' +
	    '<p></p>and is equivalent to</p><p>(define proc (lambda ([formals]) body))',
	    'variable [expression]', 'variable [expression]'),
	'delay': new SpecialForm('delay', function(e, env) {
		if (e.length == 1)
		    throw new JSError(Utils.format(e), 'Ill-formed special form', false);
		return new Promise(e[1], env);
	    }, 'Returns a <em>promise</em> which at some point in the future may be ' +
	    'asked (by the <strong>force</strong> procedure) to evaluate ' +
	    '<em>expression</em>, and deliver the resulting value.', 'expression'),
	'display': new Builtin('display' ,function(args) {
		if (args.length != 1) {
		    throw IllegalArgumentCountError('display' ,'exactly' ,1 ,args.length);
		} else {
		    var fmt = Utils.format(args[0]);
		    if (Utils.isString(fmt)) {
			jscm_printDisplay(Utils.getString(fmt));
		    } else {
			jscm_printDisplay(fmt);
		    }
		}
	    }, 'Prints <em>obj</em>.', 'obj'),
	'display-built-ins': new Builtin('display-built-ins' ,function(args) {
		throw new Escape(jscm_printBuiltinsHelp ,[args]);
	    }, 'Displays the list of built-in procedures.'),
	'e': Math.E,
	'else': true,
	'eval': new SpecialForm('eval', function(e, env) {
		if (e.length != 2)
		    throw IllegalArgumentCountError('eval' ,'exactly' ,1 ,e.length - 1);
		var args = jscm_eval(e[1], env);
		if (Utils.isAtom(args)) {
		    return jscm_eval(REPL.parser.parse(args));
		} else if (!Utils.isNull(args)) {
		    return jscm_eval(args ,env);
		} else {
		    throw IllegalArgumentTypeError('eval' ,args ,1);
		}
	    }, '<p>Evaluates <em>expression</em> in the current environment.</p><p>' +
	    '<em>expression</em> can either be a string Scheme ' +
	    'expression, or a quoted external representation of a Scheme expression.' +
	    '</p><p>For example,</p><p>(eval \'(+ 2 2)) => 4<br />(eval "(+ 2 2)") =>' +
	    ' 4</p>', 'expression'),
	'even?': new Builtin('even?', function(args) {
		Utils.validateNumberArg('even?' ,args);
		return args[0] % 2 == 0;
	    }, 'Returns #t if <em>n</em> is even, and #f otherwise.' ,'n'),
	'eq?': new Builtin('eq?' ,function(args) {
		if (args.length != 2)
		    throw IllegalArgumentCountError('eq?' ,'exactly' ,2 ,args.length);
		if (Utils.isNull(args[0])) {
		    return Utils.isNull(args[1]);
		} else if (Utils.isNull(args[1])) {
		    return false;
		} else {
		    return args[0] == args[1];
		}
	    }, '<p>Returns #t if <em>obj<sub>1</sub></em> is "equal" to ' +
	    '<em>obj<sub>2</sub></em>.</p><p>This is currently determined using the' +
	    ' JavaScript <strong>===</strong> operator.</p>',
	    'obj<sub>1</sub> obj<sub>2</sub>'),
	'equal?': new Builtin('equal?' ,function(args) {
		if (args.length != 2) {
		    throw IllegalArgumentCountError('equal?' ,'exactly' ,2 ,args.length);
		}
		var equal = function(obj1 ,obj2) {
		    if (obj1 instanceof JSString) {
			obj1 = obj1.string;
		    } else if (obj2 instanceof JSString) {
			obj2 = obj2.string;
		    }
		    if (Utils.isNull(obj1) && Utils.isNull(obj2)) {
			return true;
		    } else if (Utils.isAtom(obj1) && Utils.isAtom(obj2)) {
			return obj1 == obj2;
		    } else {
			if (obj1.length == obj2.length) {
			    for (var i = 0; i < obj1.length; i++) {
				if (!equal(obj1[i] ,obj2[i])) {
				    return false;
				}
			    }
			    return true;
			} else {
			    return false;
			}
		    }
		};
		return equal(args[0] ,args[1]);
	    }, 'Returns #t if <em>obj<sub1</sub></em> is equal to <em>obj<sub>2</sub>' +
	    '</em>.  If the objects are lists, they will be compared recursively. ' +
	    'As a rule of thumb, if the objects display the same, they are probably ' +
	    '"equal."', 'obj<sub>1</sub> obj<sub>2</sub>'),
	'eqv?': new Builtin('eqv?' ,function(args) {
		if (args.length != 2)
		    throw IllegalArgumentCountError('eqv?' ,'exactly' ,2 ,args.length);
		return args[0] === args[1] || Utils.isNull(args[0]) && Utils.isNull(args[1]);
	    }, '<p>Currently returns the exact same value as <code>(eq? <em>obj<sub>1' +
	    '</sub></em> <em>obj<sub>2</sub></em>)</code></p>',
	    'obj<sub>1</sub> obj<sub>2</sub>'),
	'expt': new Builtin('expt' ,function(args) {
		if (args.length != 2)
		    throw IllegalArgumentCountError('expt' ,'exactly' ,1 ,args.length);
		if (!Utils.isNumber(args[0]))
		    throw IllegalArgumentTypeError('expt' ,args[0] ,1);
		if (!Utils.isNumber(args[1]))
		    throw IllegalArgumentTypeError('expt' ,args[1] ,2);
		return Math.pow(args[0], args[1]);
	    }, 'Returns <em>a</em> to the power of <em>b</em>.' ,'a b'),
	'floor': new Builtin('floor' ,function(args) {
		Utils.validateNumberArg('floor' ,args);
		return Math.floor(args[0]);
	    }, 'Returns the largest integer less than or equal to <em>z</em>.' ,'z'),
	'force': new Builtin('force', function(args) {
		if (args.length != 1)
		    throw IllegalArgumentCountError('force' ,'exactly' ,1 ,args.length);
		if (!(args[0] instanceof Promise))
		    throw IllegalArgumentTypeError('force' ,args[0] ,1);
		return args[0].force();
	    }, 'Forces the value of <em>promise</em>.  If no value has been ' +
	    'computed for the promise, then that value is computed, memoized, and ' +
	    'returned.', 'promise'),
	'for-each': new Builtin('for-each', function(args) {
		if (args.length < 2)
		    throw IllegalArgumentCountError('for-each' ,'at least' ,2 ,args.length);
		var proc = Utils.car(args);
		if (proc instanceof Builtin) {
		    proc = proc.apply;
		}
		if (typeof proc != 'function') {
		    throw IllegalArgumentTypeError('for-each' ,proc ,1);
		}
		var lists = Utils.cdr(args);
		for (var i = 1; i < lists.length; i++) {
		    if (lists[i].length != lists[0].length)
			throw IllegalArgumentError("all of the lists must be the same length");
		}
		for (i = 0; i < lists[0].length; i++) {
		    var pargs = [];
		    for (var j = 0; j < lists.length; j++) {
			pargs.push(lists[j][i]);
		    }
		    proc.apply(this ,[pargs]);
		}
		return undefined;
	    }, '<p>Applies <em>proc</em> element-wise to the elements of the ' +
	    '<em>list</em>s and returns a list of the results, in order.</p>' +
	    '<p><em>Proc</em> must be a function of as many arguments as there are ' +
	    'lists specified.</p>', 'proc list<sub>1</sub> . list<sub>n</sub>'),
	'help': new Builtin('help' ,function(args) {
		throw new Escape(jscm_printHelp ,[args]);
	    }, 'Displays help information for JS-SCHEME.<p><em>Obj</em> may be an actual'+
	    ' function object, or a string for the name of a library to lookup.</p>',
	    '[obj]'),
	'if': new SpecialForm('if' ,function(e ,env) {
		var args = Utils.cdr(e);
		if (jscm_eval(args[0] ,env)) {
		    return jscm_eval(args[1] ,env);
		} else if (args.length < 3) {
		    return undefined;
		} else {
		    return jscm_eval(args[2] ,env);
		}
	    }, 'An <strong>if</strong> expression ' ,'test consequent [alternate]'),
	'lambda': new SpecialForm('lambda' ,function(e ,env) {
		if (e.length < 3) {
		    throw new JSError(Utils.format(e) ,"Ill-formed special form" ,false);
		}
		if (Utils.isAtom(e[1])) {
		    var proc = function(args) {
			env = env.extension();
			env.extend(e[1] ,new Box(args));
			return jscm_beglis(Utils.cdr(Utils.cdr(e)) ,env);
		    };
		    proc.name = '#[compound-procedure]';
		    return proc;
		} else if (Object.isArray(e[1])) {
		    if (e[1].length != e[1].uniq().length)
			throw new JSError(Utils.format(e), "Ill-formed special form" ,false);
		    var proc = function(args) {
			env = env.extension();
			if (e[1].length != args.length)
			    throw IllegalArgumentCountError('#[compound-procedure]' ,'exactly',
							    e[1].length ,args.length);
			var bargs = [];
			for (var i = 0; i < args.length; i++) {
			    bargs[i] = new Box(args[i]);
			}
			env.multiExtend(e[1] ,bargs);
			return jscm_beglis(Utils.cdr(Utils.cdr(e)) ,env);
		    };
		    proc.name = '#[compound-procedure]';
		    return proc;
		}
	    }, 'Evaluates to a procedure.  Currently, there are two possible forms for ' +
	    '&lt;formals&gt;:<ul style="margin-top:5px;"><li>(variable<sub>1' +
	    '</sub> ...) <p>The procedure takes'+
	    ' a fixed number of arguments.</p></li><li>variable<p>The procedure takes '+
	    'any number of arguments, stored in a list with the name <em>variable' +
	    '</em>.</p></li></ul>', '&lt;formals&gt; body'),
	'length': new Builtin('length' ,function(args) {
		if (args.length != 1)
		    throw IllegalArgumentCountError('length' ,'exactly' ,1 ,args.length);
		if (!Object.isArray(args[0]))
		    throw IllegalArgumentTypeError('length' ,args[0] ,1);
		return args[0].length;
	    }, 'Returns the length of <em>list</em>.', 'list'),
	'let': new SpecialForm('let' ,function(e ,env) {
		var expr = Utils.cons(Utils.cons(Tokens.LAMBDA,
					       Utils.cons(Utils.map(function(el) {
							   return Utils.car(el);
						       }, Utils.car(Utils.cdr(e))),
						   (Utils.cdr(Utils.cdr(e))))),
				     Utils.map(function(el) {
					     return (Utils.car(Utils.cdr(el)));
					 }, Utils.car(Utils.cdr(e))));
		return jscm_eval(expr ,env);
	    }, '<p><em>bindings</em> is a list of pairs where the form of the pair is: ' +
	    '</p><p>(<em>variable</em> <em>init</em>)</p><p>Each <em>init</em> is ' +
	    'evaluated in the current environment, in some unspecified order, and ' +
	    'bound to the corresponding <em>variable</em>.</p>' +
	    '<p><em>body</em> is a sequence of expressions to be evaluated; the ' +
	    'value of the last expression is the value of the let expression.</p>' +
	    '<p><em>body</em> is evaluated in an extended environment.</p>',
	    'bindings body'),
	'let*': new SpecialForm('let*' ,function(e ,env) {
		var help = function(e ,b) {
		    if (Utils.isNull(e)) {
			return [];
		    } else if (e.length == 1) {
			return Utils.cons(Utils.cons(Tokens.LAMBDA,
						   Utils.cons(Utils.cons(Utils.car(Utils.car(e)),
								       []),
							     b)),
					 Utils.cons(Utils.car(Utils.cdr(Utils.car(e))),
						   []));
		    } else {
			return Utils.cons(Utils.cons(Tokens.LAMBDA,
						     Utils.cons(Utils.cons(Utils.car(Utils.car(e)),
								       []),
							     Utils.cons(help(Utils.cdr(e), b),
								       []))),
					  Utils.cons(Utils.car(Utils.cdr(Utils.car(e))),
						   []));
		    }
		};
		var expr = help(Utils.car(Utils.cdr(e)) ,Utils.cdr(Utils.cdr(e)));
		return jscm_eval(expr ,env);
	    }, '<p><em>bindings</em> is a list of pairs where the form of the pair is: ' +
	    '</p><p>(<em>variable</em> <em>init</em>)</p><p>Each <em>init</em> is ' +
	    'evaluated sequentially from left to right, where each binding to the ' +
	    'left of the one being evaluated is visible to the one being evaluated, ' +
	    'and bound to the corresponding <em>variable</em>.</p>' +
	    '<p><em>body</em> is a sequence of expressions to be evaluated; the ' +
	    'value of the last expression is the value of the let expression.</p>' +
	    '<p><em>body</em> is evaluated in an extended environment.</p>',
	    'bindings body'),
	'letrec': new SpecialForm('letrec' ,function(e ,env) {
		var body = Utils.cdr(Utils.cdr(e));
		var col = function(li) {
		    body = Utils.cons(Utils.cons(Tokens.DEFINE,
					       Utils.cons(Utils.car(li),
							 Utils.cdr(li))),
				     body);
		};
		Utils.map(col ,Utils.car(Utils.cdr(e)));
		var lisp = Utils.cons(Utils.cons(Tokens.LAMBDA,
					       Utils.cons([] ,body)),
				     []);
		return jscm_eval(lisp ,env);
	    }, '<p><em>bindings</em> is a list of pairs where the form of the pair is: ' +
	    '</p><p>(<em>variable</em> <em>init</em>)</p><p>Each <em>init</em> is ' +
	    'bound to the corresponding <em>variable</em>, in some unspecified ' +
	    'order, where the region of each binding of a variable is the entire ' +
	    'letrec expression.</p>' +
	    '<p><em>body</em> is a sequence of expressions to be evaluated; the ' +
	    'value of the last expression is the value of the let expression.</p>' +
	    '<p><em>body</em> is evaluated in an extended environment.</p>',
	    'bindings body'),
	'list': new Builtin('list' ,function(args) {
		return args;
	    }, 'Returns a list made up of the arguments.',
	    'obj<sub>1</sub> . obj<sub>n</sub>'),
	'list?': new Builtin('list?' ,function(args) {
		if (args.length != 1) {
		    throw IllegalArgumentCountError('list?' ,'exactly' ,1 ,args.length);
		} else if (Utils.isAtom(args[0])) {
		    return false;
		} else if (Utils.isNull(args[0])) {
		    return true;
		} else {
		    var ans = true;
		    for (var i = 0; i < args[0].length; i++) {
			if (Utils.isAtom(args[0][i]) && (args[0][i] instanceof Pair) &&
			    !Utils.isNull(args[0][i].cdr)) {
			    ans = false;
			    break;
			}
			return ans;
		    }
		}
	    }, 'Returns #t if <em>obj</em> is a list, and returns #f otherwise.', 'obj'),
	'list-ref': new Builtin('list-ref' ,function(args) {
		if (args.length != 2) {
		    throw IllegalArgumentCountError('list-ref' ,'exactly' ,2 ,args.length);
		} else if (!Object.isArray(args[0])) {
		    throw IllegalArgumentTypeError('list-ref' ,args[0] ,1);
		} else if (!Utils.isNumber(args[1])) {
		    throw IllegalArgumentTypeError('list-ref' ,args[1] ,2);
		} else if (args[0] < 0) {
		    throw IllegalArgumentError('The object ' + args[1] + ', passed as an ' +
					       'argument to list-ref, is not an index integer.');
		} else if (args[1] >= args[0].length) {
		    throw IllegalArgumentError('The object ' + args[1] + ', passed as an ' +
					       'argument to list-ref, is not in the correct range.');
		} else {
		    return args[0][args[1]];
		}
	    }, 'Returns the <em>k</em>th element of <em>list</em>.  It is an error if ' +
	    '<em>list</em> has fewer than <em>k</em> elements.' ,'list k'),
	'list-tail': new Builtin('list-tail' ,function(args) {
		if (args.length != 2) {
		    throw IllegalArgumentCountError('list-tail' ,'exactly' ,2 ,args.length);
		} else if (!Object.isArray(args[0])) {
		    throw IllegalArgumentTypeError('list-tail' ,args[0] ,1);
		} else if (!Utils.isNumber(args[1])) {
		    throw IllegalArgumentTypeError('list-tail' ,args[1] ,2);
		} else if (args[1] < 0) {
		    throw IllegalArgumentError('The object ' + args[1] + ', passed as an ' +
					       'argument to list-tail, is not an index integer.');
		} else if (args[0].length < args[1]) {
		    throw IllegalArgumentError('The object ' + args[1] + ', passed as an ' +
					       'argument to list-tail, is not in the correct range.');
		} else {
		    return args[0].slice(args[1]);
		}
	    }, 'Returns the sublist of <em>list</em> obtained by omitting the first ' +
	    '<em>k</em> elements of <em>list</em> It is an error if <em>list</em> ' +
	    'has fewer than <em>k</em> elements.', 'list k'),
	'load': new SpecialForm('load' ,function(e ,env) {
		if (e.length != 2) {
		    throw IllegalArgumentCountError('load' ,'exactly' ,1 ,e.length - 1);
		} else {
		    var name = jscm_eval(e[1] ,env);
		    var lib = JSCMLibs.get(name);
		    if (lib === undefined) {
			throw new JSError('No library registered for: ' + name,
					  'IllegalArgument');
		    } else if (lib instanceof JSCMLib) {
			/* nothing to do, lib already initialized */
		    } else {
			try {
			    lib = new lib();
			    JSCMLibs.set(name ,lib);
			} catch (e) {
			    throw IllegalArgumentTypeError('load' ,lib ,1);
			}
		    }
		    lib.getProcedures().each(function (proc) {
			    env.extend(proc.key ,new Box(proc.value));
			});
		    return lib;
		}
	    }, '', 'lib'),
	'log': new Builtin('log' ,function(args) {
		Utils.validateNumberArg('log' ,args);
		return Math.log(args[0]) / Math.log(2);
	    }, 'Returns the natural logarithm (base 2) of <em>z</em>.' ,'z'),
	'map': new Builtin('map' ,function(args) {
		if (args.length < 2)
		    throw IllegalArgumentCountError('map' ,'at least' ,2 ,args.length);
		var proc = args[0];
		if (proc instanceof Builtin) {
		    proc = proc.apply;
		}
		if (typeof proc != 'function') {
		    throw IllegalArgumentTypeError('map' ,proc ,1);
		}
		var lists = Utils.cdr(args);
		for (var i = 1; i < lists.length; i++) {
		    if (lists[i].length != lists[0].length)
			throw IllegalArgumentError("all of the lists must be the same length");
		}
		var res = [];
		for (var j = 0; j < lists[0].length; j++) {
		    var pargs = [];
		    for (var k = 0; k < lists.length; k++) {
			pargs.push(lists[k][j]);
		    }
		    var val = proc.apply(this ,[pargs]);
		    res.push(val);
		}
		return res;
	    }, '<p>Applies <em>proc</em> element-wise to the elements of the ' +
	    '<em>list</em>s and returns a list of the results, in order.</p>' +
	    '<p><em>Proc</em> must be a function of as many arguments as there are ' +
	    'lists specified.</p>', 'proc list<sub>1</sub> . list<sub>n</sub>'),
	'modulo': new Builtin('modulo' ,function(args) {
		if (args.length != 2) {
		    throw IllegalArgumentCountError('modulo' ,'exactly' ,2 ,args.length);
		} else if (!Utils.isNumber(args[0])) {
		    throw IllegalArgumentTypeError('modulo' ,args[0] ,1);
		} else if (!Utils.isNumber(args[1])) {
		    throw IllegalArgumentTypeError('modulo' ,args[1] ,2);
		} else {
		    return args[0] % args[1];
		}
	    }, 'Returns <em>n<sub>1</sub></em> modulo <em>n<sub>2</sub></em>.',
	    'n<sub>1</sub> n<sub>2</sub>'),
	'newline': new Builtin('newline' ,function(args) {
		if (args.length != 0) {
		    throw IllegalArgumentCountError('newline' ,'exactly' ,0 ,args.length);
		} else {
		    REPL.newline();
		}
	    }, 'newline!'),
	'not': new Builtin('not', function(args) {
		if (args.length != 1)
		    throw IllegalArgumentCountError('not' ,'exactly' ,1 ,args.length);
		return args[0] == false;
	    },'<p><em>not</em> returns #t if <em>obj</em> is false, and returns #f ' +
	    'otherwise.</p><p>Note: <b>#f</b> is the <u>only</u> false value in ' +
	    'conditional expressions.</p>', 'obj'),
	'null?': new Builtin('null?' ,function(args) {
		if (args.length != 1)
		    throw IllegalArgumentCountError('null?', 'exactly', 1, args.length);
		return Utils.isNull(args[0]);
	    }, 'Returns #t if <em>obj</em> is the empty list, and returns #f otherwise.',
	    'obj'),
	'number?': new Builtin('number?' ,function(args) {
		if (args.length != 1)
		    throw IllegalArgumentCountError('number?' ,'exactly' ,1 ,args.length);
		return Utils.isNumber(args[0]);
	    }, 'Returns #t if <em>obj</em> is a number, and returns #f otherwise.',
	    'obj'),
	'odd?': new Builtin('odd?' ,function(args) {
		Utils.validateNumberArg('odd?' ,args);
		return args[0] % 2 != 0;
	    }, 'Returns #t if <em>n</em> is odd, and returns #f otherwise.', 'n'),
	'or': new Builtin('or' ,function(args) {
		for (var i = 0; i < args.length; i++) {
		    if (args[i]) {
			return args[i];
		    }
		}
		return false;
	    },'<p>The logical <em>or</em> returns the first element in its argument ' +
	    'list that doesn\'t evaluate to #f.  Otherwise, returns #f.</p>' +
	    '<p>Note: <b>#f</b> is the <u>only</u> false value in conditional ' +
	    'expressions.</p>', 'obj<sub>1</sub> . obj<sub>n</sub>'),
	'pair?': new Builtin('pair?' ,function(args) {
		if (args.length != 1)
		    throw IllegalArgumentCountError('pair?' ,'exactly' ,1 ,args.length);
		return !Utils.isNull(args[0]) && !Utils.isAtom(args[0]);
	    }, 'Returns #t if <em>obj</em> is a pair, and returns #f otherwise.', 'obj'),
	'pi': Math.PI,
	'procedure?': new Builtin('procedure?' ,function(args) {
		if (args.length != 1) {
		    throw IllegalArgumentCountError('procedure?', 'exactly', 1, args.length);
		}
		return Utils.isfunction(args[0]);
	    }, 'Returns #t if <em>obj</em> is a procedure.', 'obj'),
	'quote': new SpecialForm('quote' ,function(e ,env) {
		return function(args) {
		    if (args.length != 1)
			throw IllegalArgumentCountError('quote' ,'exactly' ,1 ,args.length);
		    return args[0];
		}(Utils.cdr(e));
	    }, '<p>Evaluates to <em>datum</em>.</p><p>The single-quote character ' +
	    '<strong>\'</strong> may also be used as an abbreviation, where ' +
	    '<strong>\'<em>datum</em></strong> is equivalent to <strong>(quote <em>' +
	    'datum</em></strong>)</p>', 'datum'),
	'random': new Builtin('random' ,function(args) {
		if (args.length != 0) {
		    throw IllegalArgumentCountError('random' ,'exactly' ,0 ,args.length);
		}
		return Math.random();
	    }, 'Returns a pseudo-random number in the range [0,1).'),
	'reverse': new Builtin('reverse' ,function(args) {
		if (args.length != 1)
		    throw IllegalArgumentCountError('reverse' ,'exactly' ,1 ,args.length);
		if (!Object.isArray(args[0]))
		    throw IllegalArgumentTypeError('reverse' ,args[0] ,1);
		return args[0].reverse(false);
	    }, 'Returns a newly allocated list containing the elements of ' +
	    '<em>list</em> in reverse order.' ,'list'),
	'set!': new SpecialForm('set!' ,function(e ,env) {
		var oldBox = undefined;
		var name = e[1].toLowerCase();
		var old = ReservedSymbolTable.get(name);
		if (old === undefined) {
		    oldBox = env.lookup(name);
		    old = oldBox.unbox();
		}
		var rhs = Utils.isNull(Utils.cdr(Utils.cdr(e))) ? 0 : e[2];
		var val = jscm_eval(rhs ,env);
		if (oldBox === undefined) {
		    ReservedSymbolTable.set(name ,val);
		} else {
		    oldBox.setbox(val);
		}
		return old;
	    }, 'Similar to <strong>define</strong>, except that <em>variable</em> must ' +
	    'already be in the environment. If no <em>expression</em> is present, ' +
	    '0 is used. Returns the original value that <em>variable</em> referred to.',
	    'variable [expression]'),
	'sin': new Builtin('sin' ,function(args) {
		Utils.validateNumberArg('sin' ,args);
		return Math.sin(args[0]);
	    }, 'Returns the sine (in radians) of <em>z</em>.' ,'z'),
	'sqrt': new Builtin('sqrt' ,function(args) {
		Utils.validateNumberArg('sqrt' ,args);
		return Math.sqrt(args[0]);
	    }, 'Returns the square root of <em>z</em>, or <code>NaN</code> if ' +
	    '<em>z</em> is less than 0.' ,'z'),
	'tan': new Builtin('tan' ,function(args) {
		Utils.validateNumberArg('tan' ,args);
		return Math.tan(args[0]);
	    }, 'Returns the tangent (in radians) of <em>z</em>.' ,'z'),
	'zero?': new Builtin('zero?' ,function(args) {
		Utils.validateNumberArg('zero?' ,args);
		return args[0] === 0;
	    }, 'Returns #t if <em>number</em> is 0, and returns #f otherwise.', 'number'),
	'=': new Builtin('=', function(args) {
		return Utils.mapCmp(function(lhs ,rhs) { return lhs != rhs; }, args);
	    }, 'Returns #t if every argument is "equal," and returns #f otherwise. ' +
	    'Equality is determined using the JavaScript <strong>==</strong> operator.',
	    'obj<sub>1</sub> . obj<sub>n</sub>'),
	'<': new Builtin('<', function(args) {
		return Utils.mapCmp(function(lhs ,rhs) { return lhs >= rhs; }, args);
	    }, 'Returns #t if the first argument is less than every other argument, and' +
	    ' returns #f otherwise.', 'number<sub>1</sub> . number<sub>n</sub>'),
	'>': new Builtin('>', function(args) {
		return Utils.mapCmp(function(lhs ,rhs) { return lhs <= rhs; }, args);
	    }, 'Returns #t if the first argument is greater than every other argument, ' +
	    'and returns #f otherwise.', 'number<sub>1</sub> . number<sub>n</sub>'),
	'<=': new Builtin('<=', function(args) {
		return Utils.mapCmp(function(lhs ,rhs) { return lhs > rhs; }, args);
	    }, 'Returns #t if the first argument is less than or equal to every other ' +
	    'argument, and returns #f otherwise.', 'number<sub>1</sub> . number<sub>' +
	    'n</sub>'),
	'>=': new Builtin('>=', function(args) {
		return Utils.mapCmp(function(lhs ,rhs) { return lhs < rhs; }, args);
	    }, 'Returns #t if the first argument is greater than or equal to every ' +
	    'other argument, and returns #f otherwise.',
	    'number<sub>1</sub> . number<sub>n</sub>'),
	'+': new Builtin('+', function(args) {
		return Utils.mapOp(function(lhs ,rhs) { return lhs + rhs; }, 0, args,'+');
	    }, 'Returns the sum of the arguments.',
	    'number<sub>1</sub> . number<sub>n</sub>'),
	'-': new Builtin('-', function(args) {
		if (args.length == 0) {
		    throw IllegalArgumentCountError('-' ,'at least' ,2 ,args.length);
		} else if (args.length == 1 && Utils.isNumber(args[0])) {
		    return args[0] * -1;
		} else if (args.length == 1) {
		    throw IllegalArgumentTypeError('-' ,args[0] ,1);
		} else {
		    var ans = args[0];
		    if (!Utils.isNumber(ans))
			throw IllegalArgumentTypeError('-' ,ans ,1);
		    for (var i = 1; i < args.length; i++) {
			if (!Utils.isNumber(args[i]))
			    throw IllegalArgumentTypeError('-' ,args[i] ,i+1);
			ans -= args[i];
		    }
		    return ans;
		}
	    }, 'With two or more arguments, returns the difference of the arguments. ' +
	    'With one argument, returns the additive inverse of the argument.',
	    'number<sub>1</sub> . number<sub>n</sub>'),
	'*': new Builtin('*', function(args) {
		return Utils.mapOp(function(lhs ,rhs) { return lhs * rhs; }, 1, args,'*');
	    }, 'Returns the product of the arguments.  With one argument, returns that ' +
	    'argument multiplied by 1.  With no arguments, returns 1.',
	    'number<sub>1</sub> . number<sub>n</sub>'),
	'/': new Builtin('/', function(args) {
		if (args.length == 0) {
		    throw IllegalArgumentCountError('/' ,'at least' ,1 ,args.length);
		} else if (args.length == 1) {
		    return 1 / args[0];
		} else {
		    return Utils.mapOp(function(lhs ,rhs) { return lhs / rhs; }, args[0],
				      Utils.cdr(args),'/');
		}
	    }, 'Returns the quotient of the arguments. With one argument, returns the ' +
	    'multiplicative inverse of the argument.',
	    'number<sub>1</sub> . number<sub>n</sub>')
    });

function Interpreter() { 
    this.parser = new Parser();
    this.history = new History();
    this.buffer = [];
    this.buffln = undefined;
    this.expr = '';
    this.histid = 0;
    this.lineno = 0;
    this.histline = '';
    this.histprog = false;
    this.helpid = 0;
    this.DEFAULT_PREFIX = '&gt;&nbsp;';
    this.CONTINUE_PREFIX = '<span class="continue">..&nbsp;</span>';
    this.prefix = this.DEFAULT_PREFIX;
}

Interpreter.prototype.reset = function() {
    this.lineno = 0;
    this.expr = '';
    this.prefix = this.DEFAULT_PREFIX;
}

Interpreter.prototype.focus = function() {
    $(Document.INPUT).focus();
}

Interpreter.prototype.getline = function() {
    return $F(Document.INPUT);
}
	
Interpreter.prototype.setline = function(line) {
    $(Document.INPUT).value = line;
}

Interpreter.prototype.updateprefix = function(prefix) {
    prefix = prefix === undefined ? this.prefix : prefix;
    $(Document.PREFIX).update(prefix);
}

Interpreter.prototype.getbuff = function() {
    if (this.buffln === undefined) {
	this.buffln = document.createElement('span');
	this.buffln.addClassName('block');
    }
    return this.buffln;
}

Interpreter.prototype.resetbuff = function() {
    if (this.buffln) {
	this.buffer.push(this.buffln);
    }
}

Interpreter.prototype.appendbuff = function(text) {
    this.getbuff().appendChild(document.createTextNode(text));
}

Interpreter.prototype.newline = function() {
    this.getbuff();
    this.resetbuff();
    this.buffln = undefined;
}

function jscm_repl() {
    if (REPL.expr.length == 0 && REPL.getline().strip().length == 0) {
	jscm_printElement();
    } else {
	REPL.lineno++;
	REPL.histid = 0;
	REPL.history.push(REPL.getline());
	REPL.histline = '';
	REPL.histprog = false;
	REPL.expr += Tokens.NEWLINE + REPL.getline();
	var scm = undefined;
	try {
	    scm = REPL.parser.parse(REPL.expr);
	} catch (e) {
	    if (e.isIgnorable()) {
		REPL.prefix = REPL.CONTINUE_PREFIX;
		var prefix = REPL.lineno == 1 ? REPL.DEFAULT_PREFIX : REPL.prefix;
		jscm_printElement(undefined ,prefix);
		REPL.updateprefix();
	    } else {
		jscm_print(e);
		REPL.reset();
	    }
	    return false;
	}
	try {
	    jscm_print(jscm_eval(scm ,GlobalEnvironment));
	} catch (e) {
	    if (e instanceof Escape) {
		e.invoke();
		if (REPL.buffer.length > 0) {
		    jscm_printElement();
		}
	    } else {
		jscm_print(e);
	    }
	}
	REPL.reset();
    }
    return false;
};

function jscm_eval(expr ,env) {
    var action = jscm_expressionToAction(expr);
    if (typeof action == 'function') {
	return action(expr ,env);
    } else {
	throw new TypeError('The object ' + Utils.format(action) +
			    ' is not applicable.');
    }
}

function jscm_beglis(es ,env) {
    for (var i = 0; i < es.length - 1; i++) {
	jscm_eval(es[i] ,env);
    }
    return jscm_eval(es[es.length - 1] ,env);
}

function jscm_evlis(arglis, env) {
    var res = [];
    for (var i = 0; i < arglis.length; i++) {
	res.push(jscm_eval(arglis[i] ,env));
    }
    return res;
}

function jscm_expressionToAction(expr) {
    if (Utils.isAtom(expr)) {
	expr = expr.toLowerCase();
	if (Utils.isNumber(expr) || Utils.isString(expr)) {
	    return Actions.CONST;
	} else {
	    return Actions.IDENTIFIER;
	}
    } else {
	return Actions.APPLICATION;
    }
}

function jscm_print(obj) {
    if (obj instanceof JSWarning) {
	jscm_printBlock(';' + obj ,'warning');
    } else if ((obj instanceof Error) || (obj instanceof JSError)) {
	jscm_printBlock(';' + obj ,'error');
    } else {
	jscm_printBlock(';Value: ' + Utils.format(obj), 'value');
    }
}

function jscm_printElement(element, prefix) {
    prefix = prefix === undefined ? REPL.prefix : prefix;
    var div = document.createElement('div');
    var pre = document.createElement('span');
    pre.update(prefix);
    var expr = document.createElement('pre');
    expr.addClassName(Document.INPUT);
    expr.appendChild(document.createTextNode(REPL.getline()));
    var line = document.createElement('span');
    line.addClassName('line');
    line.appendChild(pre);
    line.appendChild(expr);
    div.appendChild(line);
    REPL.resetbuff();
    for (var i = 0; i < REPL.buffer.length; i++) {
	div.appendChild(REPL.buffer[i]);
    }
    REPL.buffer = [];
    REPL.buffln = undefined;
    if (element) {
	div.appendChild(element);
    }
    $(Document.CONSOLE).appendChild(div);
    REPL.setline('');
    REPL.focus();
    REPL.updateprefix(REPL.DEFAULT_PREFIX);
    window.scrollTo(0 ,document.body.scrollHeight);
}

function jscm_printBlock(text ,className) {
    var span = document.createElement('span');
    span.addClassName(className);
    span.addClassName('block');
    span.appendChild(document.createTextNode(text));
    jscm_printElement(span);
}

function jscm_printDisplay(text) {
    REPL.appendbuff(text);
}

function jscm_printHelp(args) {
    jscm_printElement();
    REPL.helpid++;
    var div = document.createElement('div');
    div.addClassName('help');
    if (args.length == 0) {
	div.update(jscm_getHelp());
    } else if (args.length == 1)  {
	var arg = args[0];
	if (!arg.doc && JSCMLibs.get(arg)) {
	    arg = JSCMLibs.get(arg);
	}
	if (arg.doc) {
	    div.update(jscm_getBuiltinHelp(arg));
	}
    }
    $(Document.CONSOLE).appendChild(div);
    window.scrollTo(0, document.body.scrollHeight);
}

function jscm_printToggleBox(title, html) {
    jscm_printElement();
    REPL.helpid++;
    var div = document.createElement('div');
    div.addClassName('help');
    div.update('<h1><span>' + title + '</span> ' +
	       jscm_getToggleLinkFor('helpBody' ,'helpMin') + '</h1>' +
	       '<div class="helpBody"><div id="helpBody' + REPL.helpid + '">' +
	       html + '</div>');
    $(Document.CONSOLE).appendChild(div);
    window.scrollTo(0, document.body.scrollHeight);
}

function jscm_getHelp() {
    var builtins = '<h2><span>BUILT-IN PROCEDURES</span></h2>' +
	'<div class="builtinList" id="builtinList' + REPL.helpid + '">' +
	'<p>' +
	'To view documentation for a built-in procedure, use ' +
	'<strong>(help <em>proc</em>)</strong> where ' +
	'<strong><em>proc</em></strong> is the procedure to lookup.' +
	'</p>' +
	'<p>' +
	'Enter <strong>(display-built-ins)</strong> to view a list of the ' +
	'<strong>' + Builtin.instances + '</strong> ' +
	'built-ins.' +
	'</p>';
    return '<h1><span>JS-SCHEME HELP</span> ' +
	jscm_getToggleLinkFor('helpBody','helpMin') + '</h1><div class="helpBody">'+
	'<div id="helpBody' + REPL.helpid + '">' +
	'<p>Welcome to TJSS ' + TJSS.version + '!</p>' +
	'<p>' +
	'This interpreter is inspired from ' +
	'<a href="http://code.google.com/p/js-scheme/"> JS-Scheme </a>' +
	'</p>' +
	'<p>' +
	'This project is planned to extend to be a web tutorial for' +
	'<a href="http://www.gnu.org/software/guile/"> GNU/Guile </a>' +
	'</p>' +
	builtins +
	'</div></div>';
}

function jscm_getBuiltinHelp(proc) {
    return '<h1><span>Tiny JS Scheme HELP</span> ' +
	'<span class="syntax"><strong>(' + proc.name +
	'</strong>' + (proc.argdoc ? ' <em>' + proc.argdoc + '</em>' : '') +
	'<strong>)</strong></span>' +
	jscm_getToggleLinkFor('helpBody', 'helpMin')+'</h1><div class="helpBody">' +
	'<div id="helpBody' + REPL.helpid + '">' + proc.doc + '</div></div>';
}

function jscm_printBuiltinsHelp() {
    jscm_printElement();
    REPL.helpid++;
    var div = document.createElement('div');
    div.addClassName('help');
    div.update(jscm_getBuiltinsHTML());
    $(Document.CONSOLE).appendChild(div);
    window.scrollTo(0, document.body.scrollHeight);
}

function jscm_getHelpList(keys ,ITEMS_PER_COL ,test) {
    if (test === undefined)
	test = function(arg) { return true; };
    var tab = 0;
    var open = true;
    var list = '<ul>';
    keys.sort();
    for (var i = 0; i < keys.length; i++) {
	if (test(keys[i])) {
	    tab++;
	    if (!open)
		list += '<ul>';
	    open = true;
	    list += '<li>' + keys[i] + '</li>';
	    if (tab > ITEMS_PER_COL - 1 && tab % ITEMS_PER_COL == 0) {
		open = false;
		list += '</ul>';
	    }
	}
    }
    list += (open ? '</ul>' : '');
    return list;
}

function jscm_getBuiltinsHTML() {
    var ITEMS_PER_COL = 35;
    var keys = ReservedSymbolTable.keys();
    var isBuiltin = function(key) {
	return ReservedSymbolTable.get(key) instanceof Builtin;
    };
    return '<h1><span>BUILTIN-PROCEDURES</span>' +
	jscm_getToggleLinkFor('builtinList' ,'helpMin') +
	'</h1><div class="helpBody">' +
	'<div class="builtinList" id="builtinList' + REPL.helpid + '">' +
	'<div>' + jscm_getHelpList(keys ,ITEMS_PER_COL ,isBuiltin) + '</div></div>'+
	'<div style="clear:both;"></div></div>';
}

function jscm_getToggleLinkFor(what ,cssClass ,text) {
    if (text == undefined) text = '[toggle]';
    cssClass = cssClass ? (' class="' + cssClass + '" ') : '';
    return '<a href="#" onclick="$(\'' + what + REPL.helpid + '\').toggle();' +
	'return false;"' + cssClass + '>' + text + '</a>';
}

function jscm_onkeydown(e) {
    var code = e.keyCode;
    if (code == Document.KEY_DOWN && REPL.histid > 0) {
	if (REPL.histid >= REPL.history.size()) {
	    REPL.histid = REPL.history.size() - 1;
	}
	var ln = REPL.histid > 0 ? REPL.history.get(--REPL.histid) : REPL.histline;
	REPL.setline(ln);
	REPL.focus();
    } else if (code == Document.KEY_UP && REPL.histid < REPL.history.size()) {
	if (!REPL.histprog) {
	    REPL.histline = REPL.getline();
	}
	REPL.histprog = true;
	if (REPL.histid < 0) {
	    REPL.histid = 0;
	}
	REPL.setline(REPL.history.get(REPL.histid++));
	REPL.focus();
    } else if (code == Document.KEY_DOWN) {
	REPL.setline(REPL.histline);
	REPL.histid = 0;
	REPL.focus();
    } else if (code != Document.KEY_UP) {
	REPL.histprog = false;
    }
}

window.onload = function() {
    GlobalEnvironment = new Environment();
    REPL = new Interpreter();
    $(Document.INPUT).onkeydown = jscm_onkeydown;
    REPL.focus();
};
