(* TODO: Write some tests for neighbours. Consider creating a graph first,
 and then writing your tests based on it *)
let g = 
  {
    nodes = ["1"; "2"; "3"; "4"; "5"];
    edges = [("1", "2", 5); ("3", "4", 6); ("5", "3", 1); ("2", "4", 3); ("1", "4", 10)]
  }
(* Reminder: If a test case requires multiple arguments, use a tuple:
let myfn_with_2_args_tests = [
  ((arg1, arg1), (expected_output))
]
*)

(* We've added a type annotation here so that the compiler can help
   you write tests of the correct form. *)
let neighbours_tests: ((string graph * string) * (string * weight) list) list = 
  [
    ((g, "2"), ([("4", 3)]));
    ((g, "1"), ([("2", 5) ; ("4", 10)]));
    ((g, "4"), ([]));
  ]

(* TODO: Implement neighbours. *)
let neighbours (g: 'a graph) (vertex: 'a) : ('a * weight) list =
  let rec aux edges v = 
    match edges with
    | [] -> []
    | (v1, v2, w) :: es when v = v1 -> (v2, w) :: aux es v
    | _ :: es -> aux es v
  in aux g.edges vertex

(* TODO: Implement find_path. *)
let find_path (g: 'a graph) (a: 'a) (b: 'a) : ('a list * weight) = 
  let rec aux_node (node: 'a * weight) (visited : 'a list) weightTotal: ('a list * weight) = 
    match node with
    | (v, w) when v = b -> ((visited @ [b]), weightTotal)
    | (v, w) ->
        let nbrs = neighbours g v in
        match nbrs with
        | [] -> raise Fail
        | _ -> aux_list nbrs (visited @ [v]) (weightTotal)
  and aux_list (nodes: ('a * weight) list) (visited: 'a list) weightTotal : ('a list * weight) =
    match nodes with 
    | [] -> raise Fail
    | (v, w) :: ns -> 
        if List.exists(fun x -> x = v) visited then 
          aux_list ns visited weightTotal
        else 
          try 
            aux_node (v, w) visited (w + weightTotal)
          with Fail ->
            aux_list ns visited (weightTotal)
  in
  aux_node (a, 0) [] 0
    
    

(* TODO: Implement find_path'. *)
let find_path' (g: 'a graph) (a: 'a) (b: 'a) : ('a list * weight) =
  let rec aux_node (node: 'a * weight) (visited : 'a list) weightTotal fc sc : ('a list * weight)=
    match node with 
    | (v, w) when v = b -> sc (visited @ [b]) weightTotal
    | (v, w) -> 
        let nbrs = neighbours g v in
        aux_list nbrs (visited @ [v]) weightTotal fc sc
  and aux_list (nodes: ('a * weight) list) (visited: 'a list) weightTotal fc sc : ('a list * weight) =
    match nodes with
    | [] -> fc ()
    | (v, w) :: ns ->
        if List.exists(fun x -> x = v) visited then 
          aux_list ns visited weightTotal fc sc
        else 
          aux_node (v, w) visited (w + weightTotal) 
            (fun () -> aux_list ns visited weightTotal fc sc) sc
  in
  aux_node (a, 0) [] 0 (fun () -> raise Fail) (fun x y -> (x, y))


(* TODO: Implement find_all_paths *)
let find_all_paths (g: 'a graph) (a: 'a) (b: 'a) : ('a list * weight) list =
  let rec aux_node (node: 'a * weight) (visited : 'a list) weightTotal paths: ('a list * weight) list = 
    raise NotImplemented
                          (* recursive call to find all paths from current node *)
    
  and aux_list (nodes: ('a * weight) list) (visited: 'a list) weightTotal paths: ('a list * weight) list =
    raise NotImplemented

  in
  raise NotImplemented



(* TODO: Implement find_shortest_path *)
let find_shortest_path (g: 'a graph) (a: 'a) (b: 'a) : ('a list * weight) option =
  let all_paths = find_all_paths g a b in 
  let rec min paths curLow curPath = 
    match all_paths with 
    | [] -> None
    | (l, w) :: ns -> if w < curLow then min ns w l
        else min ns curLow curPath
  in min all_paths 100000 []

(* ---------- Hamming Numbers ----------- *)

let rec numsFrom n =
  {
    hd = n;
    tl = Susp (fun () -> numsFrom (n+1))
  }
  
let rec ones = 
  {
    hd = 1 ; 
    tl = Susp (fun () -> ones)
  }
  
let rec byTwoFrom n =
  { 
    hd = n;
    tl = Susp (fun () -> byTwoFrom(n+2))
  }
let two = byTwoFrom 1 
let rec byThreeFrom n =
  {
    hd = n;
    tl = Susp(fun () -> byThreeFrom(n+3))
  }
let three = byThreeFrom 1
let rec take_str n s = match n with 
  | 0 -> []
  | n -> s.hd :: take_str (n-1) (force s.tl)
           
let rec merge s1 s2 = 
  if s1.hd < s2.hd then
    {
      hd = s1.hd;
      tl = Susp(fun () -> merge (force s1.tl) s2)
    }
  else if s2.hd < s1.hd then 
    {
      hd = s2.hd;
      tl = Susp(fun () -> merge s1 (force s2.tl))
    }
  else 
    {
      hd = s1.hd;
      tl = Susp(fun () -> merge (force s1.tl)(force s2.tl))
    }
              
let rec exp n = 
  {
    hd = n*2;
    tl = Susp(fun () -> exp (n*2))
  }
  

let rec hamming_series =
  let rec exp n m= 
    {
      hd = n;
      tl = Susp(fun () -> exp (n*m) m)
    }
  in 
  {
    hd = 1;
    tl = Susp(fun() -> merge (exp 2 2) (merge (exp 3 3) (exp 5 5)))
  }
  
  
      




