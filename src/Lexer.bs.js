'use strict';

var List = require("bs-platform/lib/js/list.js");
var Block = require("bs-platform/lib/js/block.js");
var Js_exn = require("bs-platform/lib/js/js_exn.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Regex$CompilerTest = require("./Regex.bs.js");
var Evaluables$CompilerTest = require("./Evaluables.bs.js");

function lex(input) {
  var chars = Belt_Array.keepMap(input.split(Regex$CompilerTest.re_char_separator), (function (c) {
          if (c !== undefined) {
            var c$1 = c;
            if (c$1 === "") {
              return ;
            } else {
              return c$1;
            }
          }
          
        }));
  var _chars = Belt_List.fromArray(chars);
  var _state = undefined;
  var _t = /* [] */0;
  while(true) {
    var t = _t;
    var state = _state;
    var chars$1 = _chars;
    if (chars$1) {
      var next_char = List.hd(chars$1);
      var tail = List.tl(chars$1);
      var exit = 0;
      var exit$1 = 0;
      switch (next_char) {
        case "(" :
            if (state !== undefined) {
              exit$1 = 3;
            } else {
              _t = /* :: */[
                /* Special */Block.__(1, [/* Bracket */[/* Open */0]]),
                t
              ];
              _state = undefined;
              _chars = tail;
              continue ;
            }
            break;
        case ")" :
            if (state !== undefined) {
              exit$1 = 3;
            } else {
              _t = /* :: */[
                /* Special */Block.__(1, [/* Bracket */[/* Close */1]]),
                t
              ];
              _state = undefined;
              _chars = tail;
              continue ;
            }
            break;
        default:
          exit$1 = 3;
      }
      if (exit$1 === 3) {
        if (state !== undefined) {
          var n = state;
          switch (n.tag | 0) {
            case /* Eval */0 :
            case /* Special */1 :
                exit = 2;
                break;
            case /* Rune */2 :
                var match = n[0];
                if (match.tag) {
                  if (Regex$CompilerTest.is_num(next_char)) {
                    _state = /* Rune */Block.__(2, [/* Number */Block.__(1, [match[0] + next_char])]);
                    _chars = tail;
                    continue ;
                  } else if (Regex$CompilerTest.is_name(next_char)) {
                    _t = /* :: */[
                      /* Eval */Block.__(0, [/* Infix */Block.__(0, ["*"])]),
                      /* :: */[
                        n,
                        t
                      ]
                    ];
                    _state = /* Rune */Block.__(2, [/* Name */Block.__(0, [next_char])]);
                    _chars = tail;
                    continue ;
                  } else {
                    exit = 2;
                  }
                } else if (Regex$CompilerTest.is_name(next_char)) {
                  _state = /* Rune */Block.__(2, [/* Name */Block.__(0, [match[0] + next_char])]);
                  _chars = tail;
                  continue ;
                } else if (Regex$CompilerTest.is_num(next_char)) {
                  _t = /* :: */[
                    /* Eval */Block.__(0, [/* Infix */Block.__(0, ["^"])]),
                    /* :: */[
                      n,
                      t
                    ]
                  ];
                  _state = /* Rune */Block.__(2, [/* Number */Block.__(1, [next_char])]);
                  _chars = tail;
                  continue ;
                } else {
                  exit = 2;
                }
                break;
            
          }
        } else if (Regex$CompilerTest.is_num(next_char)) {
          _state = /* Rune */Block.__(2, [/* Number */Block.__(1, [next_char])]);
          _chars = tail;
          continue ;
        } else if (Regex$CompilerTest.is_name(next_char)) {
          _state = /* Rune */Block.__(2, [/* Name */Block.__(0, [next_char])]);
          _chars = tail;
          continue ;
        } else if (Evaluables$CompilerTest.is_op(next_char)) {
          _t = /* :: */[
            /* Eval */Block.__(0, [/* Infix */Block.__(0, [next_char])]),
            t
          ];
          _state = undefined;
          _chars = tail;
          continue ;
        } else if (Evaluables$CompilerTest.is_suffix(next_char)) {
          _t = /* :: */[
            /* Eval */Block.__(0, [/* Suffix */Block.__(2, [next_char])]),
            t
          ];
          _state = undefined;
          _chars = tail;
          continue ;
        }
        
      }
      if (exit === 2) {
        switch (next_char) {
          case "(" :
              var n$1 = state;
              var exit$2 = 0;
              switch (n$1.tag | 0) {
                case /* Eval */0 :
                case /* Special */1 :
                    exit$2 = 3;
                    break;
                case /* Rune */2 :
                    var match$1 = n$1[0];
                    if (match$1.tag) {
                      _t = /* :: */[
                        /* Special */Block.__(1, [/* Bracket */[/* Open */0]]),
                        /* :: */[
                          /* Eval */Block.__(0, [/* Infix */Block.__(0, ["*"])]),
                          /* :: */[
                            n$1,
                            t
                          ]
                        ]
                      ];
                      _state = undefined;
                      _chars = tail;
                      continue ;
                    } else {
                      _t = /* :: */[
                        /* Special */Block.__(1, [/* Bracket */[/* Open */0]]),
                        /* :: */[
                          /* Eval */Block.__(0, [/* Function */Block.__(1, [match$1[0]])]),
                          t
                        ]
                      ];
                      _state = undefined;
                      _chars = tail;
                      continue ;
                    }
                
              }
              if (exit$2 === 3) {
                _t = /* :: */[
                  /* Special */Block.__(1, [/* Bracket */[/* Open */0]]),
                  /* :: */[
                    n$1,
                    t
                  ]
                ];
                _state = undefined;
                _chars = tail;
                continue ;
              }
              break;
          case ")" :
              _t = /* :: */[
                /* Special */Block.__(1, [/* Bracket */[/* Close */1]]),
                /* :: */[
                  state,
                  t
                ]
              ];
              _state = undefined;
              _chars = tail;
              continue ;
          default:
            var tk = state;
            var exit$3 = 0;
            switch (tk.tag | 0) {
              case /* Eval */0 :
                  switch (tk[0].tag | 0) {
                    case /* Function */1 :
                        exit$3 = 3;
                        break;
                    case /* Infix */0 :
                    case /* Suffix */2 :
                        break;
                    
                  }
                  break;
              case /* Special */1 :
                  break;
              case /* Rune */2 :
                  exit$3 = 3;
                  break;
              
            }
            if (exit$3 === 3) {
              if (Evaluables$CompilerTest.is_op(next_char)) {
                _t = /* :: */[
                  /* Eval */Block.__(0, [/* Infix */Block.__(0, [next_char])]),
                  /* :: */[
                    tk,
                    t
                  ]
                ];
                _state = undefined;
                _chars = tail;
                continue ;
              } else if (Evaluables$CompilerTest.is_suffix(next_char)) {
                _t = /* :: */[
                  /* Eval */Block.__(0, [/* Suffix */Block.__(2, [next_char])]),
                  /* :: */[
                    tk,
                    t
                  ]
                ];
                _state = undefined;
                _chars = tail;
                continue ;
              }
              
            }
            
        }
      }
      throw Js_exn.raiseError("Encountered an unmatched loop case \"" + (String([
                      next_char,
                      state
                    ]) + "\""));
    } else if (state !== undefined) {
      _t = /* :: */[
        state,
        t
      ];
      _state = undefined;
      continue ;
    } else {
      return List.rev(t);
    }
  };
}

exports.lex = lex;
/* Regex-CompilerTest Not a pure module */
