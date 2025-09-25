# NES
An emulator for the Nintendo Entertainment System (NES) console, developed in C++.

![NES Demo](doc/nes_demo.gif?raw=true "NES Emulator")


## Supported Features

- [X] Cross Platform (Windows / Linux / OSX)
- [X] Joystick Support
- [ ] Mappers
- [ ] Save Game State
- [X] Developer Tools (CPU/RAM/PPU Inspector)
- [X] Load ROMs as .nes files
- [ ] Wiki Pages


## Build Instructions

Follow the steps below to build and run the project:

1. **Install CMake**  
    Make sure [CMake](https://cmake.org/) is installed on your system. You can download and install it from the [CMake Downloads page](https://cmake.org/download/).

2. **Clone Repository**
    ```bash
    git clone --recurse-submodules https://github.com/arminkz/NES.git
    cd NES
    ```

4. **Build and Run**
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ./NES
    ```


## Known Issues

- APU sound is not perfect. Sound artifacts might be heard.
- Right now we are using GLFW for the windowing. we should migrate to SDL.




# NES Architecture

![NES Architecture Diagram](doc/nes_overview.png?raw=true "NES Architecture")



# Addressable Range on CPU Bus

| Peripheral        | Address Start | Address End | Capacity           |
|-------------------|---------------|-------------|--------------------|
| RAM (Main Memory) | 0x0000        | 0x1FFF      | 8KB (Mirrored 2KB) |
| Program ROM       | 0x4020        | 0xFFFF      |                    |

<!-- | Row 2, Col 1 | Row 2, Col 2 | Row 2, Col 3 |
| Row 3, Col 1 | Row 3, Col 2 | Row 3, Col 3 | -->




# Addressable Range on PPU Bus

| Peripheral        | Address Start | Address End | Capacity           |
|-------------------|---------------|-------------|--------------------|
| Pattern Memory    | 0x0000        | 0x1FFF      | 8KB                |
| Name Table        | 0x2000        | 0x2FFF      | 2KB                |
| Palettes Memory   | 0x3F00        | 0x3FFF      |                    |