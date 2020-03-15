open Store;
open Reductive;
open React;
let vars_selector = state => state.vars;

[@react.component]
let make = (~dispatch) => {
  let vars = useSelector(vars_selector);

  let onChange = (i, e) => {
    dispatch(UpdateVar(e->ReactEvent.Form.currentTarget##value, i));
  };

  let onRemove = (i, e) => {
    e->ReactEvent.Mouse.preventDefault;
    dispatch(RemoveVar(i));
  };
  <> <Lister content=vars onChange onRemove dispatch /> </>;
};