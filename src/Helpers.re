[@bs.module "util"] external inspect: ('a, Js.t({..})) => 'b = "inspect";
let deep = x => inspect(x, {"depth": 50, "colors": true});
let do_log = true;
let deep_log = x => do_log ? Js.Console.log(deep(x)) : ();