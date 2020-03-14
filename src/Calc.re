let calculate = (exp, scope, functions) =>
  exp
  ->Js.String2.splitByRe(Regex.re_space)
  ->Belt.Array.reduce([], (acc, c) =>
      switch (c) {
      | Some(c) => acc @ Lexer.lex(c)
      | _ => acc
      }
    )
  ->Parser.parse
  ->Exec.exec(
      Evaluables.global_scope @ scope,
      Belt.Array.concat(Evaluables.static_functions, functions),
    );