let fs =
  Belt.Array.(
    [|"f(x) = x^2-3"|]
    ->map(Parser.func_exp_of_string)
    ->keepMap(a =>
        switch (a) {
        | Some(a) => Some(Evaluables.UserFunc(a))
        | None => None
        }
      )
  );
let eqs = [|"2*3^5-1", "f(2)", "2^2+3+(1+4)!"|];
eqs[0]
->Js.String2.splitByRe(Regex.re_space)
->Belt.Array.reduce([], (acc, c) =>
    switch (c) {
    | Some(c) => acc @ Lexer.lex(c)
    | _ => acc
    }
  )
->Parser.parse
->Exec.exec([], Belt.Array.concat(Evaluables.static_functions, fs)) /* ->Helpers.deep_log*/;
// ->Belt.List.toArray

ReactDOMRe.renderToElementWithId(<Ui__App />, "root");