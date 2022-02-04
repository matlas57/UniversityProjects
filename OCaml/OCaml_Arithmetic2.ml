(* Reminder: If a test case requires multiple arguments, use a tuple:
let myfn_with_2_args_tests = [
  ((arg1, arg1), (expected_output))
]
*)

(* Q1 *)
(* TODO: Write a good set of tests for compress *)
let compress_tests = [
  ( [A; A; A; A; G; G; A; T; T; T; C; T; C], [(4, A); (2, G); (1, A); (3, T); (1, C); (1, T); (1, C)]);
  ( [], []);
  ( [A], [(1, A)] );
]

(* TODO: Implement compress. *)
let compress (l : nucleobase list) : (int * nucleobase) list =
  let rec cmpr l counter = 
    match l with 
    | [] -> []
    | x :: [] -> [(counter, x)]
    | a :: (b :: t as r) -> if a = b then cmpr r (counter + 1) 
        else [(counter, a)] @ cmpr r 1
  in 
  match l with 
  | [] -> []
  | x :: xs -> cmpr (x :: xs) 1
  
    (* implement recurrsively with helper function
   traverse the list and pattern match the head
   - if empty list return empty list
   - if matches the previous element then increment counter
   - if its any other nucleobase append the last (int * nucleobase) and start a new pair
*) 

(* TODO: Write a good set of tests for decompress *)
let decompress_tests = [
  ([(4, A); (2, G); (1, A); (3, T); (1, C); (1, T); (1, C)], [A; A; A; A; G; G; A; T; T; T; C; T; C]);
  ([], []);
  ([(1, A)], [A]);  
]

(* TODO: Implement decompress. *)
let rec decompress (l : (int * nucleobase) list) : nucleobase list = 
  match l with 
  | [] -> []
  | (1, base) :: t -> base :: (decompress t)
  | (int, base) :: t -> base :: (decompress ([(int - 1, base)] @ t))
                             
        
    
    (* implement recurrsively
   traverse the list and match the head
   - if empty return empty
   - if any nucleobase decrement the int and call again  
*)


(* Q2 *)
(* TODO: Write a good set of tests for eval *)
let eval_tests = [
  (MULT (PLUS (FLOAT 2.2, FLOAT 3.3), FLOAT 5.0), 27.5);
  (MINUS (FLOAT 5.9, FLOAT 2.3), 3.6);
  (SIN (FLOAT 3.2), sin 3.2);
  (FLOAT 3.6, 3.6);
  (EXP (FLOAT 2.0), exp 2.0); 
]

(* TODO: Implement eval. *)
let rec eval e =
  match e with 
  | FLOAT f -> f
                 (*| PLUS (FLOAT p1, FLOAT p2) -> p1 +. p2 *)
  | PLUS (e1, e2) -> (eval e1) +. (eval e2)
                       (*| MINUS (FLOAT p1, FLOAT p2) -> p1 -. p2*)
  | MINUS (e1, e2) -> (eval e1) -. (eval e2)
                        (*| MULT (FLOAT p1, FLOAT p2) -> p1 *. p2*)
  | MULT (e1, e2) -> (eval e1) *. (eval e2)
                       (*| DIV (FLOAT p1, FLOAT p2) -> p1 /. p2*)
  | DIV (e1, e2) -> (eval e1) /. (eval e2)
                      (*| SIN (FLOAT p) -> sin p*)
  | SIN (e) -> sin (eval e)
                 (*| COS (FLOAT p) -> cos p *)
  | COS (e) -> cos (eval e)
                 (*| EXP (FLOAT p) -> exp p *)
  | EXP (e) -> exp (eval e)
  
(* TODO: Write a good set of tests for to_instr *)
let to_instr_tests = [
  ((MULT (PLUS (FLOAT 2.2, FLOAT 3.3), FLOAT 5.0)), [Float 2.2; Float 3.3; Plus; Float 5.; Mult]);
  ((SIN (FLOAT 3.2)), [Float 3.2; Sin]);
  (FLOAT 4.5, [Float 4.5]);
]

(* TODO: Implement to_instr. *)
let rec to_instr e = 
  match e with 
  | FLOAT f -> [Float f]
  | PLUS (e1, e2) ->  to_instr e1 @ to_instr e2 @ [Plus]
  | MINUS (e1, e2) ->  to_instr e1 @ to_instr e2 @ [Minus]
  | MULT (e1, e2) ->  to_instr e1 @ to_instr e2 @ [Mult]
  | DIV (e1, e2) ->  to_instr e1 @ to_instr e2 @ [Div]
  | SIN (e) -> to_instr e @ [Sin]
  | COS (e) -> to_instr e @ [Cos]
  | EXP (e) -> to_instr e @ [Exp] 
                            
(* TODO: Write a good set of tests for instr *)
let instr_tests = [
  ((Mult, [5.0; 5.5]), Some [27.5]);
  ((Sin , [3.4]), Some [sin 3.4]);
  (((Float 4.2), [2.2; 3.3; 5.5]), Some [4.2; 2.2; 3.3; 5.5] );
]


(* TODO: Implement to_instr. *)               
let instr i s = 
  match i with 
  | Plus -> (match s with 
      | [] | _ :: [] -> None
      | a :: (b :: t) -> Some ([a +. b] @ t))
  | Minus -> (match s with 
      | [] | _ :: [] -> None
      | a :: (b :: t) -> Some ([b -. a] @ t))
  | Mult -> (match s with 
      | [] | _ :: [] -> None
      | a :: (b :: t) -> Some ([a *. b] @ t))
  | Div -> (match s with 
      | [] | _ :: [] -> None
      | a :: (b :: t) -> Some ([b /. a] @ t))
  | Sin -> (match s with
      | [] -> None
      | a :: t -> Some ([sin a] @ t))
  | Cos -> (match s with
      | [] -> None
      | a :: t -> Some ([cos a] @ t))
  | Exp -> (match s with
      | [] -> None
      | a :: t -> Some ([exp a] @ t))
  | Float f -> Some ([f] @ s)


(* TODO: Write a good set of tests for prog *)
let prog_tests = [
  ([Float 2.2; Float 3.3; Plus; Float 5.; Mult], Some 27.5);
  ([Float 3.4 ; Sin], Some (sin 3.4));
]

(* TODO: Implement prog. *)
let prog instrs = 
  raise NotImplemented
    (*
let rec make_stack i_list = 
  match i_list with 
  | [] -> []
  | Float f :: t -> [f] @ (make_stack t)
  | _ :: t -> make_stack t
in 
let rec make_instr_list i_list = 
  match i_list with 
  | [] -> []
  | Float f :: t -> make_instr_list t
  | x :: t -> [x] @ make_instr_list t
in 
let rec evaluate i_list stk = 
  match i_list with 
  *)
                     
