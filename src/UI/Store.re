type app_state = {
  vars: list(Evaluables.var),
  funcs: list(Evaluables.func),
  input: string,
  history: list(string),
  ans: Grammar.node,
};

type app_action =
  | AddVar(string)
  | RemoveVar(string)
  | ChangeInput(string)
  | Del(int)
  | Calc;

let app_reducer = (state, action) =>
  switch (action) {
  | ChangeInput(n_inp) => {...state, input: n_inp}
  | Calc =>
    let ans =
      state.input
      ->Calc.calculate(
          [{name: "ans", val_: state.ans}, ...state.vars],
          state.funcs->Belt.List.toArray,
        );
    {
      ...state,
      input: "",
      history: [state.input, ...state.history],
      ans: Grammar.Number(ans->Js.Float.toString),
    };

  | Del(offset) =>
    let s = state.input;
    let len = Js.String2.length(s);
    {
      ...state,
      input:
        Js.String2.(
          slice(s, ~from=0, ~to_=len - offset)
          ++ sliceToEnd(s, ~from=len - offset + 1)
        ),
    };

  | _ => state
  };
let app_store: Reductive.Store.t(app_action, app_state) =
  Reductive.Store.create(
    ~reducer=app_reducer,
    ~preloadedState={
      vars: [],
      funcs: [],
      input: "",
      history: [],
      ans: Grammar.Number("0"),
    },
    (),
  );

include ReductiveContext.Make({
  type action = app_action;
  type state = app_state;
});