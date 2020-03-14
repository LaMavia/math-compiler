let nthroot ~n ~a ?(tol = 0.001) () =
  let f_n = float n in
  let f_n' = f_n -. 1.0 in
  let rec loop x =
    let x' = ((f_n' *. x) +. (a /. (x ** f_n'))) /. f_n in
    if tol > abs_float (x -. x') then x' else loop x'
  in
  loop 1.0
;;

let root n a =
  if n > 0.
  then nthroot ~n:(int_of_float n) ~a ()
  else if n < 0.
  then a ** (1.0 /. n)
  else
    raise
      (Js.Exn.raiseEvalError "Are you really trying to calc a 0-deg root? Dios mío...")
;;
