open ReasonReact;
[@react.component]
let make = () => {
  <nav className="nav">
    <Link className="nav__link" href="/"> "2+2"->string </Link>
    <Link className="nav__link" href="/vars"> "x=5"->string </Link>
    <Link className="nav__link" href="/funcs"> "f(x)"->string </Link>
  </nav>;
};