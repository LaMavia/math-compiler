type url = {
  path: list(string),
  hash: string,
  search: string,
};

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let dispatch = Store.useDispatch();

  let path_len = url.path->List.length;
  let path =
    if (path_len > 1) {
      url.path->List.tl;
    } else if (path_len == 0) {
      [];
    } else {
      [url.path->List.hd];
    };

  <Layout>
    {switch (path) {
     | [] => <Page__Calc dispatch />
     | ["vars"] => <Page__Vars dispatch /> 
     | ["funcs"] => <Page__Funcs dispatch /> 
     | x =>
       x->Helpers.deep_log;
       <Page__Calc dispatch />;
     }}
  </Layout>;
};