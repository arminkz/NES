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

    bool useRawOutput = false;

    struct Sequencer
    {
        uint32_t sequence = 0x00000000;
        uint32_t new_sequence = 0x00000000;
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


    struct LengthCounter
    {
        uint8_t counter = 0x00;

        uint8_t clock(bool enable, bool halt)
        {
            if(!enable){
                counter = 0;
            } 
            else{
                if (!halt && counter > 0)
                    counter--;
            }
    
            return counter;
        }
    };


    struct Envelope
    {
        bool start = false;
        bool disable = false;
        uint16_t divider_count = 0;
        uint16_t volume = 0;
        uint16_t output = 0;
        uint16_t decay_count = 0;

        void clock(bool loop)
        {
            if (!start)
            {
                if (divider_count == 0)
                {
                    divider_count = volume;
                    if (decay_count == 0)
                    {
                        if (loop)
                        {
                            decay_count = 15;
                        }
                    }
                    else
                    {
                        decay_count--;
                    }
                }
                else
                {
                    divider_count--;
                };
            }
            else
            {
                start = false;
                decay_count = 15;
                divider_count = volume;
            }


            if (disable)
            {
                output = volume;
            }
            else
            {
                output = decay_count;
            }
        }
;;
    };


    struct OscPulse
    {
        double frequency = 0.0;
        double duty_cycle = 0.0;
        double amplitude = 1.0;
        double harmonics = 5;

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


    struct Sweeper {
        bool enabled = false;
        bool down = false;
        bool reload = false;
        uint8_t shift = 0;
        uint8_t timer = 0;
        uint8_t period = 0;
        uint16_t change = 0;
        bool mute = false;

        void track(uint16_t &target)
        {
            if (enabled)
            {
                change = target >> shift;
                mute = (target < 8) || (target > 0x7FF);
            }
        }

        bool clock(uint16_t &target, bool channel)
        {
            bool changed = false;
            if (timer == 0 && enabled && shift > 0 && !mute)
            {
                if (target >= 8 && change < 0x7FF)
                {
                    if (down)
                    {
                        target -= change - channel;
                    }
                    else
                    {
                        target += change;
                    }
                    changed = true;
                }
            }

            if (timer == 0 || reload)
            {
                timer = period;
                reload = false;
            }
            else
            {
                timer--;
            }

            mute = (target < 8) || (target > 0x7FF);
            return changed;
        }
    };

    double globalTime = 0.0;

    static uint8_t length_table[];


    // Square Wave Pulse Channel 1
    bool pulse1_enable = false;
    bool pulse1_halt = false;
    double pulse1_sample = 0.0;
    double pulse1_output = 0.0;
    Sequencer pulse1_seq;
    OscPulse pulse1_osc;
    Envelope pulse1_env;
    LengthCounter pulse1_lc;
    Sweeper pulse1_sweep;

    // Square Wave Pulse Channel 2
    bool pulse2_enable = false;
    bool pulse2_halt = false;
    double pulse2_sample = 0.0;
    double pulse2_output = 0.0;
    Sequencer pulse2_seq;
    OscPulse pulse2_osc;
    Envelope pulse2_env;
    LengthCounter pulse2_lc;
    Sweeper pulse2_sweep;

    // Noise Channel
    bool noise_enable = false;
    bool noise_halt = false;
    double noise_sample = 0.0;
    double noise_output = 0.0;
    Sequencer noise_seq;
    Envelope noise_env;
    LengthCounter noise_lc;

    

};
