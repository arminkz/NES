#include "APU2A03.h"

APU2A03::APU2A03()
{
}

APU2A03::~APU2A03()
{
}

uint8_t APU2A03::cpuRead(uint16_t addr)
{
    return 0;
}

void APU2A03::cpuWrite(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0x4000:
        // Set pulse 1 duty cycle
        switch ((data & 0xC0) >> 6)
        {
        case 0x00: pulse1_seq.sequence = 0b00000001; pulse1_osc.duty_cycle = 0.125; break;
        case 0x01: pulse1_seq.sequence = 0b00000011; pulse1_osc.duty_cycle = 0.250; break;
        case 0x02: pulse1_seq.sequence = 0b00001111; pulse1_osc.duty_cycle = 0.500; break;
        case 0x03: pulse1_seq.sequence = 0b11111110; pulse1_osc.duty_cycle = 0.750; break;
        }
        break;
    case 0x4001:
        break;

    case 0x4002:
        // Set pulse 1 frequency by setting the upper 8 bits of the reload value
        pulse1_seq.reload = (pulse1_seq.reload & 0xFF00) | data;
        break;

    case 0x4003:
        // Set pulse 1 frequency by setting the lower 3 bits of the reload value
        pulse1_seq.reload = (pulse1_seq.reload & 0x00FF) | ((data & 0x07) << 8);
        pulse1_seq.timer = pulse1_seq.reload + 1;
        break;

    case 0x4015:
        // Enable/disable pulse 1
        pulse1_enable = (data & 0x01) > 0;
        break;
    }
}

void APU2A03::clock()
{
    bool isQuarterFrameClock = false;
    bool isHalfFrameClock = false;

    globalTime += (0.33333333333 / 1789773.0);

    if (apuClockCounter % 6 == 0)
    {
        apuFrameCounter++;

        if (apuFrameCounter == 3729)
        {
            isQuarterFrameClock = true;
        }
        
        if (apuFrameCounter == 7457)
        {
            isQuarterFrameClock = true;
            isHalfFrameClock = true;
        }

        if (apuFrameCounter == 11186)
        {
            isQuarterFrameClock = true;
        }

        if (apuFrameCounter == 14916)
        {
            isQuarterFrameClock = true;
            isHalfFrameClock = true;
            apuFrameCounter = 0;
        }

        if (isQuarterFrameClock)
        {
            // Adjust volume envelope
        }

        if (isHalfFrameClock)
        {
            // Adjust note length and frequency sweepers
        }

        // pulse1_seq.clock(pulse1_enable, [](uint32_t &s) {
        //     s = ((s & 0x0001) << 7) | ((s & 0x00FE) >> 1);
        // });

        // pulse1_sample = (double)pulse1_seq.output;

        pulse1_osc.frequency = 1789773.0 / (16.0 * (pulse1_seq.reload + 1));  //nesdev.com wiki
        pulse1_sample = pulse1_osc.sample(globalTime);
    }

    apuClockCounter++;
}

void APU2A03::reset()
{
}

double APU2A03::getOutputSample()
{
    return pulse1_sample;
}