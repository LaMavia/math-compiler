'use strict';

var Util = require("util");

function deep(x) {
  return Util.inspect(x, {
              depth: 50,
              colors: true
            });
}

function deep_log(x) {
  console.log(deep(x));
  return /* () */0;
}

var do_log = true;

exports.deep = deep;
exports.do_log = do_log;
exports.deep_log = deep_log;
/* util Not a pure module */
