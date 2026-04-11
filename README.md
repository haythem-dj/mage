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
- SDL3

## Build
Currently this project only works for me. It uses [nob](https://github.com/tsoding/nob.h) a build system. 

- Run once
```bash
# Linux
gcc -o nob nob.c

# Window
gcc -o nob.exe nob.c
```

- To build
```bash
nob
```

-To build and run
```bash
nob run
```

## License
MIT: see [LICENSE](LICENSE)
