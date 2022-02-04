(* ------------------------------------------------------------------------*)
(* Q 1 : Money in the bank (25 points)                                     *)
(* ------------------------------------------------------------------------*)

let new_account (p: passwd) : bank_account =
  let pw = ref p in
  let balance = ref 0 in
  let incorrect = ref 0 in
  let lock = (fun () -> if !incorrect > 2 then raise too_many_attempts)  in
  {
    update_passwd = (fun given_pw new_pw -> if given_pw = !pw 
                      then (incorrect := 0;
                            pw := new_pw)
                      else (
                        incorrect := (!incorrect + 1);
                        raise wrong_pass));
    
    retrieve  = (fun given_pw amt -> lock ();
                  if given_pw = !pw
                  then (incorrect := 0;
                        (if (amt <= !balance)
                         then balance := (!balance - amt)
                         else raise no_money))
                  else (
                    incorrect := (!incorrect + 1);
                    raise wrong_pass));    
    
    deposit = (fun given_pw amt -> lock () ;
                if given_pw = !pw 
                then (incorrect := 0; 
                      balance := (!balance + amt))
                else (
                  lock ();
                  incorrect := (!incorrect + 1);
                  raise wrong_pass));
    
    print_balance = (fun given_pw -> lock ();
                      if given_pw = !pw
                      then (incorrect := 0; 
                            !balance)
                      else (
                        lock ();
                        incorrect := (!incorrect + 1);
                        raise wrong_pass));
  }
  
;;


(* ------------------------------------------------------------------------*)
(* Q 2 : Memoization (75 points)                                           *)
(* ------------------------------------------------------------------------*)

(* Q 2.1 : Counting how many function calls are made *)

let fib_I (n: int) : fib_result = 
  let num_rec = ref 1 in 
  let rec aux n = 
    if n = 0 then 0
    else if n = 1 then 1 
    else ((num_rec := (!num_rec + 2));
          aux (n - 2) + aux (n - 1))
  in
  {num_rec = !num_rec; result = aux n}
         


(* Q 2.2 : Memoization with a global store *)

let fib_memo (n: int) : int =
  let _ = Hashtbl.add store 0 0 in
  let _ = Hashtbl.add store 1 1 in
  let rec fib n =
    match Hashtbl.find_opt store n with 
    | Some v -> v
    | None -> (let result = fib(n - 2) + fib(n - 1) in
               (Hashtbl.add store n result);
               result)
  in
  fib n
;;


(* Q 2.3 : General memoization function *)
exception Not_Found

let memo (f: (('a -> 'b) -> 'a -> 'b)) (stats: stats) : ('a -> 'b) =
  let local = Hashtbl.create 1000 in
  let rec g x = 
    match Hashtbl.find_opt local x with 
    | None -> let result = f g x in
        Hashtbl.add local x result;
        stats.entries := (!(stats.entries) + 1);
        result
    | Some v -> stats.lkp := (!(stats.lkp) + 1);
        v 
  in
  g 
;;


(* Q 2.4 : Using memo to efficiently compute the Fibonacci number *)
(* We also accept let fibM = raise NotImplemented ;; *)
let fibM (n:int) : (int * stats) =
  let stats = {entries = ref 0; lkp = ref 0} in
  let result = memo (fun g n -> if n = 0 then 0
                      else if n = 1 then 1
                      else g (n - 2) + g(n - 1)) stats
  in 
  (result n, stats) 
;;











