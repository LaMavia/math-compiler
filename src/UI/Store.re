type base = [ | `bin | `dec | `hex];
let set_of_base = b =>
  switch (b) {
  | `bin => (2, "bin")
  | `dec => (10, "dec")
  | `hex => (16, "hex")
  };

type app_state = {
  vars: list(string),
  funcs: list(string),
  input: string,
  history: list(string),
  history_i: int,
  ans: Grammar.node,
  base,
};

type app_action =
  // -------- Vars -------- //
  | AddVar(string)
  | UpdateVar(string, int)
  | /**(index, amount) */
    MoveVar(int, int)
  | RemoveVar(int)
  // -------- Funcs -------- //
  | AddFunc(string)
  | RemoveFunc(int)
  | UpdateFunc(string, int)
  | /**(index, amount) */
    MoveFunc(int, int)
  // -------- input -------- //
  | ChangeInput(string)
  | ConcatInput(string, int)
  | Del(int)
  | AC
  | LoadHist(int)
  | ChangeBase
  | Calc;

let move_item = (arr, var_i, i') =>
  if (var_i + i' > List.length(arr) - 1 || var_i + i' < 0) {
    arr;
  } else {
    arr->Belt.List.mapWithIndex((j, v) =>
      if (j == var_i) {
        List.nth(arr, var_i + i');
      } else if (j == var_i + i') {
        List.nth(arr, var_i);
      } else {
        v;
      }
    );
  };

let app_reducer = (state, action) =>
  {
    let i0 = -1;
    switch (action) {
  | ChangeInput(n_inp) => {...state, input: n_inp}
  | ConcatInput(str, offset) =>
    let s = state.input;
    let len = Js.String2.length(s);

    {
      ...state,
      input:
        Js.String2.(
          slice(s, ~from=0, ~to_=len - offset)
          ++ str
          ++ sliceToEnd(s, ~from=len - offset)
        ),
    };
  | Calc =>
    let ans =
      state.input
      ->Calc.calculate(
          [
            {name: "ans", val_: state.ans},
            ...state.vars->Belt.List.keepMap(Parser.var_of_string),
          ],
          state.funcs
          ->Belt.List.keepMap(f =>
              f
              ->Parser.func_exp_of_string
              ->Belt.Option.map(x => Evaluables.UserFunc(x))
            )
          ->Belt.List.toArray,
        );
    {
      ...state,
      input: "",
      history_i: i0,
      history: [state.input, ...state.history],
      ans: Grammar.Number(ans->Js.Float.toString),
    };

  | LoadHist(di) =>
    let i' = state.history_i + di;
    let input =
      Belt.List.get(state.history, i')
      ->Belt.Option.getWithDefault(state.input);
    let hist_len = state.history->List.length;

    {
      ...state,
      input,
      history_i:
        if (i' < 0) {
          0;
        } else if (i' > hist_len - 1) {
          hist_len - 1;
        } else {
          i';
        },
    };

  | ChangeBase =>
    let base =
      switch (state.base) {
      | `bin => `dec
      | `dec => `hex
      | `hex => `bin
      };

    {...state, base};

  | Del(offset) =>
    let s = state.input;
    let len = Js.String2.length(s);
    {
      ...state,
      input:
        Js.String2.(
          slice(s, ~from=0, ~to_=len - offset - 1)
          ++ sliceToEnd(s, ~from=len - offset)
        ),
    };
  | AC => {...state, input: "", ans: Number("0"), history_i: i0}

  | AddVar(v) => {...state, input: "", vars: [v, ...state.vars]}
  | AddFunc(f) => {...state, input: "", funcs: [f, ...state.funcs]}

  | RemoveVar(i) => {
      ...state,
      vars: state.vars->Belt.List.keepWithIndex((_, j) => j != i),
    }

  | MoveVar(var_i, i') => {...state, vars: move_item(state.vars, var_i, i')}
  | MoveFunc(func_i, i') => {
      ...state,
      vars: move_item(state.funcs, func_i, i'),
    }

  | UpdateVar(new_val, i) => {
      ...state,
      vars:
        state.vars->Belt.List.mapWithIndex((j, v) => i == j ? new_val : v),
    }
  | UpdateFunc(new_val, i) => {
      ...state,
      funcs:
        state.funcs->Belt.List.mapWithIndex((j, v) => i == j ? new_val : v),
    }

  | _ => state
  }};
let app_store: Reductive.Store.t(app_action, app_state) =
  Reductive.Store.create(
    ~reducer=app_reducer,
    ~preloadedState={
      vars: ["x=12^2", "y=1^3+2", "z=16-4"],
      funcs: ["f(x) = e^x + e^(-x)"],
      input: "",
      history: [],
      history_i: (-1),
      ans: Grammar.Number("0"),
      base: `dec,
    },
    (),
  );

include ReductiveContext.Make({
  type action = app_action;
  type state = app_state;
});