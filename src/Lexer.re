open Js.String2;
open Regex;
open Grammar;
open Evaluables;

let lex = input => {
  let chars =
    input
    ->splitByRe(re_char_separator)
    ->Belt.Array.keepMap(c =>
        switch (c) {
        | Some("") => None
        | Some(c) => Some(c)
        | _ => None
        }
      );

  let rec loop = (chars, state, t) => {
    switch (chars, state) {
    | ([], None) => List.rev(t)
    | ([], Some(a)) => loop(chars, None, [a, ...t])
    | _ =>
      let next_char = List.hd(chars);
      let tail = List.tl(chars);
      let next = loop(tail);

      switch (next_char, state) {
      | ("(" | "[", None) => next(None, [Special(Bracket(Open)), ...t])
      | (")" | "]", None) => next(None, [Special(Bracket(Close)), ...t])

      // state: None
      | (a, None) when is_num(a) => next(Some(Rune(Number(a))), t)
      | (a, None) when is_name(a) => next(Some(Rune(Name(a))), t)
      | (a, None) when is_op(a) => next(None, [Eval(Infix(a)), ...t])
      | (a, None) when is_suffix(a) =>
        next(None, [Eval(Suffix(a)), ...t])

      // state: Number
      | (a, Some(Rune(Number(n)))) when is_num(a) =>
        next(Some(Rune(Number(n ++ a))), t)
      | (a, Some(Rune(Number(_)) as n)) when is_name(a) =>
        next(Some(Rune(Name(a))), [Eval(Infix("*")), n, ...t])
      | ("(" | "[", Some(Rune(Number(_)) as n)) =>
        next(None, [Special(Bracket(Open)), Eval(Infix("*")), n, ...t])

      // state: Name
      | (a, Some(Rune(Name(n)))) when is_name(a) =>
        let op' = n ++ a;
        if (is_op(op')) {
          next(None, [Eval(Infix(op')), ...t]);
        } else {
          next(Some(Rune(Name(n ++ a))), t);
        };
      | (a, Some(Rune(Name(n_raw)) as n)) when is_num(a) =>
        if (is_op(n_raw)) {
          next(Some(Rune(Number(a))), [Eval(Infix(n_raw)), ...t]);
        } else {
          next(Some(Rune(Number(a))), [Eval(Infix("^")), n, ...t]);
        }
      | ("(" | "[", Some(Rune(Name(n)))) =>
        next(None, [Special(Bracket(Open)), Eval(Function(n)), ...t])

      // Bracket and then some
      | ("(" | "[", Some(tk)) =>
        next(None, [Special(Bracket(Open)), tk, ...t])
      | (")" | "]", Some(tk)) =>
        next(None, [Special(Bracket(Close)), tk, ...t])

      // Operator and then some
      | (a, Some((Eval(Function(_)) | Rune(Number(_) | Name(_))) as tk))
          when is_op(a) =>
        next(None, [Eval(Infix(a)), tk, ...t])
      | (a, Some((Eval(Function(_)) | Rune(Number(_) | Name(_))) as tk))
          when is_suffix(a) =>
        next(None, [Eval(Suffix(a)), tk, ...t])
      | x =>
        raise(
          Js.Exn.raiseError({j|Encountered an unmatched loop case "$x"|j}),
        )
      };
    };
  };

  loop(chars->Belt.List.fromArray, None, []);
};