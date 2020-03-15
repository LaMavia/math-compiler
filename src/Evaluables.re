open Grammar;
type func_exp = {
  name: string,
  var: string,
  exp: node,
};

type func_static = {
  name: string,
  eval: float => float,
};

type func =
  | UserFunc(func_exp)
  | StaticFunc(func_static);

let static_functions =
  [|
    // cos
    {name: "cos", eval: cos},
    {name: "acos", eval: acos},
    {name: "cosh", eval: cosh},
    // sin
    {name: "sin", eval: sin},
    {name: "asin", eval: asin},
    {name: "sinh", eval: sinh},
    // tan
    {name: "tan", eval: tan},
    {name: "atan", eval: atan},
    {name: "tanh", eval: tanh},
    {name: "log", eval: log},
    {name: "neg", eval: (~-.)},
    {name: "abs", eval: abs_float},
    {
      name: "!",
      eval: {
        let rec f = n =>
          if (n < 2.0) {
            1.0;
          } else {
            n *. f(n -. 1.0);
          };
        f;
      },
    },
  |]
  |> Array.map(x => StaticFunc(x));

type operator = {
  name: string,
  eval: (float, float) => float,
  identity: float,
};

let operators = [|
  {name: "+", identity: 0.0, eval: (+.)},
  {name: "-", identity: 0.0, eval: (-.)},
  {name: "*", identity: 1.0, eval: ( *. )},
  {name: "/", identity: 1.0, eval: (/.)},
  {name: "^", identity: 1.0, eval: ( ** )},
  {name: "_", identity: 1.0, eval: Math.root}, //
  {name: "mod", identity: max_float, eval: mod_float},
  {name: "root", identity: max_float, eval: Math.root},
|];

let eval_op = (op, f) =>
  switch (Belt.Array.getBy(operators, ({name}) => name == op)) {
  | Some(def) => f(def)
  | None =>
    raise(
      Js.Exn.raiseReferenceError(
        {j|Operator ($op) is not defined in the current scope|j},
      ),
    )
  };

type var = {
  name: string,
  val_: node,
};

let global_scope = [
  {name: "e", val_: Js.Math._E->Js.Float.toString->Grammar.Number},
  {name: "pi", val_: Js.Math._PI->Js.Float.toString->Grammar.Number},
  {name: "R", val_: 8.31->Js.Float.toString->Grammar.Number},
];

let is_op = c => Array.exists(({name}: operator) => {name == c}, operators);
let is_suffix = c => ["!"] |> List.exists(o => o == c);