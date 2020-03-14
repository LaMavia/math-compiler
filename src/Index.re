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

"2 mod 5"
->Calc.calculate([], fs)
->Helpers.deep_log /* ReactDOMRe.renderToElementWithId(<Ui__App />, "root")*/ /* ->Belt.List.toArra*/;