'use strict';


var re_char_separator = /\s*/gi;

var re_num = /[\d\.]+/;

var re_name = /[a-zA-Z]/i;

var re_space_any = /\s*/;

var re_sign = /[\+\-]/;

var re_eol = new RegExp("\n+$");

var re_space = /\s+/g;

function is_num(param) {
  return re_num.test(param);
}

function is_name(param) {
  return re_name.test(param);
}

function is_sign(param) {
  return re_sign.test(param);
}

function escape_regex(str) {
  return str.replace(/[.*+?^${}()|[\]\-]/g, "\\$&");
}

var re_function_exp = /(?<name>\w+)\((?<var>\w)\)\s*=\s*(?<exp>.*)/;

exports.re_char_separator = re_char_separator;
exports.re_num = re_num;
exports.re_name = re_name;
exports.re_space_any = re_space_any;
exports.re_sign = re_sign;
exports.re_eol = re_eol;
exports.re_space = re_space;
exports.is_num = is_num;
exports.is_name = is_name;
exports.is_sign = is_sign;
exports.escape_regex = escape_regex;
exports.re_function_exp = re_function_exp;
/* re_eol Not a pure module */
