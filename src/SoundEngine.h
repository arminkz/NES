#pragma once
#include "stdafx.h"

#include "utilities/Singleton.h"

#include <SDL_mixer.h>

class SoundEngine : public Singleton<SoundEngine>
{
    friend class Singleton<SoundEngine>;

public:
    SoundEngine();
    ~SoundEngine();

    bool initialize(uint32_t nSampleRate = 44100, uint32_t nChannels = 1, uint32_t nBlocks = 8, uint32_t nBlockSamples = 512);
    bool destroy();

    void start();
    void stop();

    std::vector<std::string> getOutputDevices();

    uint32_t getSampleRate() const { return m_nSampleRate; }
    uint32_t getChannelsCount() const { return m_nChannels; }
    uint32_t getBlocksCount() const { return m_nBlocks; }
    uint32_t getBlockSamplesCount() const { return m_nBlockSamples; }

    void useOutputDevice(const std::string& sDeviceOut);

    void setNewSampleCallback(std::function<float(double)> func);

    void setOutputVolume(const float fVolume);

    void getFullOutputBlock(std::vector<float>& vFloatBuffer);

private:
    uint32_t m_nSampleRate = 44100;
	uint32_t m_nChannels = 1;
	uint32_t m_nBlocks = 8;
	uint32_t m_nBlockSamples = 512;
	double	 m_dSamplePerTime = 44100.0;
	double   m_dTimePerSample = 1.0 / 44100;
	double   m_dGlobalTime = 0.0;
	float    m_fOutputVolume = 1.0;

    std::string m_sOutputDevice;

    std::function<float(double)> m_funcNewSample;

    uint16_t m_haveFormat = AUDIO_F32SYS;
    std::vector<uint8_t> audioBuffer;
    Mix_Chunk audioChunk;
    bool m_keepRunning = false;

    static void SDLMixerCallback(int channel);
    
    uint32_t fillOutputBuffer(std::vector<float>& vBuffer, const uint32_t nBufferOffset, const uint32_t nRequiredSamples);
    void fillChunkBuffer(const std::vector<float>& userData) const;
};