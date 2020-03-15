let trash = [%raw {|require("@public/svg/trash.svg")|}];

type name = [ | `trash];

[@react.component]
let make = (~name, ~className="") => {
  let logo =
    switch (name) {
    | `trash => trash
    };
  <div className dangerouslySetInnerHTML={"__html": logo} />;
};