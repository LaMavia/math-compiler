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

let fact = {
  let rec f = n =>
    if (n < 2.0) {
      1.0;
    } else {
      n *. f(n -. 1.0);
    };
  f;
};

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
    // rest
    {name: "log", eval: log},
    {name: "neg", eval: (~-.)},
    {name: "abs", eval: abs_float},
    {name: "sqrt", eval: sqrt},
    {name: "ceil", eval: ceil},
    {name: "round", eval: Js.Math.round},
    {name: "floor", eval: floor},
    {
      name: "zeta",
      eval: s => {
        let range = 1000.0;

        let rec f = n =>
          if (n >= range) {
            1. /. n ** s;
          } else {
            1. /. n ** s +. f(n +. 1.);
          };
        f(1.);
      },
    },
    {name: "!", eval: fact},
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
  {name: "#", identity: 0.0, eval: (a, e) => a *. 10. ** e},
  {name: "mod", identity: max_float, eval: mod_float},
  {name: "root", identity: max_float, eval: Math.root},
  {
    name: "choose",
    identity: 1.0,
    eval: (n, k) => fact(n) /. (fact(k) *. fact(n -. k)),
  },
  {name: "perm", identity: 1.0, eval: (n, k) => fact(n) /. fact(n -. k)},
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

let global_scope =
  [
    ("e", Js.Math._E),
    ("pi", Js.Math._PI),
    ("R", 8.31),
    ("kb", 1.38064852 *. 10. ** (-23.)),
    ("Na", 6.02214086 *. 10. ** 23.),
  ]
  ->Belt.List.map(((name, val_)) =>
      {name, val_: val_->Js.Float.toString->Grammar.Number}
    );

let is_op = c => Array.exists(({name}: operator) => {name == c}, operators);
let is_suffix = c => ["!"] |> List.exists(o => o == c);