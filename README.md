# BlackHole

BlackHole is an early-stage renderer written using modern OpenGL. This project primarily was created for learning purposes, but it may grow into something bigger over time.

***

## Screenshots

Currently the program looks like this:
![Screenshot of the program](/resources/screenshots/BlackHole_Screenshot_11.02.24.png?raw=true "BlackHole")

***

## Getting started

**Downloading the repository**
Clone the repository with `git clone --recursive https://github.com/MichailBlack/BlackHole.git`.
If you cloned the repository non-recursively, don't forget to clone necessary submodules with `git submodule update --init`.

**Build**
This project uses CMake. You can run it directly if your compiler supports CMake projects (e.g. Visual Studio) or generate project files manually.
The process of manual configuration and generation is trivial, just follow these steps:
```sh
cd [project-directory]
cmake -S . -B ./build
```
After execution of these commands you can find your project files in `./build/`. To build the project run your compiler or use this:
```sh
cmake --build ./build
```

Now you can make any changes that you want. If you prefer to manually use CMake, don't forget to regenerate projects after changes were made.

***

## The Plan

The plan for BlackHole right now is to implement the necessary features and tools to have an opportunity to create a scene directly from the editor.

### Main features to implement:

- High-fidelity Physically-Based 3D rendering
- Support for multiple rendering APIs (Vulkan, DirecX)
- Physics engine
- Audio system

### Short term features to implement:

- Cascaded shadow mapping
- Tile-based deferred shading
- SSAO

***

## Known Issues

- Cube maps uploading failure on some hardware
- ImGui and BlackHole events dispatching conflicts when trying to control Viewport camera via mouse (will investigate this later, when we have complete ECS)
