#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
    releaseResources();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resamplingSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resamplingSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
    resamplingSource.releaseResources();
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

          
            auto metadata = reader->metadataValues;

            soundTitle = metadata.getValue("title", file.getFileNameWithoutExtension());
            soundArtist = metadata.getValue("artist", "Unknown Artist");
            soundDuration = reader->lengthInSamples / reader->sampleRate;

            juce::Logger::outputDebugString("Title: " + soundTitle);
            juce::Logger::outputDebugString("Artist: " + soundArtist);
            juce::Logger::outputDebugString("Duration: " + juce::String(soundDuration) + " sec");

            
            transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
            resamplingSource.setResamplingRatio(1.0);

            return true;
        }
    }
    return false;
}

void PlayerAudio::play()
{
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
}

void PlayerAudio::pause()
{
    transportSource.stop();
}

void PlayerAudio::end()
{
    transportSource.setPosition(transportSource.getLengthInSeconds());
}

void PlayerAudio::gotostart()
{
    transportSource.setPosition(0.0);
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}

void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}

void PlayerAudio::skipForward(double second)
{
    double newPos = transportSource.getCurrentPosition() + second;
    double length = transportSource.getLengthInSeconds();
    if (newPos < length)
        transportSource.setPosition(newPos);
    else
        transportSource.setPosition(length - 0.1);
}

void PlayerAudio::skipBackward(double second)
{
    double newPos = transportSource.getCurrentPosition() - second;
    if (newPos > 0)
        transportSource.setPosition(newPos);
    else
        transportSource.setPosition(0.0);
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}

void PlayerAudio::toggleMute()
{
    setMuted(!isMuted);
}

void PlayerAudio::setMuted(bool shouldBeMuted)
{
    if (shouldBeMuted && !isMuted)
    {
        previousGain = transportSource.getGain();
        transportSource.setGain(0.0f);
        isMuted = true;
    }
    else if (!shouldBeMuted && isMuted)
    {
        transportSource.setGain(previousGain);
        isMuted = false;
    }
}

void PlayerAudio::ToggleLoopingState()
{
    isLooping = !isLooping;
    if (readerSource != nullptr)
        readerSource->setLooping(isLooping);
}

void PlayerAudio::setSpeed(double ratio)
{
    if (ratio > 0.0)
        resamplingSource.setResamplingRatio(ratio);
}