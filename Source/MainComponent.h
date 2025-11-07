#pragma once 
#include <JuceHeader.h>
#include "PlayerGUI.h"
#include <vector>


class MainComponent : public juce::AudioAppComponent,
					  public juce::Button::Listener,
					  public juce::LookAndFeel_V4

{
public:
	MainComponent();
	~MainComponent() override;

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;
	void resized() override;
	void paint(juce::Graphics& g) override;


	juce::TextButton DualPlayerButton{ "P", "Enable 2-Player mode with independent players."};
	juce::TextButton ThemeChangeButton{ "T", "Switch the UI color theme between light and dark."};

	void buttonClicked(juce::Button* button) override;


private:
	PlayerGUI player1;
	PlayerGUI player2;

	juce::MixerAudioSource mixersource;

	void drawButtonBackground(juce::Graphics& g, juce::Button& button,
		const juce::Colour& backgroundColour,
		bool shouldDrawButtonAsHighlighted,
		bool shouldDrawButtonAsDown) override;

	juce::TooltipWindow tooltipWindow{ this, 500 };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
