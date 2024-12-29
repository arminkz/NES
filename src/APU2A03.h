#pragma oncee
#include "stdafx.h"

class APU2A03
{
public:
    APU2A03();
    ~APU2A03();

    uint8_t cpuRead(uint16_t addr);
    void cpuWrite(uint16_t addr, uint8_t data);
    void clock();
    void reset();

    double getOutputSample();

private:

    uint32_t apuClockCounter = 0;
    uint32_t apuFrameCounter = 0;

    struct Sequencer
    {
        uint32_t sequence = 0x00000000;
        uint16_t timer = 0x0000;
        uint16_t reload = 0x0000;
        uint8_t output = 0x00;

        uint8_t clock(bool enable, std::function<void(uint32_t &s)> funcManip)
        {
            if (enable) {
                timer--;
                if (timer == 0xFFFF) {
                    timer = reload + 1;
                    funcManip(sequence);
                    output = sequence & 0x00000001;
                }
            }

            return output;
        }
    };

    struct OscPulse
    {
        double frequency = 0.0;
        double duty_cycle = 0.0;
        double amplitude = 1.0;
        double harmonics = 20;

        double sample(double time)
        {
            double a = 0.0;
            double b = 0.0;
            double pwm = duty_cycle * 2.0 * 3.14159;

            auto approxSin = [](float t) -> double
            {
                float j = t * 0.15915;
                j = j - (int)j;
                return 20.785 * j * (j - 0.5) * (j - 1.0);
            };

            for (int n = 1; n < harmonics; n++)
            {
                double w = n * frequency * 2.0 * 3.14159 * time;
                a += approxSin(w) / n;
                b += approxSin(w - pwm * n) / n;
            }

            return (a - b) * amplitude * (2.0 / 3.14159);
        }
    };

    double globalTime = 0.0;

    Sequencer pulse1_seq;
    OscPulse pulse1_osc;
    bool pulse1_enable = false;
    double pulse1_sample = 0.0;

};