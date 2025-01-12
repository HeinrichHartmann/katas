with builtins;
with (import ./preamble.nix);

runTests (rec {
  #
  # Fibbonacci Sequences
  #

  # Recursion
  fib1 = n:
    if n <= 2 then 1 else (fib1 (n - 1)) + (fib1 (n - 2));

  # Iteration
  fib2 = n:
    let
      fi = i: acc: lag:
        if (i < n) then
          fi (i+1) (acc+lag) (acc)
        else
          acc;
    in
      fi 2 1 1;

  # Anonymous recursive function
  fib3 = n:
    (REC (self: i: acc: lag:
      (if i >= n then acc
       else self (i + 1) (acc + lag) (acc))
    )) 2 1 1;

  # Memorization
  fib5 = n:
    let
      # Lists are strict in length but lazy in values.
      # We can use this to trigger computation only on the first access.
      # iota n = [1 2 3 ... n]
      iota = n: if n == 0 then [] else (iota (n - 1)) ++ [n];
      results = map (x: fib x) ([0] ++ (iota n));
      fib = x:
          if x <= 2 then 1
          else
            (elemAt results (x - 1)) + (elemAt results (x - 2));
    in
      fib n;

  test_fib = fib:
    [
      (T (map fib (iota 10)) [1 1 2 3 5 8 13 21 34 55])
    ];

  #
  # Prime numbers
  #
  dividesp = p: n: (n / p) * p == n;
  primes1 = n: # -> list of primes
    let
      rf = primes: cand:
        if length cand == 0 then
          primes
        else
          let
            p = head cand;
            cand' = filter (x: !(dividesp p x)) (tail cand);
            primes' = primes ++ [p];
          in
            rf primes' cand';
    in
      rf [] (tail (iota n));

  # optimize filtering
  filter_out_multiles_of = p: lst:
    let
      rf = pre: np: post:
        if (length post) == 0 then pre else
          let
            x = head post;
            pre-x = pre ++ [x];
            tail-post = tail post;
          in
            if np < x then
              rf pre (np + p) post
            else if np == x then
              rf pre (np + p) tail-post
            else #  np >  (head post)
              rf pre-x np tail-post;
    in
      rf [] p lst;
  primes2 = n: # -> list of primes
    let
      rf = primes: cand:
        if length cand == 0 then
          primes
        else
          let
            p = head cand;
            cand' = filter_out_multiles_of p cand;
            primes' = primes ++ [p];
          in
            rf primes' cand';
    in
      rf [] (tail (iota n));

  test_primes = primes:
    [
      (T (primes 20) [2 3 5 7 11 13 17 19] )
    ];

  #
  # Sort
  #
  sort1 = REC (self: L:
    # quicksort
    assert isList L;
    if length L == 0 then L
    else
      let
        p = head L;
        A = filter (x: x < p) L;
        M = filter (x: x == p) L;
        B = filter (x: x > p) L;
      in
        (self A) ++ M ++ (self B)
  );

  sort2 = REC (self: L:
    # mergesort
    assert isList L;
    if length L <= 1 then L
    else
      let
        m = (length L) / 2;
        A = subList L 0 m ;
        B = subList L m (length L);
        splice = out: A: B:
          if length A == 0 then out ++ B else
          if length B == 0 then out ++ A else
          if (head A) < (head B) then
            splice (out ++ [ (head A) ]) (tail A) B
          else
            splice (out ++ [ (head B) ]) A (tail B);
      in
        splice [] (self A) (self B)
  );

  sort3 = L:
    # Bubble Sort
    let
      swapIfNeeded = L: i: j:
        if (elemAt L i) > (elemAt L j) then
          (swapList L i j) else L;
      bubblePass = L: lim:
        # walk from i=0 .. lim-2 and swap L[i] L[i+1] if needed
        let rf = i: out:
          if i > lim - 2 then
            out
          else # i <= lim - 2
            rf (i+1) (swapIfNeeded out (i) (i + 1));
        in
          rf 0 L;
      mainLoop = L: i:
        if i >= (length L) then L
        else
          mainLoop (bubblePass L ((length L) - i)) (i + 1);
    in
      mainLoop L 0;

  test_sort = sort: [
    (T (sort []) [])
    (T (sort [1]) [1])
    (T (sort [1 1]) [1 1])
    (T (sort [2 1]) [1 2])
    (T (sort [1 2 3]) [1 2 3])
    (T (sort [3 2 1]) [1 2 3])
    (T (sort [5 22 13 1 31 221 21 1]) [1 1 5 13 21 22 31 221])
  ];

  #
  # Euklidean Algorithm
  #
  mod = x: d: # returns number within 0 .. (d-1)
    if x >= 0 then x - (x / d) * d
      # We always want to always retrun a positive respresentative of the class x + Z*d.
      # To catch edge cases, e.g with d==1, we identify a positive representative and mod again.
    else (mod (x - (x / d) * d + d) d);

  test_mod = mod:
    [
      (T (mod 1 1) 0)
      (T (mod 3 1) 0)
      (T (mod (-1) 1) 0)
      (T (mod (-1) 2) 1)
      (T (mod 144 10) 4)
    ];

  div = x: d: # so that x = (div x d) * d + (mod x d)
    (x - (mod x d)) / d;

  gcd = x: y:
    if y == 0 then x else
      (gcd y (mod x y));

  test_gcd = gcd:
    [
      (T (gcd 1 1) 1)
      (T (gcd 3 1) 1)
      (T (gcd (-1) 1) 1)
      (T (gcd (-1) 2) 1)
      (T (gcd 144 10) 2)
    ];

  # Extended Euklidean Algoritm
  # Computes representation g = a*x + b*y
  # where g = gcd(a,b)
  egcd = x: y:
    assert x > 0;
    assert y > 0;
    assert isInt x;
    assert isInt y;
    let
      minimize = A: B:
        let
          r = mod A.g B.g;
          q = div A.g B.g;
        in
          # Write     A.g  = r   + q * M.g
          # so        r    = A.g - q * M.g
          #                = (A.a*x + A.b*y) - q*(M.a*x + M.b*y)
          #                = (A.a-q M.a)*x + (A.b-q M.b)*y
          { g=r;  a=(A.a - q * B.a); b=(A.b + q * B.b); };
      rf = A: B:
        if A.g == 0 then B else
          rf (minimize B A) A;
    in
      (rf { g=x; a=1; b=0;} { g=y; a=0; b=1; });

  test_egcd = egcd:
    [
      (T (egcd 1 1).g 1)
      (T (egcd 3 5) {g=1;a=2;b=1;})
      (T (egcd 75 21) {g=3;a=2;b=7;})
    ];
})
