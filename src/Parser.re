open Evaluables;
open Grammar;

let tripple_map = (list, func) => {
  let rec loop = arr =>
    if (arr == []) {
      [];
    } else {
      let (a, b, c, rest) =
        switch (arr) {
        | [a] => (Some(a), None, None, [])
        | [a, b] => (Some(a), Some(b), None, [])
        | [a, b, c] => (Some(a), Some(b), Some(c), [])
        | [a, b, c, ...rest] => (Some(a), Some(b), Some(c), rest)
        | x =>
          raise(
            Js.Exn.raiseError(
              {j|Encountered an unmatched list destructiring pattern "$x"|j},
            ),
          )
        };

      func(a, b, c) @ loop(rest);
    };

  loop(list);
};

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

let insert = (tree, v) =>
  if (v == Empty) {
    tree;
  } else {
    switch (tree) {
    | Empty => v
    | NFunc(f, _) => NFunc(f, v)
    | NInfix(op, a, _) => NInfix(op, a, v)
    | a =>
      raise(
        Js.Exn.raiseError(
          {j|Unmatched type of tree while inserting { $a ; $v }|j},
        ),
      )
    };
  };

let parse = tokens => {
  let rest = ref(tokens);

  let rec loop = (tree, state) => {
    let (token, tail) =
      switch (rest^) {
      | [t, ...ts] => (Some(t), ts)
      | [] => (None, [])
      };
    rest := tail;
    switch (tree, state, token) {
    // EOI,
    // 1: num
    | (tree, old_state, Some(Rune(Number(n)))) =>
      loop(tree, insert(old_state, Number(n)))
    // 1: var
    | (tree, old_state, Some(Rune(Name(v)))) =>
      loop(tree, insert(old_state, Variable(v)))
    // 2
    | (Empty, old_state, Some(Eval(Infix(op)))) =>
      loop(NInfix(op, old_state, Empty), Empty)
    // 3
    | (NInfix(old_op, _, _) as old_tree, old_state, Some(Eval(Infix(op))))
        when weight(op) > weight(old_op) =>
      loop(old_tree, NInfix(op, old_state, Empty))
    // 5
    | (NInfix(old_op, _, _) as old_tree, old_state, Some(Eval(Infix(op))))
        when weight(op) <= weight(old_op) =>
      loop(NInfix(op, old_tree->insert(old_state), Empty), Empty)
    // 6.1
    | (old_tree, old_state, Some(Special(Bracket(Open)))) =>
      loop(old_tree, old_state->insert(loop(Empty, Empty)))
    // 6.2 / End Of Input
    | (old_tree, old_state, Some(Special(Bracket(Close))) | None) =>
      old_tree->insert(old_state)->NVal
    // 7: inf
    | (old_tree, NInfix(op, a, b), Some(Eval(Function(f) | Suffix(f)))) =>
      loop(old_tree, NInfix(op, a, NFunc(f, b)))
    // 7
    | (old_tree, old_state, Some(Eval(Function(f) | Suffix(f)))) =>
      loop(old_tree, NFunc(f, old_state))
    | (t, s, k) =>
      raise(
        Js.Exn.raiseError(
          "Encountered an unmatched loop case: \n"
          ++ Helpers.deep(t)
          ++ "\n"
          ++ Helpers.deep(s)
          ++ "\n"
          ++ Helpers.deep(k),
        ),
      )
    };
  };
  loop(Empty, Empty);
};

let func_exp_of_string = f =>
  Belt.(
    Js.Re.exec_(Regex.re_function_exp, f)
    ->Option.map(r =>
        r
        ->Js.Re.captures
        ->Array.map(x => x |> Js.Nullable.toOption |> Option.getExn)
        ->Array.sliceToEnd(1)
      )
    ->Option.map(r =>
        switch (r) {
        | [|n, v, e|] => {name: n, var: v, exp: e->Lexer.lex->parse}
        | _ =>
          raise(
            Js.Exn.raiseError(
              {j|Error converting "$f" to a function expression|j},
            ),
          )
        }
      )
  );

let var_of_string = f =>
  Belt.(
    Js.Re.exec_(Regex.re_var_exp, f)
    ->Option.map(r =>
        r
        ->Js.Re.captures
        ->Array.map(x => x |> Js.Nullable.toOption |> Option.getExn)
        ->Array.sliceToEnd(1)
      )
    ->Option.map(r =>
        switch (r) {
        | [|n, e|] => {name: n, val_: e->Lexer.lex->parse}
        | _ =>
          raise(
            Js.Exn.raiseError(
              {j|Error converting "$f" to a function expression|j},
            ),
          )
        }
      )
  );