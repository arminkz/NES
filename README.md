# NES
An emulator for the Nintendo Entertainment System (NES) console, developed in C++.

![NES Demo](doc/nes_demo.gif?raw=true "NES Emulator")


## Build Instructions

Follow the steps below to build and run the project:

1. **Install Conan**  
    Ensure you have [Conan](https://conan.io/) installed on your system. You can install it using `pip`:
    ```bash
    pip install conan
    ```

2. **Clone Repository**
    ```bash
    git clone --recurse-submodules https://github.com/arminkz/NES.git
    cd NES
    ```

3. **Run Build Script**
    ```bash
    build_and_run_release.bat
    ```



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