'use strict';

var React = require("react");

function Ui__App(Props) {
  var match = React.useReducer((function (param, action) {
          var o_pos = param[1];
          var o_inp = param[0];
          switch (action.tag | 0) {
            case /* IncPos */0 :
                return /* tuple */[
                        o_inp,
                        o_pos + action[0] | 0
                      ];
            case /* ConcatInput */1 :
                return /* tuple */[
                        o_inp + action[0],
                        o_pos
                      ];
            case /* ChangeInput */2 :
                return /* tuple */[
                        action[0],
                        o_pos
                      ];
            
          }
        }), /* tuple */[
        "",
        0
      ]);
  return React.createElement("div", undefined, React.createElement("p", {
                  className: "output"
                }, match[0][0]), React.createElement("input", {
                  id: "",
                  name: "",
                  type: "text"
                }));
}

var make = Ui__App;

exports.make = make;
/* react Not a pure module */
