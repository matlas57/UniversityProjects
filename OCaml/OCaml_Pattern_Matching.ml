(* TODO: Write some tests for tabulate. *)
(* Note: we've added a type annotation here so that the compiler can help
   you write tests of the correct form. *)
let tabulate_tests: (((int -> int) * int) * int list) list = [
  (((fun x -> x), 3), [0; 1; 2; 3]);
  (((fun x -> x * 2), 4), [0; 2; 4; 6; 8]);
  (((fun x -> x), 0), [0]); 
  (((fun x -> x), -1), []);
]

(* TODO: Implement dist_table: (int * int) -> int -> float list *)
    (*
      use tabulate to make the list with dist_black
      here the manner to "nest" function calls can be seen
    *)
let dist_table ((marblesTotal, marblesDrawn): (int * int)) (x: int) : float list =
  tabulate (fun n -> dist_black n x (marblesTotal, marblesDrawn)) marblesTotal

(* TODO: Write some test cases for is_empty. *)
let is_empty_tests: (float list list * bool) list = [
  ([ []; []; [] ] , true);
  ([ [4.5]; []; [] ], false);
  ([ [] ], true);
]

(* TODO: Implement is_empty: 'a list list -> bool *)
let is_empty (matrix: 'a list list) : bool =
  List.for_all (fun l -> l = []) matrix

(* TODO: Implement dist_matrix: int * int -> int list -> float list list *)
let dist_matrix ((total, drawn): int * int) (resultList: int list) : float list list =
  List.map(fun x -> dist_table (total, drawn) x) resultList

(* TODO: Implement combined_dist_table: float list list -> float list *)
let rec combined_dist_table (matrix: float list list) = 
  raise NotImplemented
    (*
    match matrix with 
    | a :: (b :: t) -> combined_dist_table (List.map (fun f -> (List.fold_left(fun f y -> f *. y) 1.0 a) (List.hd(b))) @ t)
    
       *)                    
        

(* Once you have implemented all the above functions, you can
   use this function to compute the maximum likelihood.
   You can try it on the given example by running:
     max_likelihood (6, 3) [2; 0; 1]
*)
let max_likelihood (total, drawn) resultList =
  max_in_list
    (combined_dist_table
       (dist_matrix (total, drawn) resultList))


(* TODO: Implement all: (ingredients list -> bool) -> cake -> bool *)
let rec all (p: (ingredients list -> bool)) (c: cake) : bool = 
  match c with 
  | Slice s -> (p s)
  | Cake (c1, c2) -> all p c1 && all p c2
(* TODO: Write some test cases for is_chocolate_cake. *)
let is_chocolate_cake_tests = [
  ((Cake (Slice [Chocolate ; Flour], Cake (Slice [Chocolate ; Almonds] , Slice [Chocolate ; BlackBeans])), true));
  (Slice [Flour; Almonds], false);
  ((Cake (Slice[Chocolate ; Flour] , Cake (Slice [Orange ; Almonds] , Slice [Chocolate ; BlackBeans])), false));
]

(* TODO: Implement is_chocolate_cake: cake -> bool *)
let is_chocolate_cake (c: cake) : bool = 
  all (fun x -> List.exists(fun e -> e = Chocolate) x) c 


(* TODO: Implement map: (ingredients list -> ingredients list) -> cake -> cake *)
let rec map (p: (ingredients list -> ingredients list)) (c: cake) = 
  match c with 
  | Slice s -> Slice (p s)
  | Cake (c1, c2) -> Cake (map p c1, map p c2)
                                  

(* TODO: Write some test cases for add_ingredient. *)
let add_ingredient_tests = [
  ((Orange, (Cake (Slice [Chocolate ; Flour], Cake (Slice [Chocolate ; Almonds] , Slice [Chocolate ; BlackBeans])))),
   (Cake (Slice [Chocolate ; Flour; Orange], Cake (Slice [Chocolate ; Almonds; Orange] , Slice [Chocolate ; BlackBeans; Orange]))));
]

(* TODO: Implement add_ingredient: ingredients -> cake -> cake *)
let add_ingredient (x: ingredients) (c: cake) : cake = 
  map (fun p -> 
      if (List.exists(fun e -> e = x) p) then
        p
      else
        p @ [x]) c
    
    
(* TODO: Implement fold_cake: (ingredients list -> 'a -> 'a) -> 'a -> cake -> 'a  *)
let rec fold_cake (f: (ingredients list -> 'a -> 'a)) (base: 'a) (c: cake) : 'a = 
  match c with 
  | Slice s -> base
  | Cake (c1, c2) -> fold_cake f (fold_cake f base c1) c2 
                 




(* TODO: Implement get_all_ingredients: cake -> ingredients list *)
let get_all_ingredients (c: cake) : ingredients list = 
  raise NotImplemented
      
    









