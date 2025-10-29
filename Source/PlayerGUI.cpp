#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{

	for (auto* btn : { &loadButton,
						&restartButton ,
						&stopButton, &pauseButton, &playButton,
						&backwardButton ,&forwardButton,
						&endButton , &gotostartButton,
						&muteButton,
						&LoopingButton })
	{
		btn->addListener(this);
		addAndMakeVisible(btn);
	}

	volumeSlider.setRange(0.0, 1.0, 0.01);
	volumeSlider.setValue(0.5);
	volumeSlider.addListener(this);
	addAndMakeVisible(volumeSlider);

	speedSlider.setRange(0.25, 2.0, 0.01);
	speedSlider.setValue(1.0);
	speedSlider.addListener(this);
	addAndMakeVisible(speedSlider);

}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
	playerAudio.releaseResources();
}

void PlayerGUI::resized()
{
	int y = 20;
	int x = getHeight() / 3.7;
	int z = getHeight() / 2;

	loadButton.setBounds((getWidth() / 2) - (5 * y / 2), 20, 5 * y, 2 * y);

	gotostartButton.setBounds((getWidth() / 2) - 260, x, 80, 45);
	stopButton.setBounds((getWidth() / 2) - 170, x, 80, 45);
	playButton.setBounds((getWidth() / 2) - 80, x, 80, 45);
	pauseButton.setBounds((getWidth() / 2) + 10, x, 80, 45);
	restartButton.setBounds((getWidth() / 2) + 100, x, 80, 45);
	endButton.setBounds((getWidth() / 2) + 190, x, 80, 45);

	muteButton.setBounds(getWidth() - 210, (getHeight() / 2), 80, 40);
	muteButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(105, 105, 105));
	muteButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(3, 150, 90));
	muteButton.setClickingTogglesState(true);

	LoopingButton.setBounds(getWidth() - 110, (getHeight() / 2), 80, 40);
	LoopingButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(105, 105, 105));
	LoopingButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(3, 150, 90));
	LoopingButton.setClickingTogglesState(true);

	backwardButton.setBounds((getWidth() / 2) - 130, (getHeight() / 1.15) - 30, 120, 35);
	forwardButton.setBounds((getWidth() / 2) + 10, (getHeight() / 1.15) - 30, 120, 35);

	volumeSlider.setBounds(20, (getHeight() / 1.6), getWidth() - 40, 20);
	speedSlider.setBounds(20, (getHeight() / 1.42), getWidth() - 40, 20);

}

void PlayerGUI::buttonClicked(juce::Button* button)
{
	if (button == &loadButton)
	{
		juce::FileChooser chooser("Select audio files...", juce::File{}, "*.wav;*.mp3");
		fileChooser = std::make_unique<juce::FileChooser>("Select an audio file...", juce::File{}, "*.wav;*.mp3");

		fileChooser->launchAsync(
			juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
			[this](const juce::FileChooser& fc)
			{
				auto file = fc.getResult();
				if (file.existsAsFile())
				{
					playerAudio.loadFile(file);
				}
			}
		);
	}


	else if (button == &restartButton)
	{
		playerAudio.setPosition(0.0);
		playerAudio.play();
	}
	else if (button == &stopButton)
	{
		playerAudio.stop();
		playerAudio.setPosition(0.0);
	}

	else if (button == &playButton) { playerAudio.play(); }

	else if (button == &endButton) { playerAudio.end(); }

	else if (button == &gotostartButton) { playerAudio.gotostart(); }

	else if (button == &pauseButton) { playerAudio.pause(); }

	else if (button == &forwardButton) playerAudio.skipForward(10.0);

	else if (button == &backwardButton) playerAudio.skipBackward(10.0);


	else if (button == &muteButton) {
		playerAudio.toggleMute();
		if (playerAudio.getMuteState()) {
			muteButton.setButtonText("Unmute");
		}
		else {
			muteButton.setButtonText("Mute");
		}
	}

	else if (button == &LoopingButton) {
		playerAudio.ToggleLoopingState();

	}

}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &volumeSlider) { playerAudio.setGain((float)volumeSlider.getValue()); }
	else if (slider == &speedSlider) { playerAudio.setSpeed(speedSlider.getValue()); }
}
