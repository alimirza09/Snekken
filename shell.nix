let
  pkgs = import <nixpkgs> {};
in
  pkgs.mkShell {
    packages = [
      # Choose the build tools that you need
      pkgs.cmake
      pkgs.gnumake
      pkgs.xorg.libX11
      pkgs.gcc
    ];
  }
