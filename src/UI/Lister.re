open Store;
open React;

[@react.component]
let make = (~dispatch, ~onChange, ~onRemove, ~content) => {

  <ol className="vars">
    {content
     ->Belt.List.toArray
     ->Belt.Array.mapWithIndex((i, var) => {
         <li key={string_of_int(i)} className="vars__var">
           <button className="vars__var__remove" onClick={onRemove(i)}>
             <Icon className="vars__var__remove__icon" name=`trash />
           </button>
           <input
             type_="text"
             value=var
             name={j|var#$i|j}
             id={j|var#$i|j}
             className="vars__var__exp"
             onChange={onChange(i)}
           />
           <div className="vars__var__movers">
             <button
               onClick={_ => dispatch(MoveVar(i, -1))}
               className="vars__var__movers__btn">
               "<"->string
             </button>
             <button
               onClick={_ => dispatch(MoveVar(i, 1))}
               className="vars__var__movers__btn">
               ">"->string
             </button>
           </div>
         </li>
       })
     ->array}
  </ol>;
};