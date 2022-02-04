(* Question 1 *)
(* TODO: Write your own tests for the fact function.
         See the provided tests for double, above, for how to write test cases.
         Remember that you should NOT test cases for n < 0.
*)
(* fact_tests is done all tests pass grader *)
let fact_tests = [
  (0, 1.);
  (1, 1.);
  (2, 2.);
  (5, 120.)
]

(* fact function done all tests pass grader *)
let rec fact (n: int): float =
  match n with
  | 0 -> 1.
  | _ -> (float_of_int n) *. fact(n - 1)


(* binomial_tests is done all tests pass grader *)
let binomial_tests = [
  (* Your test cases go here. Correct the incorrect test cases for the function. *)
  ((0, 0), 1.);
  ((1, 0), 1.);
  ((2, 0), 1.);
  ((10, 1), 10.);
  ((10, 2), 45.)
]

(* binomial function done all tests pass grader *)
let binomial (n: int) (k: int) =
  if n < 0 || k < 0
  then domain ()
  else (if k > n
        then domain ()
        else fact n /. (fact k *. fact (n - k)))


(* ackerman_tests done all tests pass grader *)
let ackerman_tests = [
  (* Your test cases go here *)
  ((0, 0), 1);
  ((0, 1), 2);
  ((1, 0), 2);
  ((2, 3), 9);
]

(* ackerman function done all tests pass grader *)

let ackerman (n, k)  =
  if n < 0 || k < 0
  then domain ()
  else (let rec ack n k =
          match (n, k) with
          | (0, _) -> k + 1
          | (_, 0) -> ack (n - 1) 1
          | (_, _) -> ack (n - 1) (ack n (k - 1))
        in ack n k)


(* Question 2: is_prime *)

(* TODO: Write a good set of tests for is_prime. *)
let is_prime_tests = [
(* Your tests go here *)
  (2, true);
  (3, true);
  (4, false);

]

(* is_prime is done all tests pass grader *)
let is_prime n =
  if n < 2
  then domain()
  else 
    let h = (n / 2) in
    let rec prm n h = 
      match h with
      | 1 -> true
      | _ -> match n mod h with 
        | 0 -> false
        | _ -> prm n (h - 1)
    in prm n h
        
          
          


(* Question 3: Newton-Raphson method for computing the square root
*)

let square_root_tests = [ 
  (1., 1.);
  (4., 2.);
  (16., 4.);
  (36., 6.);
]


let square_root a =
  let rec findroot x acc =
    if abs_float((((a /. x) +. x) /. 2.) -. x) < acc
    then (((a /. x) +. x) /. 2.)
    else findroot (((a /. x) +. x) /. 2.) acc
  in
  if a > 0.
  then findroot 1. epsilon_float
  else domain ()
       

(* Question 4: Fibonacci*)

(* TODO: Write a good set of tests for fib_tl. *)
let fib_tl_tests = [
  (0, 1);
  (1, 1);
  (2, 2);
  (3, 3);
  (4, 5);
]

(* TODO: Implement a tail-recursive helper fib_aux. *)
let rec fib_aux n a b =
  match n with 
  | 2 -> a + b
  | _ -> fib_aux (n - 1) (a + b) (a)
  
  

(* TODO: Implement fib_tl using fib_aux. *)
let fib_tl n =
  match n with 
  | 0 -> 1
  | 1 -> 1
  | _ -> fib_aux n 1 1
           
      
