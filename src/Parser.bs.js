'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var Js_exn = require("bs-platform/lib/js/js_exn.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Pervasives = require("bs-platform/lib/js/pervasives.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var Lexer$CompilerTest = require("./Lexer.bs.js");
var Regex$CompilerTest = require("./Regex.bs.js");
var Grammar$CompilerTest = require("./Grammar.bs.js");
var Helpers$CompilerTest = require("./Helpers.bs.js");
var Evaluables$CompilerTest = require("./Evaluables.bs.js");

function tripple_map(list, func) {
  var loop = function (arr) {
    if (arr === /* [] */0) {
      return /* [] */0;
    } else {
      var match;
      if (arr) {
        var match$1 = arr[1];
        var a = arr[0];
        if (match$1) {
          var match$2 = match$1[1];
          var b = match$1[0];
          if (match$2) {
            var rest = match$2[1];
            var c = match$2[0];
            match = rest ? /* tuple */[
                Caml_option.some(a),
                Caml_option.some(b),
                Caml_option.some(c),
                rest
              ] : /* tuple */[
                Caml_option.some(a),
                Caml_option.some(b),
                Caml_option.some(c),
                /* [] */0
              ];
          } else {
            match = /* tuple */[
              Caml_option.some(a),
              Caml_option.some(b),
              undefined,
              /* [] */0
            ];
          }
        } else {
          match = /* tuple */[
            Caml_option.some(a),
            undefined,
            undefined,
            /* [] */0
          ];
        }
      } else {
        throw Js_exn.raiseError("Encountered an unmatched list destructiring pattern \"" + (String(arr) + "\""));
      }
      return Pervasives.$at(Curry._3(func, match[0], match[1], match[2]), loop(match[3]));
    }
  };
  return loop(list);
}

function eval_op(op, f) {
  var match = Belt_Array.getBy(Evaluables$CompilerTest.operators, (function (param) {
          return param.name === op;
        }));
  if (match !== undefined) {
    return Curry._1(f, match);
  } else {
    throw Js_exn.raiseReferenceError("Operator (" + (String(op) + ") is not defined in the current scope"));
  }
}

function insert(tree, v) {
  if (v === /* Empty */0) {
    return tree;
  } else if (typeof tree === "number") {
    return v;
  } else {
    switch (tree.tag | 0) {
      case /* NFunc */3 :
          return /* NFunc */Block.__(3, [
                    tree[0],
                    v
                  ]);
      case /* NInfix */4 :
          return /* NInfix */Block.__(4, [
                    tree[0],
                    tree[1],
                    v
                  ]);
      default:
        throw Js_exn.raiseError("Unmatched type of tree while inserting { " + (String(tree) + " }"));
    }
  }
}

function parse(tokens) {
  var rest = {
    contents: tokens
  };
  var loop = function (_tree, _state) {
    while(true) {
      var state = _state;
      var tree = _tree;
      var match = rest.contents;
      var match$1 = match ? /* tuple */[
          match[0],
          match[1]
        ] : /* tuple */[
          undefined,
          /* [] */0
        ];
      var token = match$1[0];
      rest.contents = match$1[1];
      var exit = 0;
      var exit$1 = 0;
      if (token !== undefined) {
        var match$2 = token;
        switch (match$2.tag | 0) {
          case /* Eval */0 :
              if (typeof state === "number" || state.tag !== /* NInfix */4) {
                exit$1 = 3;
              } else {
                var match$3 = token[0];
                var exit$2 = 0;
                switch (match$3.tag | 0) {
                  case /* Infix */0 :
                      exit = 2;
                      break;
                  case /* Function */1 :
                  case /* Suffix */2 :
                      exit$2 = 4;
                      break;
                  
                }
                if (exit$2 === 4) {
                  _state = /* NInfix */Block.__(4, [
                      state[0],
                      state[1],
                      /* NFunc */Block.__(3, [
                          match$3[0],
                          state[2]
                        ])
                    ]);
                  continue ;
                }
                
              }
              break;
          case /* Special */1 :
              if (match$2[0][0]) {
                return /* NVal */Block.__(2, [insert(tree, state)]);
              } else {
                _state = insert(state, loop(/* Empty */0, /* Empty */0));
                continue ;
              }
          case /* Rune */2 :
              var match$4 = match$2[0];
              if (match$4.tag) {
                _state = insert(state, /* Number */Block.__(0, [match$4[0]]));
                continue ;
              } else {
                _state = insert(state, /* Variable */Block.__(1, [match$4[0]]));
                continue ;
              }
          
        }
      } else {
        return /* NVal */Block.__(2, [insert(tree, state)]);
      }
      if (exit$1 === 3) {
        var match$5 = token[0];
        switch (match$5.tag | 0) {
          case /* Infix */0 :
              exit = 2;
              break;
          case /* Function */1 :
          case /* Suffix */2 :
              _state = /* NFunc */Block.__(3, [
                  match$5[0],
                  state
                ]);
              continue ;
          
        }
      }
      if (exit === 2) {
        if (typeof tree === "number") {
          _state = /* Empty */0;
          _tree = /* NInfix */Block.__(4, [
              token[0][0],
              state,
              /* Empty */0
            ]);
          continue ;
        } else if (tree.tag === /* NInfix */4) {
          var op = token[0][0];
          var old_op = tree[0];
          if (Grammar$CompilerTest.weight(op) > Grammar$CompilerTest.weight(old_op)) {
            _state = /* NInfix */Block.__(4, [
                op,
                state,
                /* Empty */0
              ]);
            continue ;
          } else if (Grammar$CompilerTest.weight(op) <= Grammar$CompilerTest.weight(old_op)) {
            _state = /* Empty */0;
            _tree = /* NInfix */Block.__(4, [
                op,
                insert(tree, state),
                /* Empty */0
              ]);
            continue ;
          }
          
        }
        
      }
      throw Js_exn.raiseError("Encountered an unmatched loop case: \n" + (Helpers$CompilerTest.deep(tree) + ("\n" + (Helpers$CompilerTest.deep(state) + ("\n" + Helpers$CompilerTest.deep(token))))));
    };
  };
  return loop(/* Empty */0, /* Empty */0);
}

function func_exp_of_string(f) {
  return Belt_Option.map(Belt_Option.map(Caml_option.null_to_opt(Regex$CompilerTest.re_function_exp.exec(f)), (function (r) {
                    return Belt_Array.sliceToEnd(Belt_Array.map(r, (function (x) {
                                      return Belt_Option.getExn((x == null) ? undefined : Caml_option.some(x));
                                    })), 1);
                  })), (function (r) {
                if (r.length !== 3) {
                  throw Js_exn.raiseError("Error converting \"" + (String(f) + "\" to a function expression"));
                }
                var n = r[0];
                var v = r[1];
                var e = r[2];
                return {
                        name: n,
                        var: v,
                        exp: parse(Lexer$CompilerTest.lex(e))
                      };
              }));
}

exports.tripple_map = tripple_map;
exports.eval_op = eval_op;
exports.insert = insert;
exports.parse = parse;
exports.func_exp_of_string = func_exp_of_string;
/* Lexer-CompilerTest Not a pure module */
