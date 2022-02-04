(* TODO: Write a good set of tests for unused_vars. *)
let unused_vars_tests = [
  (* An example test case.
     Note that you are *only* required to write tests for Rec, Fn, and Apply!
  *)
  (Let ("x", I 1, I 5), ["x"]);
  (Rec ("x", Bool, Var "x"), []);
  (Rec ("x", Int, (If (B true, I 5, I 4))), ["x"]); 
  (Fn
     ([("x", Int); ("y", Int)],
      Primop (Plus,
              [Primop (Times, [Var "x"; Var "x"]);
               Primop (Times, [Var "y"; Var "y"])])), []);
  (Fn (["x", Int], (If (B true, I 5, I 4))), ["x"]);
  (Apply (Fn (["x", Int], (If (B true, I 5, I 4))), ([I 1; I 2])), ["x"])
]

(* TODO: Implement the missing cases of unused_vars. *)
let rec unused_vars =
  function
  | Var _ | I _ | B _ -> []
  | If (e, e1, e2) -> unused_vars e @ unused_vars e1 @ unused_vars e2
  | Primop (_, args) ->
      List.fold_left (fun acc exp -> acc @ unused_vars exp) [] args
  | Let (x, e1, e2) ->
      let unused = unused_vars e1 @ unused_vars e2 in
      if List.mem x (free_variables e2) then
        unused
      else
        x :: unused

  | Rec (x, _, e) -> 
      let unused = unused_vars e in
      if List.mem x (free_variables e) then
        unused
      else x :: unused

  | Fn (xs, e) -> 
      let unused = unused_vars e in
      let rec find_xs xs = match xs with
        | [] -> []
        | (var, _) :: xs -> if List.mem var (free_variables e) then
              find_xs xs
            else var :: find_xs xs
      in unused @ find_xs xs
      

  | Apply (e, es) -> 
      let unused = unused_vars e in
      let rec unused_es es = match es with 
        | [] -> []
        | e :: es -> unused_vars e @ unused_es es
      in unused @ unused_es es

(* TODO: Write a good set of tests for subst. *)
(* Note: we've added a type annotation here so that the compiler can help
   you write tests of the correct form. *)
let subst_tests : (((exp * name) * exp) * exp) list = [
  (* An example test case. If you have trouble writing test cases of the
     proper form, you can try copying this one and modifying it.
     Note that you are *only* required to write tests for Rec, Fn, and Apply!
  *)
  (((I 1, "x"), (* [1/x] *)
    (* let y = 2 in y + x *)
    Let ("y", I 2, Primop (Plus, [Var "y"; Var "x"]))),
   (* let y = 2 in y + 1 *)
   Let ("y", I 2, Primop (Plus, [Var "y"; I 1])));
  
  (((I 1, "x"),
    Rec ("y", Int, Primop (Plus, [Var "y"; Var "x"]))),
   Rec("y", Int, Primop (Plus, [Var "y"; I 1])));
  
  (((I 1, "x"),
    Rec ("x", Int, Primop (Plus, [Var "x"; Var "w"]))),
   Rec("x", Int, Primop (Plus, [Var "x"; Var "w"])));
  
  ((( Var "y", "x"),
    Rec ("y", Int, Primop (Plus, [Var "y"; Var "x"]))),
   Rec ("w", Int, Primop (Plus, [Var "w"; Var "y"])));
  
  (((I 1, "x"),
    Fn (["y", Int], (If (B true, Var "y", Var "x")))),
   Fn (["y", Int], (If (B true, Var "y", I 1))));
  
  (((I 1, "x"),
    Fn (["x", Int], (If (B true, Var "y", Var "x")))),
   Fn (["x", Int], (If (B true, Var "y", Var "x" ))));
  
  ((( Var "y", "x"),
    Fn (["y", Int], (If (B true, Var "y", Var "x")))),
   Fn (["w", Int], (If (B true, Var "w", Var "y" ))));
   
  (((I 1, "x"),
    (Apply (Fn (["y", Int], (If (B true, I 1, I 4))), ([Var "x"; I 2])))),
   (Apply (Fn (["y", Int], (If (B true, I 1, I 4))), ([I 1; I 2]))));
  
  (((I 1, "x"),
    (Apply (Fn (["y", Int], (If (B true, I 1, I 4))), ([Var "x"; Var "x"])))),
   (Apply (Fn (["y", Int], (If (B true, I 1, I 4))), ([I 1; I 1]))));
  
    
]

