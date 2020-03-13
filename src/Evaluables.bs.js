'use strict';

var List = require("bs-platform/lib/js/list.js");
var $$Array = require("bs-platform/lib/js/array.js");
var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var Js_exn = require("bs-platform/lib/js/js_exn.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Pervasives = require("bs-platform/lib/js/pervasives.js");

function f(n) {
  if (n < 2.0) {
    return 1.0;
  } else {
    return n * f(n - 1.0);
  }
}

var static_functions = $$Array.map((function (x) {
        return /* StaticFunc */Block.__(1, [x]);
      }), [
      {
        name: "cos",
        eval: (function (prim) {
            return Math.cos(prim);
          })
      },
      {
        name: "cosh",
        eval: (function (prim) {
            return Math.cosh(prim);
          })
      },
      {
        name: "sin",
        eval: (function (prim) {
            return Math.sin(prim);
          })
      },
      {
        name: "sinh",
        eval: (function (prim) {
            return Math.sinh(prim);
          })
      },
      {
        name: "tan",
        eval: (function (prim) {
            return Math.tan(prim);
          })
      },
      {
        name: "tanh",
        eval: (function (prim) {
            return Math.tanh(prim);
          })
      },
      {
        name: "log",
        eval: (function (prim) {
            return Math.log(prim);
          })
      },
      {
        name: "neg",
        eval: (function (prim) {
            return -prim;
          })
      },
      {
        name: "!",
        eval: f
      }
    ]);

var operators = [
  {
    name: "+",
    eval: (function (prim, prim$1) {
        return prim + prim$1;
      }),
    identity: 0.0
  },
  {
    name: "-",
    eval: (function (prim, prim$1) {
        return prim - prim$1;
      }),
    identity: 0.0
  },
  {
    name: "*",
    eval: (function (prim, prim$1) {
        return prim * prim$1;
      }),
    identity: 1.0
  },
  {
    name: "/",
    eval: (function (prim, prim$1) {
        return prim / prim$1;
      }),
    identity: 1.0
  },
  {
    name: "^",
    eval: (function (prim, prim$1) {
        return Math.pow(prim, prim$1);
      }),
    identity: 1.0
  },
  {
    name: "_",
    eval: (function (a, b) {
        return Math.pow(b, 1.0 / a);
      }),
    identity: 1.0
  },
  {
    name: "mod",
    eval: (function (prim, prim$1) {
        return prim % prim$1;
      }),
    identity: Pervasives.max_float
  }
];

function eval_op(op, f) {
  var match = Belt_Array.getBy(operators, (function (param) {
          return param.name === op;
        }));
  if (match !== undefined) {
    return Curry._1(f, match);
  } else {
    throw Js_exn.raiseReferenceError("Operator (" + (String(op) + ") is not defined in the current scope"));
  }
}

function is_op(c) {
  return $$Array.exists((function (param) {
                return param.name === c;
              }), operators);
}

function is_suffix(c) {
  return List.exists((function (o) {
                return o === c;
              }), /* :: */[
              "!",
              /* [] */0
            ]);
}

exports.static_functions = static_functions;
exports.operators = operators;
exports.eval_op = eval_op;
exports.is_op = is_op;
exports.is_suffix = is_suffix;
/* static_functions Not a pure module */
