[@react.component]
let make = (~children) => {
  <section className="layout">
    <main className="layout__main"> children </main>
    <Nav />
  </section>;
};