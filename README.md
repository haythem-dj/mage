# Mage

A minimal SDL3-based application written in C.
This project serves as a foundation for building a custom game engine or experimenting with real-time systems.

## Features

- SDL3 callback based application
- Keyboard input handling
- Window resizing support
- Fullscreen toggle (F11)

## Requirements

- C compiler (GCC / Clang / MSVC)
- CMake
- SDL3

## Build

This project uses CMake, so it can generate build files for any platform or IDE supported by CMake.
Scripts are available for Linux and Windows, but they assume:

- gcc is installed
- make is available

Make sure these tools are installed before running the scripts.

## SDL3 Dependency

CMake uses find_package(SDL3)

This means CMake must be able to locate SDL3 on your system.
You can either install SDL using a system package manager or download pre built binaries or build it your self.

### Important Note

- if you did not install SDL3 system-wide, you must tell CMake where SDL3 is installed:

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/Libraries/SDL3"
```
