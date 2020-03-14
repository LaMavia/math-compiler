type action =
  | IncPos(int)
  | ConcatInput(string)
  | ChangeInput(string);

[@react.component]
let make = () => {
  let ((inp, pos), dispatch) =
    React.useReducer(
      ((o_inp, o_pos), action) =>
        switch (action) {
        | IncPos(n) => (o_inp, o_pos + n)
        | ConcatInput(str) => (o_inp ++ str, o_pos)
        | ChangeInput(inp) => (inp, o_pos)
        },
      ("", 0),
    );
  <div>
    <p className="output"> inp->React.string </p>
    <input type_="text" name="" id="" />
  </div>;
};