# NES
NES Console Emulator



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