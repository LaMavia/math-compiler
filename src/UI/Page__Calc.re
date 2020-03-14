open Store;
open React;
let calc_selector = state => (state.input, state.ans);

type action_button = {
  content: string,
  onClick: ReactEvent.Mouse.t => unit,
  size: float,
};

type button = {
  main: string,
  alt: string,
  size: float,
};

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

[@react.component]
let make = (~dispatch) => {
  let ((mode, offset), local_dispatch) =
    useReducer(
      ((m, o), a) =>
        switch (a) {
        | ChangeMode => (m == `main ? `alt : `main, o)
        | IncOffset(n) => (m, o + n)
        | ChangeOffset(n) => (m, n)
        | _ => (m, o)
        },
      (`main, 1),
    );
  let (input, ans) = useSelector(calc_selector);

  let action_buttons: array(array(action_button)) = [|
    [|
      {
        content: "del",
        onClick: _ => {
          dispatch(Del(offset));
        },
        size: 1.,
      },
      {
        content: "ac",
        onClick: _ => {
          dispatch(ChangeInput(""));
        },
        size: 1.0,
      },
      {
        content: "prev",
        onClick: _ => {
          (); // PASS
        },
        size: 1.5,
      },
      {
        content: "next",
        onClick: _ => {
          (); // PASS
        },
        size: 1.5,
      },
      {
        content: "=",
        onClick: _ => {
          dispatch(Calc);
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
        content: "base",
        onClick: _ => (), // PASS
        size: 1.5,
      },
    |],
  |];
  let buttons: array(array(button)) = [||];
  <form onSubmit={e => {e->ReactEvent.Form.preventDefault}} className="calc">
    <div className="calc__top">
      <input
        type_="text"
        name="input"
        id="input"
        value=input
        className="calc_top_eq"
        onChange={e => {
          e->ReactEvent.Form.currentTarget##value->ChangeInput->dispatch
        }}
      />
      <p className="calc__top__output"> {ans->string_of_ans->string} </p>
    </div>
    <ul className="calc__actions">
      {action_buttons
       ->Belt.Array.map(row => {
           <li className="calc__actions__row">
             <ul className="calc__actions__row__items">
               {row
                ->Belt.Array.map(btn => {
                    <li
                      style=ReactDOMRe.Style.(
                        make()
                        ->unsafeAddProp("--size", btn.size->Js.Float.toString)
                      )
                      className="calc__actions__row__items__btn">
                      <button
                        onClick={btn.onClick}
                        className="calc__actions__row__items__btn__key">
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
       ->Belt.Array.map(row => {
           <li className="calc__keys__row">
             <ul className="calc__keys__row__items">
               {row
                ->Belt.Array.map(b =>
                    <li className="calc__keys__row__items__key">
                      (mode == `main ? b.main : b.alt)->string
                    </li>
                  )
                ->array}
             </ul>
           </li>
         })
       ->array}
    </ul>
  </form>;
};