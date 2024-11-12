#pragma once
#include <cstdint>

#include <nes.h>

class CPU6502 {
public:
    
    enum FLAGS6502 {
        C = (1 << 0), //Carry Bit
        Z = (1 << 1), //Zero
        I = (1 << 2), //Disable Interrupts
        D = (1 << 3), //Decimal Mode (unused in this implementation)
        B = (1 << 4), //Break
        U = (1 << 5), //Unused
        V = (1 << 6), //Overflow
        N = (1 << 7), //Negative
    };

    //Registers
    uint8_t a = 0x00; //Accumulator Register
    uint8_t x = 0x00; //X Register
    uint8_t y = 0x00; //Y Register
    uint8_t stkp = 0x00; //Stack Pointer
    uint16_t pc = 0x0000; //Program Counter
    uint8_t status = 0x00; //Status Register


public:
    CPU6502();
    ~CPU6502();

    void connectBus(NES *n) { bus = n; }

private:
    NES *bus = nullptr;

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
};