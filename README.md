# Project Sienna

A 3d web game, with an concept of cleaning an object

## Project Structure

### C++ files (/cpps)

Using CMake for generating build system, and ninja for the build

- `include`: header files
- `src`: c++ source files
= `third-party`: external packages include glm

### Web (/web)

- Vanilla JS Project created with [Vite](https://vitejs.dev/)

## How to run project

### Compile from C++

You need CMake and Ninja for the process.

```zsh
# Load Emscripten SDK (if you already have the emsdk, skip this step)
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
source ./emsdk_env.sh
cd ..

# Generate build system with CMake
# You can choose preset in cpps/CMakePresets.json
cd cpps
cmake . --preset=debug
# cmake . --preset=release

# Build js and WASM file
cd build-debug
# cd build-release
ninja

# After the build, you can test the result with running a web project
```

### Run Web Project

```zsh
# Move to /web directory
cd web

# Install dependencies
npm i

# Start dev server
npm run dev
```
