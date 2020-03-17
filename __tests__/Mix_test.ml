open Jest
open Expect
open! Expect.Operators

let vars_of_exps exps =
  exps
  |> List.map (fun z -> Parser.var_of_string z)
  |> List.filter (fun a ->
         match a with
         | Some _ -> true
         | None -> false)
  |> List.map (fun x ->
         match x with
         | Some x -> x
         | None -> assert false)
;;

let () =
  describe "mix" (fun () ->
      testAll
        "2 dependent vars"
        [ ( "x=3^2"
          , "y=2^x-1"
          , "sin(21 + 2_y + x) mod 5"
          , 0.7185853557574577940219162339565178339 )
        ; ( "x=2_7"
          , "y=3^x-1"
          , "acos((2x * 2y2) mod 1)^2 mod 3"
          , 0.5473689269000994902219914 )
        ; "x=2", "y = 2x", "y choose x mod 15 + x^2 - y", 6.0
        ]
        (fun (x, y, e, r) ->
          expect (Calc.calculate e (vars_of_exps [ x; y ]) [||]) |> toBeCloseTo r))
;;
