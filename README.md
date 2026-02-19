# Lab 4 - Mipmapping

This lab implements a demonstration of texture mipmapping techniques in OpenGL. The scene features a large rotating floor plane with a chessboard texture that showcases different mipmapping filter modes and LOD (Level of Detail) bias controls.

## Features

- Multiple mipmapping filter modes:
  - Linear-Linear (GL_LINEAR_MIPMAP_LINEAR)
  - Nearest-Linear (GL_NEAREST_MIPMAP_LINEAR)
  - Linear-Nearest (GL_LINEAR_MIPMAP_NEAREST)
  - Nearest-Nearest (GL_NEAREST_MIPMAP_NEAREST)
- Option to disable mipmapping for comparison
- Interactive LOD bias slider (-3.0 to 3.0)
- Large 100x100 floor with 50x texture repetition to demonstrate mipmapping effects at distance
- Rotating floor plane to observe texture filtering behavior
- Real-time camera controls (WASD + mouse)
- ImGui interface for dynamic parameter adjustment

## Project Structure

```
Lab4/
├── src/
│   └── main.cpp
├── include/
│   ├── camera.h
│   ├── callbacks.h
│   ├── model.h
│   └── shaders.h
├── shaders/
│   ├── main.vert
│   └── main.frag
├── assets/
│   └── textures/
│       └── chess_board.jpeg
├── build_mac.sh
├── build_linx.sh
├── run.sh
└── CMakeLists.txt
```

## Build

For macOS:
```bash
chmod +x build_mac.sh
./build_mac.sh
```

For Linux:
```bash
chmod +x build_linx.sh
./build_linx.sh
```

## Run

```bash
chmod +x run.sh
./run.sh
```

## Controls

- **WASD + Space/Shift**: Camera movement
- **Mouse**: Look around (hold right-click to enable camera control)
- **TAB**: Toggle UI
- **UI Controls**: Switch between mipmapping modes and adjust LOD bias

## Implementation Details

The mipmapping demonstration uses OpenGL's `glTexParameteri` to dynamically switch between different minification filter modes. The large floor plane with high texture repetition (50x) creates a scenario where distant texels are observed, making the visual differences between mipmapping modes clearly visible. The LOD bias control allows fine-tuning of which mipmap levels are selected during rendering.

## Resources

Chessboard Texture:
<a href="https://www.vecteezy.com/free-vector/checkerboard">Checkerboard Vectors by Vecteezy</a>
# MipMapping
