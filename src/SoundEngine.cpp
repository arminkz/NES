#include "SoundEngine.h"

SoundEngine::SoundEngine()
{
    m_sOutputDevice = "DEFAULT";
}

SoundEngine::~SoundEngine()
{
    stop();
    destroy();
}

bool SoundEngine::initialize(uint32_t nSampleRate, uint32_t nChannels, uint32_t nBlocks, uint32_t nBlockSamples)
{
    m_nSampleRate = nSampleRate;
	m_nChannels = nChannels;
	m_nBlocks = nBlocks;
	m_nBlockSamples = nBlockSamples;
	m_dSamplePerTime = double(nSampleRate);
	m_dTimePerSample = 1.0 / double(nSampleRate);

    auto errc = Mix_OpenAudioDevice(static_cast<int>(m_nSampleRate),
                                    AUDIO_F32,
                                    static_cast<int>(m_nChannels),
                                    static_cast<int>(m_nBlockSamples),
                                    m_sOutputDevice == "DEFAULT" ? nullptr : m_sOutputDevice.c_str(),
                                    SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    // Query the actual format of the audio device, as we have allowed it to be changed.
    if (errc || !Mix_QuerySpec(nullptr, &m_haveFormat, nullptr))
    {
        spdlog::error("[SoundEngine] Failed to open audio device '{}'", m_sOutputDevice);
        return false;
    }

    // Compute the Mix_Chunk buffer's size according to the format of the audio device
    uint32_t bufferSize = 0;
    switch (m_haveFormat)
    {
        case AUDIO_F32:
        case AUDIO_S32:
            bufferSize = m_nBlockSamples * m_nChannels * 4;
            break;
        case AUDIO_S16:
        case AUDIO_U16:
            bufferSize = m_nBlockSamples * m_nChannels * 2;
            break;
        case AUDIO_S8:
        case AUDIO_U8:
            bufferSize = m_nBlockSamples * m_nChannels * 1;
            break;
        default:
            spdlog::error("[SoundEngine] Audio format of device '{}' is not supported", m_sOutputDevice);
            return false;
    }

    // Allocate the buffer once. The size will never change after this
    audioBuffer.resize(bufferSize);
    audioChunk = {
        0,                  // 0, as the chunk does not own the array
        audioBuffer.data(), // Pointer to data array
        bufferSize,         // Size in bytes
        128                 // Volume; max by default as it's not controlled by the driver.
    };

    return true;
}

bool SoundEngine::destroy()
{
    Mix_CloseAudio();
    return true;
}

void SoundEngine::start()
{
    // Start the audio engine
    m_keepRunning = true;

    // Kickoff the audio driver
    SDLMixerCallback(0);

    // SDLMixer handles all other calls to reinsert user data
    Mix_ChannelFinished(SDLMixerCallback);
}

void SoundEngine::stop()
{
    m_keepRunning = false;

    // Stop might be called multiple times, so we check whether the device is already closed
    if (Mix_QuerySpec(nullptr, nullptr, nullptr))
    {
        for (int i = 0; i != m_nChannels; ++i)
        {
            if (Mix_Playing(i))
                Mix_HaltChannel(i);
        }
    }
}

std::vector<std::string> SoundEngine::getOutputDevices()
{
    return { "NOT_IMPLEMENTED" };
}

void SoundEngine::useOutputDevice(const std::string& sDeviceOut)
{
    m_sOutputDevice = sDeviceOut;
}

void SoundEngine::setNewSampleCallback(std::function<float(double)> func)
{
    m_funcNewSample = func;
}

void SoundEngine::setOutputVolume(const float fVolume)
{
    m_fOutputVolume = (fVolume < 0.0) ? 0.0 : (fVolume > 1.0) ? 1.0 : fVolume;
}

void SoundEngine::SDLMixerCallback(int channel)
{
    static std::vector<float> userData(getInstance()->getBlockSamplesCount() * getInstance()->getChannelsCount());

    if (channel != 0)
    {
        spdlog::error("[SoundEngine] Unexpected channel number");
    }

    // Don't add another chunk if we should not keep running
    if (!getInstance()->m_keepRunning)
        return;

    getInstance()->getFullOutputBlock(userData);
    getInstance()->fillChunkBuffer(userData);

    if (Mix_PlayChannel(0, &getInstance()->audioChunk, 0) == -1)
    {
        spdlog::error("[SoundEngine] Error while playing Chunk");
    }
}

void SoundEngine::getFullOutputBlock(std::vector<float>& vFloatBuffer)
{
    uint32_t nSamplesToProcess = m_nBlockSamples;
    uint32_t nSampleOffset = 0;
    while (nSamplesToProcess > 0)
    {
        uint32_t nSamplesGathered = fillOutputBuffer(vFloatBuffer, nSampleOffset, nSamplesToProcess);

        nSampleOffset += nSamplesGathered;
        nSamplesToProcess -= nSamplesGathered;
    }
}

uint32_t SoundEngine::fillOutputBuffer(std::vector<float>& vBuffer, const uint32_t nBufferOffset, const uint32_t nRequiredSamples)
{
    for (uint32_t nSample = 0; nSample < nRequiredSamples; nSample++)
    {
        double dSampleTime = m_dGlobalTime + nSample * m_dTimePerSample;

        for (uint32_t nChannel = 0; nChannel < m_nChannels; nChannel++)
        {
            // Construct the sample
            float fSample = 0.0f;

            if (m_funcNewSample)
                fSample += m_funcNewSample(dSampleTime);

            // Place sample in buffer
            vBuffer[nBufferOffset + nSample * m_nChannels + nChannel] = fSample * m_fOutputVolume;
        }
    }

    // UPdate global time, accounting for error (thanks scripticuk)
    m_dGlobalTime += nRequiredSamples * m_dTimePerSample;
    return nRequiredSamples;
}

template<typename Int>
void ConvertFloatTo(const std::vector<float>& fromArr, Int* toArr)
{
    static auto minVal = static_cast<float>(std::numeric_limits<Int>::min());
    static auto maxVal = static_cast<float>(std::numeric_limits<Int>::max());
    for (size_t i = 0; i != fromArr.size(); ++i)
    {
        auto clamped = (fromArr[i] * maxVal < minVal) ? minVal : (fromArr[i] * maxVal > maxVal) ? maxVal : fromArr[i] * maxVal;
        toArr[i] = static_cast<Int>(clamped);
    }
}

void SoundEngine::fillChunkBuffer(const std::vector<float>& userData) const
{
    // Since the audio device might have changed the format we need to provide,
    // we convert the wave data from the user to that format.
    switch (m_haveFormat)
    {
        case AUDIO_F32:
            memcpy(audioChunk.abuf, userData.data(), audioChunk.alen);
            break;
        case AUDIO_S32:
            ConvertFloatTo<Sint32>(userData, reinterpret_cast<Sint32*>(audioChunk.abuf));
            break;
        case AUDIO_S16:
            ConvertFloatTo<Sint16>(userData, reinterpret_cast<Sint16*>(audioChunk.abuf));
            break;
        case AUDIO_U16:
            ConvertFloatTo<Uint16>(userData, reinterpret_cast<Uint16*>(audioChunk.abuf));
            break;
        case AUDIO_S8:
            ConvertFloatTo<Sint8>(userData, reinterpret_cast<Sint8*>(audioChunk.abuf));
            break;
        case AUDIO_U8:
            ConvertFloatTo<Uint8>(userData, audioChunk.abuf);
            break;
    }
}
    