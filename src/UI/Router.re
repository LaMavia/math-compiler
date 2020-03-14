type url = {
  path: list(string),
  hash: string,
  search: string,
};

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let dispatch = Store.useDispatch();

  <Layout>
    {switch (url.path) {
     | [] => <Page__Calc dispatch />
     }}
  </Layout>;
};