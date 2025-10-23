#pragma once // PlayerAudio.h
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
	void start();
	void end();
	void play();
	void stop();
	void gotostart();
	void pause();
	void setGain(float gain);
	void setPosition(double pos);
	double getPosition() const;
	double getLength() const;
	bool getMuteState() const { return isMuted; }
	void toggleMute();
	void setMuted(bool shouldBeMuted);

private:
	bool isMuted = false;
	float previousGain = 1.0f;
	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
	// JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
