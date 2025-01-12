{
  description = "A C engironment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-24.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
        
        # Windows cross compilation setup
        pkgsCross = import nixpkgs {
          inherit system;
          crossSystem = {
            config = "x86_64-w64-mingw32";
            system = "x86_64-windows";
          };
        };
      in
      {
        packages = {
          default = pkgs.stdenv.mkDerivation {
            name = "photosort";
            src = ./.;
            
            buildInputs = with pkgs; [
              raylib
            ];
            
            buildPhase = ''
              $CC -o photosort main.c -lraylib -lm \
                -Wall -Wextra -Werror -std=c99 -pedantic
            '';
            
            installPhase = ''
              mkdir -p $out/bin
              cp photosort $out/bin/
            '';
          };

          windows = pkgsCross.stdenv.mkDerivation {
            name = "photosort-windows";
            src = ./.;
            
            buildInputs = with pkgsCross; [
              raylib
            ];
            
            buildPhase = ''
              $CC -o photosort.exe main.c -lraylib -lm \
                -Wall -Wextra -Werror -std=c99 -pedantic
            '';
            
            installPhase = ''
              mkdir -p $out/bin
              cp photosort.exe $out/bin/
            '';
          };
        };

        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            clang
            lldb
            libllvm
            gnumake
            raylib
            raygui
            clang-tools
          ];
        };
      });
}
