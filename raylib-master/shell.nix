# shell.nix
let
  pkgs = import <nixpkgs> {};
in
  pkgs.mkShell {
    packages = [
    pkgs.gcc
    pkgs.cmake
    pkgs.ninja
    pkgs.meson
    pkgs.alsa-lib 
    pkgs.mesa 
    pkgs.xorg.libX11
    pkgs.xorg.libXrandr 
    pkgs.xorg.libXi 
    pkgs.xorg.libXcursor 
    pkgs.xorg.libXinerama

    ];

    # ...
  }

