open Jest
open Expect
open! Expect.Operators

let () =
  describe "Basic equations" (fun () ->
      (* Weight 0 operations *)
      test "Addidion" (fun () ->
          let eq = "2+3 + 5"
          and exp_res = 10. in
          expect (Calc.calculate eq [] [||]) |> toBe exp_res);
      test "Subtraction" (fun () ->
          let eq = "2-5 - 1"
          and exp_res = -4. in
          expect (Calc.calculate eq [] [||]) |> toBe exp_res);
      testAll
        "Add & Sub"
        [ "[-12 + 5 - 3 + 2]", -8.; "[-12 + (-1) + 5 - 1]", -9. ]
        (fun pr ->
          let a = fst pr
          and b = snd pr in
          expect (Calc.calculate a [] [||]) |> toBe b);
      (* Weight 1 operations *)
      test "Multiplication" (fun () ->
          let eq = "2*3 * 5 * (-2)"
          and exp_res = -60. in
          expect (Calc.calculate eq [] [||]) |> toBe exp_res);
      test "Division" (fun () ->
          let eq = "-2/5/(-4)"
          and exp_res = 0.1 in
          expect (Calc.calculate eq [] [||]) |> toBe exp_res);
      testAll
        "Mult & Div"
        [ "[-5*2 / 6 * 2/(-1)]", 10. /. 3.; "[-1*5*7/4]", -8.75 ]
        (fun pr ->
          let a = fst pr
          and b = snd pr in
          expect (Calc.calculate a [] [||]) |> toBe b);
      (* Weight 2 operations *)
      testAll
        "Exponentiation"
        [ "(2^((-1)*(-3)))^3", 512.; "(-2) ^ 2 + 7^3", 347. ]
        (fun (a, b) -> expect (Calc.calculate a [] [||]) |> toBe b);
      testAll
        "Roots"
        [ "5_(-1)", -1.; "2_2", sqrt 2.; "2_4", 2.; "2_2^2", 2.; "(-1)_2", 0.5 ]
        (fun (a, b) -> expect (Calc.calculate a [] [||]) |> toBeCloseTo b);
      testAll
        "Mixed Operations"
        [ "(10*5^2/(88+ 4 root 2)-3)", -0.196970260340537025873220 ]
        (fun (a, b) -> expect (Calc.calculate a [] [||]) |> toBeCloseTo b))
;;
