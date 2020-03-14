type evaluable =
  | Infix(string)
  | Function(string)
  | Suffix(string);

type rune =
  | Name(string)
  | Number(string);

type special =
  | Bracket(bracket)
  // | Absolute(bracket)
  // | Assign
and bracket =
  | Open
  | Close;

type token =
  | Eval(evaluable)
  | Special(special)
  | Rune(rune);

type node =
  | Empty
  | Number(string)
  | Variable(string)
  | /**Holds a single value */
    NVal(node)
  | /**Holds the name of the function/suffix_op to be evaluated on the node */
    NFunc(
      string,
      node,
    )
  | /**Holds the name of the infix operator to be used on the nodes provided */
    NInfix(
      string,
      node,
      node,
    );

let weight = op =>
  switch (op) {
  | "+"
  | "-" => 0
  | "*"
  | "/" => 1
  | "^"
  | "_" => 2
  | a =>
    Js.Console.log({j|$a|j});
    3;
  };