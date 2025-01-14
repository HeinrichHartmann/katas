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
          config.allowUnfree = true;
        };
        
        # Windows cross compilation setup
        mingw = pkgs.pkgsCross.mingw32;

        # Common build configuration
        CFLAGS = "-Wall -Wextra -Werror -std=c99 -pedantic";
        LIBS = "-lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo";
      in
      {
        packages = {
          default = pkgs.stdenv.mkDerivation {
            name = "photosort";
            src = ./.;
            
            buildInputs = with pkgs; [ raylib ];
            
            enableParallelBuilding = true;
            
            buildPhase = ''
              $CC -o photosort main.c ${LIBS} ${CFLAGS}
            '';
            
            installPhase = ''
              mkdir -p $out/bin
              cp photosort $out/bin/
            '';
          };

          windows = mingw.stdenv.mkDerivation {
            name = "photosort-windows";
            src = ./.;
            
            buildInputs = [ mingw.raylib ];
            
            enableParallelBuilding = true;
            
            buildPhase = ''
              $CC -o photosort.exe \
                main.c \
                -I${mingw.raylib}/include \
                -L${mingw.raylib}/lib \
                -lraylib \
                -lopengl32 \
                -lgdi32 \
                -lwinmm \
                -lm \
                -mconsole \
                -g \
                -O0 \
                ${CFLAGS}
            '';
            
            installPhase = ''
              mkdir -p $out/bin
              cp photosort.exe $out/bin/
              cp ${mingw.raylib}/bin/*.dll $out/bin/
            '';
          };
        };

        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            clang
            lldb
            libllvm
            gnumake
            clang-tools
            raylib
          ];
        };
      });
}
