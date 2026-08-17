# N-Body Stars Simulation

A gravitational N-body simulation written in C++, rendered with [raylib](https://www.raylib.com/).

## Features

- Gravitational attraction between all stars (`O(n²)`)
- Stars merge on collision
- Toggle between 2D and 3D view with `TAB`
- Free-fly camera in 3D (`WASD` + mouse)

## Build

The project uses CMake, with raylib pulled in as a submodule under `third_party/raylib`.

```bash
git clone --recurse-submodules <url>
cd nbody-stars
cmake -B build
cmake --build build --config Release
```

If you already cloned without `--recurse-submodules`:

```bash
git submodule update --init --recursive
```

## Controls

| Key | Action |
|---|---|
| `TAB` | Toggle 2D / 3D |
| `WASD` | Move camera (3D) |
| `SPACE` / `SHIFT` | Up / down (3D) |
| Mouse | Look around (3D) |
