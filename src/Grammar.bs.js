'use strict';


function weight(op) {
  switch (op) {
    case "+" :
    case "-" :
        return 0;
    case "*" :
    case "/" :
        return 1;
    case "^" :
    case "_" :
        return 2;
    default:
      console.log("" + (String(op) + ""));
      return 3;
  }
}

exports.weight = weight;
/* No side effect */
