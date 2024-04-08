let
  pkgs = import <nixpkgs> {};
in
  pkgs.mkShell {
    packages = [
      pkgs.cmake
    ];
    hardeningDisable = ["all"];

  }
