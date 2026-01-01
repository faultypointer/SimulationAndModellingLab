{
  description = "Raylib development environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = {
    self,
    nixpkgs,
    ...
  }: let
    system = "x86_64-linux";
  in {
    devShells."${system}".default = let
      pkgs = import nixpkgs {
        inherit system;
      };
    in
      pkgs.mkShell {
        packages = [
          pkgs.libGL
          pkgs.mesa

          pkgs.libxkbcommon
          pkgs.wayland
          pkgs.wayland-protocols
          pkgs.wayland-scanner
          # # X11 dependencies
          pkgs.xorg.libX11
          pkgs.xorg.libX11.dev
          pkgs.xorg.libXcursor
          pkgs.xorg.libXi
          pkgs.xorg.libXinerama
          pkgs.xorg.libXrandr

          # Uncomment the line beloUUUUUUw if you want to build Raylib with web support
          # pkgs.emscripten
        ];

        # Audio dependencies
        LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [pkgs.alsa-lib pkgs.wayland pkgs.libxkbcommon pkgs.libGL pkgs.mesa];
      };
  };
}
