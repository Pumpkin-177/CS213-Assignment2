#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
	formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio() { releaseResources(); }

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	resamplingSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	resamplingSource.getNextAudioBlock(bufferToFill);
}

void PlayerAudio::releaseResources()
{
	resamplingSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
	if (file.existsAsFile())
	{
		if (auto* reader = formatManager.createReaderFor(file))
		{
			// ?? Disconnect old source first
			transportSource.stop();
			transportSource.setSource(nullptr);
			readerSource.reset();

			// Create new reader source
			readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

			// Attach safely
			transportSource.setSource(readerSource.get(),
				0,
				nullptr,
				reader->sampleRate);
			transportSource.start();
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

void PlayerAudio::end() {
	transportSource.setPosition(transportSource.getLengthInSeconds());
}

void PlayerAudio::pause()
{
	transportSource.stop();
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
	double newstate = transportSource.getCurrentPosition() + second;
	double length = transportSource.getLengthInSeconds();
	if (newstate < length)
		transportSource.setPosition(newstate);
	else
		transportSource.setPosition(length - 0.1);
}
void PlayerAudio::skipBackward(double second)
{
	double newstate = transportSource.getCurrentPosition() - second;
	double length = transportSource.getLengthInSeconds();
	if (newstate > 0)
		transportSource.setPosition(newstate);
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

void PlayerAudio::gotostart()
{
	transportSource.setPosition(0.0);

}

void PlayerAudio::toggleMute()
{
	if (isMuted) {
		setMuted(false);
	}
	else {
		setMuted(true);
	}
}

void PlayerAudio::setMuted(bool shouldBeMuted)
{
	if (shouldBeMuted && !isMuted) {
		previousGain = transportSource.getGain();
		transportSource.setGain(0.0f);
		isMuted = true;
	}
	else if (!shouldBeMuted && isMuted) {
		transportSource.setGain(previousGain);
		isMuted = false;
	}
}


void PlayerAudio::ToggleLoopingState()
{
	isLooping = !isLooping;
	if (readerSource != nullptr)
	{
		auto CurrentPosition = transportSource.getCurrentPosition();

		readerSource->setLooping(isLooping);

		transportSource.setPosition(CurrentPosition);
		transportSource.start();
	}
}

void PlayerAudio::setSpeed(double ratio)
{
	resamplingSource.setResamplingRatio(ratio);
}
