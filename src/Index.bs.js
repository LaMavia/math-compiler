'use strict';

var Block = require("bs-platform/lib/js/block.js");
var React = require("react");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Caml_array = require("bs-platform/lib/js/caml_array.js");
var Pervasives = require("bs-platform/lib/js/pervasives.js");
var ReactDOMRe = require("reason-react/src/ReactDOMRe.js");
var Exec$CompilerTest = require("./Exec.bs.js");
var Lexer$CompilerTest = require("./Lexer.bs.js");
var Regex$CompilerTest = require("./Regex.bs.js");
var Parser$CompilerTest = require("./Parser.bs.js");
var Ui__App$CompilerTest = require("./UI/Ui__App.bs.js");
var Evaluables$CompilerTest = require("./Evaluables.bs.js");

var fs = Belt_Array.keepMap(Belt_Array.map(["f(x) = x^2-3"], Parser$CompilerTest.func_exp_of_string), (function (a) {
        if (a !== undefined) {
          return /* UserFunc */Block.__(0, [a]);
        }
        
      }));

var eqs = [
  "2*3^5-1",
  "f(2)",
  "2^2+3+(1+4)!"
];

Exec$CompilerTest.exec(Parser$CompilerTest.parse(Belt_Array.reduce(Caml_array.caml_array_get(eqs, 0).split(Regex$CompilerTest.re_space), /* [] */0, (function (acc, c) {
                if (c !== undefined) {
                  return Pervasives.$at(acc, Lexer$CompilerTest.lex(c));
                } else {
                  return acc;
                }
              }))), /* [] */0, Belt_Array.concat(Evaluables$CompilerTest.static_functions, fs));

ReactDOMRe.renderToElementWithId(React.createElement(Ui__App$CompilerTest.make, { }), "root");

exports.fs = fs;
exports.eqs = eqs;
/* fs Not a pure module */
