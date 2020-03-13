'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Js_exn = require("bs-platform/lib/js/js_exn.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Caml_format = require("bs-platform/lib/js/caml_format.js");
var Regex$CompilerTest = require("./Regex.bs.js");
var Evaluables$CompilerTest = require("./Evaluables.bs.js");

function get_var(scope, name) {
  var match = Belt_List.getBy(scope, (function (param) {
          return param.name === name;
        }));
  if (match !== undefined) {
    return match.val_;
  } else {
    throw Js_exn.raiseReferenceError("Undefined variable \"" + (String(name) + "\""));
  }
}

function get_func(funcs, name) {
  var match = Belt_Array.getBy(funcs, (function (f) {
          return f[0].name === name;
        }));
  if (match !== undefined) {
    return match;
  } else {
    return Js_exn.raiseReferenceError("Undefined function \"" + (String(name) + "\""));
  }
}

function exec(tree, global_scope, funcs) {
  var calc = function (_node, _scope) {
    while(true) {
      var scope = _scope;
      var node = _node;
      if (typeof node === "number") {
        return 0;
      } else {
        switch (node.tag | 0) {
          case /* Number */0 :
              return Caml_format.caml_float_of_string(node[0]);
          case /* Variable */1 :
              return get_var(scope, node[0]);
          case /* NVal */2 :
              _node = node[0];
              continue ;
          case /* NFunc */3 :
              var arg = node[1];
              var f_def = get_func(funcs, node[0]);
              if (f_def.tag) {
                return Curry._1(f_def[0].eval, calc(arg, scope));
              } else {
                var match = f_def[0];
                var f_scope_000 = {
                  name: match.var,
                  val_: calc(arg, scope)
                };
                var f_scope = /* :: */[
                  f_scope_000,
                  scope
                ];
                _scope = f_scope;
                _node = match.exp;
                continue ;
              }
          case /* NInfix */4 :
              var b = node[2];
              var a = node[1];
              var op = node[0];
              if (typeof b === "number" && Regex$CompilerTest.is_sign(op)) {
                return Evaluables$CompilerTest.eval_op(op, (function(scope,a){
                          return function (param) {
                            return Curry._2(param.eval, param.identity, calc(a, scope));
                          }
                          }(scope,a)));
              }
              return Evaluables$CompilerTest.eval_op(op, (function(scope,a,b){
                        return function (param) {
                          return Curry._2(param.eval, calc(a, scope), calc(b, scope));
                        }
                        }(scope,a,b)));
          
        }
      }
    };
  };
  return calc(tree, global_scope);
}

exports.get_var = get_var;
exports.get_func = get_func;
exports.exec = exec;
/* Regex-CompilerTest Not a pure module */
