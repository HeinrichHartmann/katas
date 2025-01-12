with builtins;
rec {
  #
  # Helpers
  #
  # print argument as side-effect, return null
  print = x : (trace (toString x) x);
  # iota n = [ 1 2 3 ... n ]
  iota = n: if n <= 0 then [] else concatLists [(iota (n - 1)) [n]];
  # Fixpoint-combinator - used to define anonymous recursive funtions like so
  # REC (self: x: y: ..function body..) - self referrs to the name of the function in the body
  REC = f: let x = f x; in x;
  # force evaluation of all entries in list
  force = seq: deepSeq seq null;
  # predicate version of match
  matchp = regexp: str:
    if (match regexp str) != null then true else false;
  # like listToattrs
  attrsToList = set:
    map (name: { name = name; value = getAttr name set; }) (attrNames set);

  subList = L: start: end:
    # returns [ L[start] ... L[end - 1] ]
    assert isList L;
    assert isInt start && start >= 0;
    assert isInt end && end >= start && end <= length L;
    let
      rf = out: idx:
        if idx == end then out else
          rf (out ++ [ (elemAt L idx) ]) (idx + 1);
    in
      rf [] start;

  swapList = L: i: j:
    (subList L 0 i) ++ [ (elemAt L j) ] ++ (subList L (i + 1) j) ++ [ (elemAt L i) ] ++ (subList L (j + 1) (length L));


  #
  # Testing Framework
  #
  equals = x: y:
    if isInt x then x == y else
      if isString x then x == y else
        (toJSON x) == (toJSON y);
  T = val1: val2:
    if (equals val1 val2) then
      "✅"
    else
      (trace "expected ${toJSON val2} got ${toJSON val1}" "❌");
  runTests = namespace:
    # runs all test of an algorithm against available implementations
    #
    # - tests are functions that start with test_* -> test_gcd
    # - algorithms are the things that match test_(.*) -> gcd
    # - implementations are functions with name algorithm_name + digits -> gcd1, gcd2, gcd3
    #
    # runTests {} will run (test_gcd gcd1) (test_gcd gcd2) (test_gcd gcd3)
    #
    let
      isTest = name:
        matchp "^test_.*" name;
      get_implementations = alg: ns:
        assert isString alg;
        assert isAttrs ns;
        let
          impl_names = filter (name: matchp "^${alg}[[:digit:]]*" name) (attrNames ns);
        in
          map (name: { name=name; lambda=getAttr name ns; } ) impl_names;
      test_names = filter isTest (attrNames namespace);
      tests = map (name:
        rec {
          test_name = name;
          algorithm = elemAt (match "^test_(.*)$" name) 0;
          test = getAttr name namespace;
          implementations = get_implementations algorithm namespace;
          run = _:
            trace "Running tests for ${algorithm}"
              (map ( impl: [ impl.name ":" ] ++ (test impl.lambda) ++  [";"]) implementations);
        }
      ) test_names;
    in
      force (map (t: (print (t.run true))) tests);
}
