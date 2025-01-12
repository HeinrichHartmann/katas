with builtins;
with (import ./preamble.nix);

let
  ns = rec {

    #
    # Read Local File
    #
    A = readFile ./Makefile;

    #
    # Create File in NixStore
    #
    store_path = toFile "Makefile" A;

    #
    # Read Directry Contents
    #
    C = ( readDir ./. );
    file_names = attrNames C;

  };
in
  trace (toJSON ns) false
