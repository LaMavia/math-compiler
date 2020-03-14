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

"e"->Calc.calculate([], [||])->Helpers.deep_log;
ReactDOMRe.renderToElementWithId(
  <Store.Provider store=Store.app_store> <Router /> </Store.Provider>,
  "root",
);