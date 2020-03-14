open Grammar;
open Evaluables;

let get_var = (scope: list(Evaluables.var), name) =>
  switch (scope->Belt.List.getBy(({name: f}) => f == name)) {
  | Some(v) => v.val_
  | None =>
    raise(Js.Exn.raiseReferenceError({j|Undefined variable "$name"|j}))
  };

let get_func = (funcs: array(Evaluables.func), name) =>
  switch (
    funcs->Belt.Array.getBy(f =>
      switch (f) {
      | StaticFunc({name: f})
      | UserFunc({name: f}) => f == name
      }
    )
  ) {
  | Some(f) => f
  | None => Js.Exn.raiseReferenceError({j|Undefined function "$name"|j})
  };

/**(tree: node, global_scope: list(var), funcs: list(func)) */
let exec = (tree, global_scope, funcs) => {
  let rec calc = (node, scope) =>
    switch (node) {
    | Empty => 0.
    | Number(n) => float_of_string(n)
    | Variable(v) => get_var(scope, v)
    | NVal(v) => calc(v, scope)
    | NFunc(f, arg) =>
      let f_def = get_func(funcs, f);
      switch (f_def) {
      | UserFunc({var, exp}) =>
        let f_scope = [{name: var, val_: calc(arg, scope)}, ...scope];
        calc(exp, f_scope);
      | StaticFunc({eval}) => eval(calc(arg, scope))
      };
    | NInfix(op, a, Empty) when Regex.is_sign(op) =>
      Evaluables.eval_op(op, ({identity: i, eval: f}) =>
        f(i, calc(a, scope))
      )
    | NInfix(op, a, b) =>
      Evaluables.eval_op(op, ({eval: f}) =>
        f(calc(a, scope), calc(b, scope))
      )
    | x =>
      raise(
        Js.Exn.raiseEvalError(
          "Compilation error: ye've just witnessed some weird ass witchery...\n" ++ Helpers.deep(x),
        ),
      )
    };
  calc(tree, global_scope);
};