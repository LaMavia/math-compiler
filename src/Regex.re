open Js.Re;
let re_char_separator = [%bs.re "/\s*/gi"];
let re_num = [%bs.re "/[\d\.]+/"];
let re_name = [%bs.re "/[a-zA-Z]/i"];
let re_space_any = [%bs.re "/\s*/"];
let re_sign = [%bs.re "/[\+\-]/"];
let re_eol = Js.Re.fromString("\n+$");
let re_space = [%bs.re "/\s+/g"];

let is_num = test_(re_num);
let is_name = test_(re_name);
let is_sign = test_(re_sign);

let escape_regex = str =>
  str->Js.String2.replaceByRe([%bs.re "/[.*+?^${}()|[\]\\-]/g"], "\\$&");

/**matches: (name, variable, expression) */
let re_function_exp = [%bs.re "/(?<name>\w+)\((?<var>\w)\)\s*=\s*(?<exp>.*)/"];