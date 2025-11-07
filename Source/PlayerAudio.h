#pragma once 
#include <JuceHeader.h>

class PlayerAudio
{
public:
	PlayerAudio();
	~PlayerAudio();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();
	void saveLocation(const juce::String &player);
	void loadLocation(const juce::String& player);
	juce::File PlayerAudio::getLastFile() const;
	bool loadFile(const juce::File& file);
	void play();
	void stop();
	void setGain(float gain);
	void setPosition(double pos);
	void gotostart();
	void pause();
	void end();
	void skipBackward(double second);
	void skipForward(double second);
	double getPosition() const;
	double getLength() const;
	double getDuration() const { return soundDuration; }
	bool getMuteState() const { return isMuted; }
	bool isPlaying() const;
	void toggleMute();
	void setMuted(bool shouldBeMuted);
	void ToggleLoopingState(bool shouldloop);
	void setSpeed(double ratio);

	juce::String getTitle() const { return soundTitle; }
	juce::String getArtist() const { return soundArtist; }

	juce::AudioSource* getAudioSource() {
		return &resamplingSource;
	}

private:
	bool isMuted = false;
	bool isLooping = false;
	float previousGain = 1.0f;
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
	juce::ResamplingAudioSource resamplingSource{ &transportSource, false, 2 };
	juce::File lastFile;

	juce::String soundTitle, soundArtist;
	double soundDuration = 0.0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
