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
	bool getMuteState() const { return isMuted; }
	void toggleMute();
	void setMuted(bool shouldBeMuted);
	void ToggleLoopingState();
	void setSpeed(double ratio);

private:
	bool isMuted = false;
	bool isLooping = false;
	float previousGain = 1.0f;
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
	juce::ResamplingAudioSource resamplingSource{ &transportSource, false, 2 };
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
