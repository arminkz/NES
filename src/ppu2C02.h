#pragma once

#include "stdafx.h"

#include "Cartridge.h"

class PPU2C02 {

public:
    PPU2C02();
    ~PPU2C02();

private:
    //Devices (Memories) on the PPU Bus
    uint8_t tblName[2][1024];    //2x 1KB Name Tables for Layout of Background Tiles
    uint8_t tblPalette[32];      //32x 8-bit RGB Color Palette
    uint8_t tblPattern[2][4096]; //2x 4KB Pattern Tables for Sprites and Backgrounds

    //Color Palette for PAL systems
    glm::vec3 palColors[64];     
 
public:
    //Communications on the CPU Bus
    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);

    //Communications on the PPU Bus
    uint8_t ppuRead(uint16_t addr, bool readOnly = false);
    void ppuWrite(uint16_t addr, uint8_t data);

public:
    void connectCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void clock();
    void reset();

    bool frameComplete = false; // PPUs signals that a frame is done
    bool signal_nmi = false;    // PPU can signal the CPU to trigger an NMI

    std::vector<glm::vec3> getPatternTable(uint8_t i, uint8_t palette);
    glm::vec3 getColorFromPalette(uint8_t palette, uint8_t pixel);
    void printPalette();

    int16_t scanline = 0;
    int16_t cycle = 0;

private:
    std::shared_ptr<Cartridge> cart;

    // Status register
    union PPUSTATUS {
        struct {
            uint8_t unused : 5;
            uint8_t sprite_overflow : 1;
            uint8_t sprite_zero_hit : 1;
            uint8_t vertical_blank : 1;
        };
        uint8_t reg;
    } status;
 
    // Mask register
    union PPUMASK {;
        struct {
            uint8_t grayscale : 1;
            uint8_t render_background_left : 1;
            uint8_t render_sprites_left : 1;
            uint8_t render_background : 1;
            uint8_t render_sprites : 1;
            uint8_t enhance_red : 1;
            uint8_t enhance_green : 1;
            uint8_t enhance_blue : 1;
        };
        uint8_t reg;
    } mask;

    // Control register
    union PPUCTRL {
        struct {
            uint8_t nametable_x : 1;
            uint8_t nametable_y : 1;
            uint8_t increment_mode : 1;
            uint8_t pattern_sprite : 1;
            uint8_t pattern_background : 1;
            uint8_t sprite_size : 1;
            uint8_t slave_mode : 1;
            uint8_t enable_nmi : 1;
        };
        uint8_t reg;
    } control;

    // Loopy Register
    union LOOPY {
        struct {
            uint16_t coarse_x : 5;
            uint16_t coarse_y : 5;
            uint16_t nametable_x : 1;
            uint16_t nametable_y : 1;
            uint16_t fine_y : 3;
            uint16_t unused : 1;
        };
        uint16_t reg = 0x0000;
    };

    LOOPY vram_addr; // Active "pointer" address into nametable to extract background tile info
    LOOPY tram_addr; // Temporary store of information to be "transferred" into "pointer" at various times

    uint8_t fine_x = 0x00; // Fine X scroll (3 bits)
    uint8_t addr_latch = 0x00;
    uint8_t ppu_data_buffer = 0x00;

    // Background rendering
	uint8_t bg_next_tile_id        = 0x00;
	uint8_t bg_next_tile_attrib    = 0x00;
	uint8_t bg_next_tile_lsb       = 0x00;
	uint8_t bg_next_tile_msb       = 0x00;
	uint16_t bg_shifter_pattern_lo = 0x0000;
	uint16_t bg_shifter_pattern_hi = 0x0000;
	uint16_t bg_shifter_attrib_lo  = 0x0000;
	uint16_t bg_shifter_attrib_hi  = 0x0000;


    struct PPUOAMEntry {
        uint8_t y;         // Y Position of the sprite
        uint8_t id;        // ID of the tile from pattern memory
        uint8_t attribute; // Flags define how the sprite should be rendered
        uint8_t x;         // X Position of the sprite
    } OAM[64]; // 64 sprites maximum - each sprite entry is 4 bytes - total 256 bytes

    uint8_t oam_addr = 0x00; // OAM Address

    PPUOAMEntry scanline_sprites[8]; // Holds the 8 sprites to be rendered on the next scanline
    uint8_t scanline_sprite_count;   // Number of sprites found for the next scanline
    uint8_t sprite_shifter_pattern_lo[8]; // Low bit of the sprite pattern
    uint8_t sprite_shifter_pattern_hi[8]; // High bit of the sprite pattern

    bool sprite_zero_hit_possible = false;   // Flag to indicate if a sprite-zero hit is possible
    bool sprite_zero_being_rendered = false; // Flag to indicate if the zero sprite is being rendered

public:
    uint8_t* pOAM = (uint8_t*)OAM; // Pointer to access the OAM byte by byte

};