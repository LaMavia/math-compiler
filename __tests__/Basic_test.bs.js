'use strict';

var Jest = require("@glennsl/bs-jest/src/jest.js");
var Calc$CompilerTest = require("../src/Calc.bs.js");

Jest.describe("Basic equations", (function (param) {
        Jest.test("Addidion", (function (param) {
                return Jest.Expect.toBe(10, Jest.Expect.expect(Calc$CompilerTest.calculate("2+3 + 5", /* [] */0, [])));
              }));
        Jest.test("Subtraction", (function (param) {
                return Jest.Expect.toBe(-4, Jest.Expect.expect(Calc$CompilerTest.calculate("2-5 - 1", /* [] */0, [])));
              }));
        Jest.testAll("Add & Sub", /* :: */[
              /* tuple */[
                "[-12 + 5 - 3 + 2]",
                -8
              ],
              /* :: */[
                /* tuple */[
                  "[-12 + (-1) + 5 - 1]",
                  -9
                ],
                /* [] */0
              ]
            ], (function (pr) {
                var a = pr[0];
                var b = pr[1];
                return Jest.Expect.toBe(b, Jest.Expect.expect(Calc$CompilerTest.calculate(a, /* [] */0, [])));
              }));
        Jest.test("Multiplication", (function (param) {
                return Jest.Expect.toBe(-60, Jest.Expect.expect(Calc$CompilerTest.calculate("2*3 * 5 * (-2)", /* [] */0, [])));
              }));
        Jest.test("Division", (function (param) {
                return Jest.Expect.toBe(0.1, Jest.Expect.expect(Calc$CompilerTest.calculate("-2/5/(-4)", /* [] */0, [])));
              }));
        Jest.testAll("Mult & Div", /* :: */[
              /* tuple */[
                "[-5*2 / 6 * 2/(-1)]",
                10 / 3
              ],
              /* :: */[
                /* tuple */[
                  "[-1*5*7/4]",
                  -8.75
                ],
                /* [] */0
              ]
            ], (function (pr) {
                var a = pr[0];
                var b = pr[1];
                return Jest.Expect.toBe(b, Jest.Expect.expect(Calc$CompilerTest.calculate(a, /* [] */0, [])));
              }));
        Jest.testAll("Exponentiation", /* :: */[
              /* tuple */[
                "(2^((-1)*(-3)))^3",
                512
              ],
              /* :: */[
                /* tuple */[
                  "(-2) ^ 2 + 7^3",
                  347
                ],
                /* [] */0
              ]
            ], (function (param) {
                return Jest.Expect.toBe(param[1], Jest.Expect.expect(Calc$CompilerTest.calculate(param[0], /* [] */0, [])));
              }));
        Jest.testAll("Roots", /* :: */[
              /* tuple */[
                "5_(-1)",
                -1
              ],
              /* :: */[
                /* tuple */[
                  "2_2",
                  Math.sqrt(2)
                ],
                /* :: */[
                  /* tuple */[
                    "2_4",
                    2
                  ],
                  /* :: */[
                    /* tuple */[
                      "2_2^2",
                      2
                    ],
                    /* :: */[
                      /* tuple */[
                        "(-1)_2",
                        0.5
                      ],
                      /* [] */0
                    ]
                  ]
                ]
              ]
            ], (function (param) {
                return Jest.Expect.toBeCloseTo(param[1], Jest.Expect.expect(Calc$CompilerTest.calculate(param[0], /* [] */0, [])));
              }));
        return Jest.testAll("Mixed Operations", /* :: */[
                    /* tuple */[
                      "(10*5^2/(88+ 4 root 2)-3)",
                      -0.196970260340537025873220
                    ],
                    /* [] */0
                  ], (function (param) {
                      return Jest.Expect.toBeCloseTo(param[1], Jest.Expect.expect(Calc$CompilerTest.calculate(param[0], /* [] */0, [])));
                    }));
      }));

/*  Not a pure module */
