open Store;
open React;
let calc_selector = state => (state.input, state.ans, state.base);

type action_button = {
  content: string,
  onClick: ReactEvent.Mouse.t => unit,
  size: float,
};

type button = (string, string);

let string_of_ans = ans =>
  switch (ans) {
  | Grammar.Number(n) => n
  | _ => raise(Js.Exn.raiseError({j|Unknown answer type: $ans|j}))
  };

type mode = [ | `main | `alt];

type mode_action =
  | ChangeMode
  | IncOffset(int)
  | ChangeOffset(int);

type input = [ | `var | `func | `exp];

[@react.component]
let make = (~dispatch) => {
  let (input, ans, base) = useSelector(calc_selector);
  let ((mode, offset), local_dispatch) =
    useReducer(
      ((m, o), a) =>
        switch (a) {
        | ChangeMode => (m == `main ? `alt : `main, o)
        | IncOffset(n) =>
          let len = input->Js.String2.length;
          let o' = o + n;
          (m, o' > len - 1 || o' < 0 ? o : o');
        | ChangeOffset(n) => (m, n)
        | _ => (m, o)
        },
      (`main, 0),
    );

  let (type_of_input, eq_btn_display) =
    if (input->Regex.is_var_exp) {
      (`var, "+var");
    } else if (input->Regex.is_function_exp) {
      (`func, "+f");
    } else {
      (`exp, "=");
    };

  let (base_n, base_display) = Store.set_of_base(base);

  let action_buttons: array(array(action_button)) = [|
    [|
      {
        content: "del",
        onClick: _ => {
          dispatch(Del(offset));
          let len = input->String.length;
          if (offset >= len) {
            local_dispatch(ChangeOffset(len - 1 >= 0 ? len - 1 : 0));
          };
        },
        size: 1.,
      },
      {
        content: "ac",
        onClick: _ => {
          dispatch(ChangeInput(""));
          local_dispatch(ChangeOffset(0));
        },
        size: 1.0,
      },
      {
        content: "prev",
        onClick: _ => {
          dispatch(LoadHist(1));
        },
        size: 1.5,
      },
      {
        content: "next",
        onClick: _ => {
          dispatch(LoadHist(-1));
        },
        size: 1.5,
      },
      {
        content: eq_btn_display,
        onClick: _ => {
          switch (type_of_input) {
          | `var => dispatch(AddVar(input))
          | `func => dispatch(AddFunc(input))
          | `exp => dispatch(Calc)
          };
        },
        size: 4.,
      },
    |],
    [|
      {
        content: "<",
        onClick: _ => {
          local_dispatch(IncOffset(1));
        },
        size: 1.,
      },
      {
        content: ">",
        onClick: _ => {
          local_dispatch(IncOffset(-1));
        },
        size: 1.,
      },
      {
        content: "alt",
        onClick: _ => {
          local_dispatch(ChangeMode);
        },
        size: 1.,
      },
      {
        content: base_display,
        onClick: _ => {
          dispatch(ChangeBase);
        },
        size: 1.5,
      },
    |],
  |];
  let buttons: array(array(button)) = [|
    [|
      ("7", "x"),
      ("8", "v"),
      ("9", "t"),
      ("+", "sin("),
      ("-", "asin("),
    |],
    [|
      ("4", "y"),
      ("5", "a"),
      ("6", "r"),
      ("*", "cos("),
      ("/", "acos("),
    |],
    [|
      ("1", "pi"),
      ("2", "e"),
      ("3", "kb"),
      ("^", "tan("),
      ("_", "atan("),
    |],
    [|
      (".", "="),
      ("0", "R"),
      ("ans", "Na"),
      ("(", "!"),
      (")", " mod "),
    |],
  |];
  <form onSubmit={e => {e->ReactEvent.Form.preventDefault}} className="calc">
    <div className="calc__top">
      <input
        type_="text"
        name="input"
        id="input"
        value=input
        className="calc__top__eq"
        onChange={e => {
          e->ReactEvent.Form.currentTarget##value->ChangeInput->dispatch
        }}
      />
      <p className="calc__top__output">
        {ans
         ->string_of_ans
         ->float_of_string
         ->Js.Float.toStringWithRadix(~radix=base_n)
         ->string}
      </p>
    </div>
    <ul className="calc__keys calc__keys--actions">
      {action_buttons
       ->Belt.Array.mapWithIndex((i, row) => {
           <li key={string_of_int(i)} className="calc__keys__row">
             <ul className="calc__keys__row__items">
               {row
                ->Belt.Array.mapWithIndex((i, btn) => {
                    <li
                      key={string_of_int(i)}
                      style=ReactDOMRe.Style.(
                        make()
                        ->unsafeAddProp("--size", btn.size->Js.Float.toString)
                      )
                      className="calc__keys__row__items__key">
                      <button
                        onClick={btn.onClick}
                        className="calc__keys__row__items__key__btn calc__keys__row__items__key__btn--action">
                        btn.content->string
                      </button>
                    </li>
                  })
                ->array}
             </ul>
           </li>
         })
       ->array}
    </ul>
    <ul className="calc__keys">
      {buttons
       ->Belt.Array.mapWithIndex((i, row) => {
           <li key={string_of_int(i)} className="calc__keys__row">
             <ul className="calc__keys__row__items">
               {row
                ->Belt.Array.mapWithIndex((i, (m, a)) => {
                    let content = mode == `main ? m : a;
                    <li
                      key={string_of_int(i)}
                      className="calc__keys__row__items__key">
                      <button
                        onClick={e => {
                          e->ReactEvent.Mouse.preventDefault;
                          dispatch(ConcatInput(content, offset));
                        }}
                        className="calc__keys__row__items__key__btn calc__keys__row__items__key__btn--board">
                        {content->Js.String2.trim->string}
                      </button>
                    </li>;
                  })
                ->array}
             </ul>
           </li>
         })
       ->array}
    </ul>
  </form>;
};