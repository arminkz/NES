#include "cpu6502.h"

CPU6502::CPU6502()
{
    using a = CPU6502;
    lookup =
    {
        { "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 }
    };
}

CPU6502::~CPU6502()
{
    
}

/////////////////////////////////////////////////////////////////////////////////////
//Bus Read and Write Functions

uint8_t CPU6502::read(uint16_t addr)
{
    return bus->read(addr, false);
}

void CPU6502::write(uint16_t addr, uint8_t data)
{
    bus->write(addr, data);
}

/////////////////////////////////////////////////////////////////////////////////////
//External Signals and Interrupts

void CPU6502::clock()
{
    if (cycles == 0)
    {
        opcode = read(pc);
        pc++;
        cycles = lookup[opcode].cycles;
        uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();
        uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();
        cycles += (additional_cycle1 & additional_cycle2);
    }
    cycles--;
}

void CPU6502::reset()
{
    addr_abs = 0xFFFC;
    uint16_t lo = read(addr_abs + 0);
    uint16_t hi = read(addr_abs + 1);
    pc = (hi << 8) | lo;
    a = 0;
    x = 0;
    y = 0;
    status = 0x00 | U;
    addr_rel = 0x0000;
    addr_abs = 0x0000;
    fetched = 0x00;
    cycles = 8;
}

void CPU6502::irq()
{
    if (getFlag(I) == 0)
    {
        //Store PC
        write(0x0100 + stkp, (pc >> 8) & 0x00FF);
        stkp--;
        write(0x0100 + stkp, pc & 0x00FF);
        stkp--;
        //Store Status
        setFlag(B, 0);
        setFlag(U, 1);
        setFlag(I, 1);
        write(0x0100 + stkp, status);
        stkp--;
        //Jump to the Interrupt Routine
        addr_abs = 0xFFFE;
        uint16_t lo = read(addr_abs + 0);
        uint16_t hi = read(addr_abs + 1);
        pc = (hi << 8) | lo;
        cycles = 7;
    }
}

void CPU6502::nmi()
{
    //Store PC
    write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    write(0x0100 + stkp, pc & 0x00FF);
    stkp--;
    //Store Status
    setFlag(B, 0);
    setFlag(U, 1);
    setFlag(I, 1);
    write(0x0100 + stkp, status);
    stkp--;
    //Jump to the Interrupt Routine
    addr_abs = 0xFFFA;
    uint16_t lo = read(addr_abs + 0);
    uint16_t hi = read(addr_abs + 1);
    pc = (hi << 8) | lo;
    cycles = 8;
}

/////////////////////////////////////////////////////////////////////////////////////
//CPU cycle helper functions and variables

uint8_t CPU6502::fetch()
{
    if (!(lookup[opcode].addrmode == &CPU6502::IMP))
    {
        fetched = read(addr_abs);
    }
    return fetched;
}

uint8_t CPU6502::getFlag(FLAGS6502 f)
{
    return ((status & f) > 0) ? 1 : 0;
}

void CPU6502::setFlag(FLAGS6502 f, bool v)
{
    if (v)
    {
        status |= f;
    }
    else
    {
        status &= ~f;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//Addressing Modes

//Implied (no additional data is needed or its on accumulator)
uint8_t CPU6502::IMP() 
{
    fetched = a;
    return 0;
}

//Immediate (address is in the next byte)
uint8_t CPU6502::IMM() 
{
    addr_abs = pc++;
    return 0;
}

//Zero Page (address is in the zero page)
//Address is 1 byte instead of 2 bytes because the offset is in the zero page
uint8_t CPU6502::ZP0() 
{
    addr_abs = read(pc);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

//Zero Page with X Offset (address is in the zero page with X offset)
//Address is 1 byte instead of 2 bytes because the offset is in the zero page
uint8_t CPU6502::ZPX() 
{
    addr_abs = (read(pc) + x);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

//Zero Page with Y Offset (address is in the zero page with Y offset)
//Address is 1 byte instead of 2 bytes because the offset is in the zero page
uint8_t CPU6502::ZPY() 
{
    addr_abs = (read(pc) + y);
    pc++;
    addr_abs &= 0x00FF;
    return 0;
}

//Absolute (address is in the next two bytes)
uint8_t CPU6502::ABS() 
{
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;
    addr_abs = (hi << 8) | lo; //Combine the two bytes to form the address
    return 0;
}

//Absolute with X Offset (address is in the next two bytes with X offset)
uint8_t CPU6502::ABX() 
{
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;
    addr_abs = (hi << 8) | lo; //Combine the two bytes to form the address
    addr_abs += x;
    return ((addr_abs & 0xFF00) != (hi << 8)) ? 1 : 0; //If the high byte of the address changes, we need an additional cycle
}

//Absolute with Y Offset (address is in the next two bytes with Y offset)
uint8_t CPU6502::ABY() 
{
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;
    addr_abs = (hi << 8) | lo; //Combine the two bytes to form the address
    addr_abs += y;
    return ((addr_abs & 0xFF00) != (hi << 8)) ? 1 : 0; //If the high byte of the address changes, we need an additional cycle
}

//Indirect (pointer is in the next two bytes and is the pointer to the actual address)
uint8_t CPU6502::IND() 
{
    uint16_t ptr_lo = read(pc);
    pc++;
    uint16_t ptr_hi = read(pc);
    pc++;
    uint16_t ptr = (ptr_hi << 8) | ptr_lo;
    if (ptr_lo == 0x00FF) //Simulate page boundary hardware bug
    {
        addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr);
    }
    else
    {
        addr_abs = (read(ptr + 1) << 8) | read(ptr);
    }
    return 0;
}

//Indirect Zero Page with X offset (pointer is in the next byte and is the pointer to the actual address with X offset)
//Pointer is 1 byte instead of 2 bytes because the offset is in the zero page
uint8_t CPU6502::IZX() 
{
    uint16_t t = read(pc);
    pc++;
    uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
    uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);
    addr_abs = (hi << 8) | lo;
    return 0;
}

//Indirect Zero Page with Y offset (pointer is in the next byte and is the pointer to the actual address with Y offset)
//Pointer is 1 byte instead of 2 bytes because the offset is in the zero page
uint8_t CPU6502::IZY() 
{
    uint16_t t = read(pc);
    pc++;
    uint16_t lo = read(t & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);
    addr_abs = (hi << 8) | lo;
    addr_abs += y;
    return ((addr_abs & 0xFF00) != (hi << 8)) ? 1 : 0; //If the high byte of the address changes, we need an additional cycle
}

//Relative (address is in the next byte and is the offset from the current PC)
uint8_t CPU6502::REL() 
{
    addr_rel = read(pc);
    pc++;
    if (addr_rel & 0x80) //If the offset is negative
    {
        addr_rel |= 0xFF00; //Sign extend the offset
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
//Arithmatic and Logic Instructions

//Add with Carry
uint8_t CPU6502::ADC()
{
    fetch();
    uint16_t temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)getFlag(C);
    setFlag(C, temp > 255);
    setFlag(Z, (temp & 0x00FF) == 0);
    setFlag(N, temp & 0x80);
    setFlag(V, (~(uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp) & 0x0080);
    a = temp & 0x00FF;
    return 1;
}

//Subtract with Borrow (Carry)
uint8_t CPU6502::SBC()
{
    fetch();
    uint16_t value = ((uint16_t)fetched) ^ 0x00FF;
    uint16_t temp = (uint16_t)a + value + (uint16_t)getFlag(C);
    setFlag(C, temp & 0xFF00);
    setFlag(Z, (temp & 0x00FF) == 0);
    setFlag(N, temp & 0x80);
    setFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
    a = temp & 0x00FF;
    return 1;
}

//Logical AND
uint8_t CPU6502::AND()
{
    fetch();
    a = a & fetched;
    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);
    return 1;
}

//Arithmetic Shift Left
uint8_t CPU6502::ASL()
{
    fetch();
    uint16_t temp = (uint16_t)fetched << 1;
    setFlag(C, (temp & 0xFF00) > 0);
    setFlag(Z, (temp & 0x00FF) == 0x00);
    setFlag(N, temp & 0x80);
    if (lookup[opcode].addrmode == &CPU6502::IMP)
    {
        a = temp & 0x00FF;
    }
    else
    {
        write(addr_abs, temp & 0x00FF);
    }
    return 0;
}

//Decrement Value at Memory Location
uint8_t CPU6502::DEC()
{
    fetch();
    uint16_t temp = fetched - 1;
    write(addr_abs, temp & 0x00FF);
    setFlag(Z, (temp & 0x00FF) == 0x00);
    setFlag(N, temp & 0x80);
    return 0;
}

//Decrement X Register
uint8_t CPU6502::DEX()
{
    x--;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}

//Decrement Y Register
uint8_t CPU6502::DEY()
{
    y--;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 0;
}

//Exclusive OR
uint8_t CPU6502::EOR()
{
    fetch();
    a = a ^ fetched;
    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);
    return 1;
}

//Increment Value at Memory Location
uint8_t CPU6502::INC()
{
    fetch();
    uint16_t temp = fetched + 1;
    write(addr_abs, temp & 0x00FF);
    setFlag(Z, (temp & 0x00FF) == 0x00);
    setFlag(N, temp & 0x80);
    return 0;
}

//Increment X Register
uint8_t CPU6502::INX()
{
    x++;
    setFlag(Z, x == 0x00);
    setFlag(N, x & 0x80);
    return 0;
}

//Increment Y Register
uint8_t CPU6502::INY()
{
    y++;
    setFlag(Z, y == 0x00);
    setFlag(N, y & 0x80);
    return 0;
}



/////////////////////////////////////////////////////////////////////////////////////
//Branch Instructions

//Branch if Carry Clear
uint8_t CPU6502::BCC()
{
    if (getFlag(C) == 0)
    {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

//Branch if Carry Set
uint8_t CPU6502::BCS()
{
    if (getFlag(C) == 1)
    {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

//Branch if Equal
uint8_t CPU6502::BEQ()
{
    if (getFlag(Z) == 1)
    {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

//Branch if Not Equal
uint8_t CPU6502::BNE()
{
    if (getFlag(Z) == 0)
    {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

//Branch if Negative
uint8_t CPU6502::BMI()
{
    if (getFlag(N) == 1)
    {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

//Branch if Positive
uint8_t CPU6502::BPL()
{
    if (getFlag(N) == 0)
    {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

//Branch if Overflow Clear
uint8_t CPU6502::BVC()
{
    if (getFlag(V) == 0)
    {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

//Branch if Overflow Set
uint8_t CPU6502::BVS()
{
    if (getFlag(V) == 1)
    {
        cycles++;
        addr_abs = pc + addr_rel;
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
        {
            cycles++;
        }
        pc = addr_abs;
    }
    return 0;
}

//Jump to New Location
uint8_t CPU6502::JMP()
{
    pc = addr_abs;
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
//Clear and Set Instructions

//Clear Carry Flag
uint8_t CPU6502::CLC()
{
    setFlag(C, 0);
    return 0;
}

//Clear Decimal Flag
uint8_t CPU6502::CLD()
{
    setFlag(D, 0);
    return 0;
}

//Clear Interrupt Flag (Enable Interrupts)
uint8_t CPU6502::CLI()
{
    setFlag(I, 0);
    return 0;
}

//Clear Overflow Flag
uint8_t CPU6502::CLV()
{
    setFlag(V, 0);
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////
//Compare Instructions

//Compare Accumulator
uint8_t CPU6502::CMP()
{
    fetch();
    uint16_t temp = (uint16_t)a - (uint16_t)fetched;
    setFlag(C, a >= fetched);
    setFlag(Z, (temp & 0x00FF) == 0);
    setFlag(N, temp & 0x80);
    return 1;
}

//Compare X Register
uint8_t CPU6502::CPX()
{
    fetch();
    uint16_t temp = (uint16_t)x - (uint16_t)fetched;
    setFlag(C, x >= fetched);
    setFlag(Z, (temp & 0x00FF) == 0);
    setFlag(N, temp & 0x80);
    return 0;
}

//Compare Y Register
uint8_t CPU6502::CPY()
{
    fetch();
    uint16_t temp = (uint16_t)y - (uint16_t)fetched;
    setFlag(C, y >= fetched);
    setFlag(Z, (temp & 0x00FF) == 0);
    setFlag(N, temp & 0x80);
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////
//Flag Instructions


/////////////////////////////////////////////////////////////////////////////////////
//Jump and Call Instructions


/////////////////////////////////////////////////////////////////////////////////////
//Load Instructions


/////////////////////////////////////////////////////////////////////////////////////
//Stack Instructions

//Push Accumulator to Stack
uint8_t CPU6502::PHA()
{
    write(0x0100 + stkp, a);
    stkp--;
    return 0;
}

//Pop from Stack to Accumulator
uint8_t CPU6502::PLA()
{
    stkp++;
    a = read(0x0100 + stkp);
    setFlag(Z, a == 0x00);
    setFlag(N, a & 0x80);
    return 0;
}

//Break
uint8_t CPU6502::BRK()
{
    pc++;
    setFlag(I, 1);
    write(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    write(0x0100 + stkp, pc & 0x00FF);
    stkp--;
    setFlag(B, 1);
    write(0x0100 + stkp, status);
    stkp--;
    setFlag(B, 0);
    pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
    return 0;
}

//Bit Test
uint8_t CPU6502::BIT()
{
    fetch();
    uint16_t temp = a & fetched;
    setFlag(Z, (temp & 0x00FF) == 0x00);
    setFlag(N, fetched & (1 << 7));
    setFlag(V, fetched & (1 << 6));
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////
//Interrupt Instructions

//Return from Interrupt
uint8_t CPU6502::RTI()
{
    //Restore Status
    stkp++;
    status = read(0x0100 + stkp);
    status &= ~B;
    status &= ~U;
    //Restore PC
    stkp++;
    pc = (uint16_t)read(0x0100 + stkp);
    stkp++;
    pc |= (uint16_t)read(0x0100 + stkp) << 8;
    return 0;
}