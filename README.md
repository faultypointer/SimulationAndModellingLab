# Simulation and Modelling Lab

## The Setup
Using Nix flake for setting up the development environment.
Followed the instuctions from [raylib docs](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux#nixnixos)
The flake.nix file and the command to build raylib are different because I'm on wayland but the instruction above are for x11.
[This](https://github-wiki-see.page/m/raysan5/raylib/wiki/Working-on-GNU-Linux) helped with building raylib for wayland.

In your project root, run the following instructions:
```bash
mkdir external
wget https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz -P external/
tar xvf external/5.5.tar.gz -C external/
```

Use `direnv` or run `nix develop`.

Finally build raylib

```bash
cd external/raylib-5.5/src/
make PLATFORM=PLATFORM_DESKTOP GLFW_LINUX_ENABLE_WAYLAND=TRUE
```
