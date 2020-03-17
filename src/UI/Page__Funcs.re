open Store;
let funcs_selector = state => state.funcs;

[@react.component]
let make = (~dispatch) => {
  let funcs = useSelector(funcs_selector);

  let onChange = (i, e) => {
    dispatch(UpdateFunc(e->ReactEvent.Form.currentTarget##value, i));
  };

  let onRemove = (i, e) => {
    e->ReactEvent.Mouse.preventDefault;
    dispatch(RemoveFunc(i));
  };
  <> <Lister content=funcs onChange onRemove dispatch /> </>;
};