(* TODO: Implement the missing cases of subst. *)
let rec subst ((e', x) as s) exp =
  match exp with
  | Var y ->
      if x = y then e'
      else Var y
  | I n -> I n
  | B b -> B b
  | Primop (po, args) -> Primop (po, List.map (subst s) args)
  | If (e, e1, e2) ->
      If (subst s e, subst s e1, subst s e2)
  | Let (y, e1, e2) ->
      let e1' = subst s e1 in
      if y = x then
        Let (y, e1', e2)
      else
        let (y, e2) =
          if List.mem y (free_variables e') then
            rename y e2
          else
            (y, e2)
        in
        Let (y, e1', subst s e2)

  | Rec (y, t, e) -> 
      (* Case 2 *)
      if x = y then 
        Rec (y, t, e)
      else 
        let (y, e) =
          if List.mem y (free_variables e') then
            rename y e (* Case 3 *)
          else 
            (y, e) (* Case 1 *)
        in 
        Rec (y, t, subst s e)

  | Fn (xs, e) -> 
      let rec x_in_xs xs = match xs with
        | [] -> false
        | (v, _) :: xs -> if v = x then true
            else x_in_xs xs
      in
      if x_in_xs xs then 
        Fn (xs, e)
      else 
        let (ns, _) = List.split xs in
        let (names', exp') = rename_all ns e in
        let renamed = 
          let rec replace xs counter = match xs with
            | [] -> []
            | (_, t) :: rest -> 
                ((List.nth names' counter), t) :: replace rest (counter + 1)
          in 
          replace xs 0
        in Fn(renamed, subst s exp') 
          
  | Apply (e, es) -> Apply ((subst s e), (List.map (fun exp -> subst s exp)) es)

and rename x e =
  let x' = freshVar x in
  (x', subst (Var x', x) e)

and rename_all names exp =
  List.fold_right
    (fun name (names, exp) ->
       let (name', exp') = rename name exp in
       (name' :: names, exp'))
    names
    ([], exp)

(* Applying a list of substitutions to an expression, leftmost first *)
let subst_list subs exp =
  List.fold_left (fun exp sub -> subst sub exp) exp subs


(* TODO: Write a good set of tests for eval. *)
let eval_tests = [
  (* An example test case.
     Note that you are *only* required to write tests for Rec and Apply!
  *)
  (Let ("x", I 1, Primop (Plus, [Var "x"; I 5])), I 6);
  (Rec ("x", Int, (If (B true, I 5, I 4))), I 5); 
  ((Apply (Fn (["y", Int], (If (B true, Var "y", I 4))), ([I 1]))), I 1);
  ((Apply (Fn ([("y", Int); ("x", Int)], (If (B true, Primop (Plus, [(Var "x"); (Var "y")]), I 0))), ([(I 4); I 3]))), I 7);
  ((Apply (Fn ([], (If (B true, I 5, I 4))), ([]))), I 5); 
  ((Apply (If (B true, (Fn(["y", Int], (If (B true, Var "y", I 4)))), I 0), ([I 8]))), I 8)
  
]

(* TODO: Implement the missing cases of eval. *)
let rec eval exp =
  match exp with
  (* Values evaluate to themselves *)
  | I _ -> exp
  | B _ -> exp
  | Fn _ -> exp

  (* This evaluator is _not_ environment-based. Variables should never
     appear during evaluation since they should be substituted away when
     eliminating binding constructs, e.g. function applications and lets.
     Therefore, if we encounter a variable, we raise an error.
*)
  | Var x -> raise (Stuck (Free_variable x))

  (* Primitive operations: +, -, *, <, = *)
  | Primop (po, args) ->
      let args = List.map eval args in
      begin
        match eval_op po args with
        | None -> raise (Stuck Bad_primop_args)
        | Some v -> v
      end

  | If (e, e1, e2) ->
      begin
        match eval e with
        | B true -> eval e1
        | B false -> eval e2
        | _ -> raise (Stuck If_non_true_false)
      end

  | Let (x, e1, e2) ->
      let e1 = eval e1 in
      eval (subst (e1, x) e2)

  | Rec (f, _, e) -> eval e 
      (*
        begin
          match e with
          | Fn (args, exp) -> 
              begin
                match args with
                | [] -> Fn (args, exp)
                | (v, _) -> subst (ex
              end
          | _ -> eval e
        end
                       
    *)

  | Apply (e, es) -> match eval e with 
    | Fn (xs, e) -> 
        begin
          let pair = List.split xs in 
          let names = 
            begin
              match pair with 
              | (n, t) -> n
            end
          in 
          try
            let combined = List.combine es names in
            subst_list combined e
          with Invalid_argument "List.combine" -> raise (Stuck Arity_mismatch)
        end
    | _ -> raise (Stuck Apply_non_fn)

(* TODO: Write a good set of tests for infer. *)
let infer_tests = [
  (* An example test case.
     Note that you are *only* required to write tests for Rec, Fn, and Apply!
  *)
  (([("x", Int)], Var "x"), Int);
  (*(([("x", Int)], Rec ("x", Int, Fn ([], Var "x"))), Arrow([Int], Int));*)
  (([("y", Int)], Fn (["y", Int], (If (B true, Var "y", I 4)))), Arrow([Int], Int));
  (([("x", Int); ("y", Int)], Fn ([("x", Int); ("y", Int)], Primop (Times, [Var "x"; Var "y"]))), Arrow ([Int; Int], Int));
  (([], Fn ([], Primop (Times, [I 3; I 4]))), Arrow ([], Int));
  (([("y", Int)], (Apply (Fn (["y", Int], (If (B true, Var "y", I 4))), ([I 1])))), Int);
  (([("x", Int); ("y", Int)], Apply (Fn ([("y", Int); ("x", Int)], (If (B true, Primop (Plus, [(Var "x"); (Var "y")]), I 0))), ([(I 4); I 3]))), Int);
  (([], Apply (Fn ([], (If (B true, I 5, I 4))), ([]))), Int); 
]

(* TODO: Implement the missing cases of infer. *)
let rec infer ctx e =
  match e with
  | Var x ->
      begin
        try lookup x ctx
        with Not_found -> raise (TypeError (Free_variable x))
      end
  | I _ -> Int
  | B _ -> Bool

  | Primop (po, exps) ->
      let (domain, range) = primopType po in
      check ctx exps domain range

  | If (e, e1, e2) ->
      begin
        match infer ctx e with
        | Bool ->
            let t1 = infer ctx e1 in
            let t2 = infer ctx e2 in
            if t1 = t2 then t1
            else type_mismatch t1 t2
        | t -> type_mismatch Bool t
      end

  | Let (x, e1, e2) ->
      let t1 = infer ctx e1 in
      infer (extend ctx (x, t1)) e2

  | Rec (f, t, e) -> raise NotImplemented

  | Fn (xs, e) ->
      let te = infer (extend_list ctx xs) e in 
      let arrow_args = 
        let rec get_ts ts = 
          match ts with 
          | [] -> []
          | (_, t) :: ts -> t :: get_ts ts
        in 
        get_ts xs
      in Arrow(arrow_args, te)
      

  | Apply (e, es) -> match infer ctx e with 
    | Arrow (_) -> infer ctx e
    | t -> raise (TypeError (Apply_non_arrow t))

and check ctx exps tps result =
  match exps, tps with
  | [], [] -> result
  | e :: es, t :: ts ->
      let t' = infer ctx e in
      if t = t' then check ctx es ts result
      else type_mismatch t t'
  | _ -> raise (TypeError Arity_mismatch)

(* TODO: Implement type unification. *)
let rec unify (t1 : utp) (t2 : utp) : unit =
  match t1, t2 with
  (* unifying identical concrete types does nothing *)
  | UInt, UInt
  | UBool, UBool -> ()
  (* For type constructors, recursively unify the parts *)
  | UArrow (t1, t1'), UArrow (t2, t2') ->
      raise NotImplemented
  | UTVar a, _ -> unifyVar a t2
  | _, UTVar b -> unifyVar b t1
  (* All other cases are mismatched types. *)
  | _, _ -> unif_error @@ UnifMismatch (t1, t2)

(* Unify a variable with a type *)
and unifyVar a t =
  raise NotImplemented
