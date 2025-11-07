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
		lastFile = file;
		if (auto* reader = formatManager.createReaderFor(file))
		{
			// ?? Disconnect old source first
			transportSource.stop();
			transportSource.setSource(nullptr);
			readerSource.reset();

			// Create new reader source
			readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

			auto metadata = reader->metadataValues;

			soundTitle = metadata.getValue("title", file.getFileNameWithoutExtension());
			soundArtist = metadata.getValue("artist", "Unknown Artist");
			soundDuration = reader->lengthInSamples / reader->sampleRate;

			juce::Logger::outputDebugString("Title: " + soundTitle);
			juce::Logger::outputDebugString("Artist: " + soundArtist);
			juce::Logger::outputDebugString("Duration: " + juce::String(soundDuration) + " sec");

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

bool PlayerAudio::isPlaying() const {
	return transportSource.isPlaying();
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


void PlayerAudio::ToggleLoopingState(bool shouldloop)
{
	isLooping = !isLooping;
	if (readerSource != nullptr)
	{
		auto CurrentPosition = transportSource.getCurrentPosition();

		readerSource->setLooping(shouldloop);

		transportSource.setPosition(CurrentPosition);
		if (isPlaying())
		transportSource.start();
	}
}

void PlayerAudio::setSpeed(double ratio)
{
	resamplingSource.setResamplingRatio(ratio);
}

void PlayerAudio::saveLocation(const juce::String& player)
{
	if (!readerSource)
		return;

	juce::File locationFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
		.getChildFile(player + ".txt");

	juce::FileOutputStream out(locationFile);
		out.setPosition(0);
		out.truncate();
		out << lastFile.getFullPathName() << "\n";
		out << juce::String(transportSource.getCurrentPosition(), 6) << "\n";
}

void PlayerAudio::loadLocation(const juce::String& player)
{
	juce::File locationFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
		.getChildFile(player + ".txt");


	juce::FileInputStream in(locationFile);

	juce::String path = in.readNextLine();
	double pos = in.readNextLine().getDoubleValue();


	juce::File file(path);
	if (file.existsAsFile())
	{ 
	loadFile(file);
		transportSource.stop();
		transportSource.setPosition(pos);
	}
}

juce::File PlayerAudio::getLastFile() const
{
	return lastFile;
}

