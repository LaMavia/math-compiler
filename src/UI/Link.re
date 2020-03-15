[@react.component]
let make = (~children, ~href, ~className="") => {
  let base_url =
    if (Env.node_env == "production") {
      "/math-compiler";
    } else {
      "";
    };
  <a
    href
    onClick={e => {
      e->ReactEvent.Mouse.preventDefault;
      ReasonReactRouter.push(base_url ++ href);
    }}
    className>
    children
  </a>;
};