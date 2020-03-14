open ReasonReact;
[@react.component]
let make = () => {
  <nav className="nav">
    <Link href="/"> "calc"->string </Link>
    <Link href="/vars"> "vars"->string </Link>
    <Link href="/funcs"> "funcs"->string </Link>
  </nav>;
};