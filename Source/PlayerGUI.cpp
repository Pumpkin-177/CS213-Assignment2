#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{

	for (auto* btn : { &loadButton, &LoadPlayList,
						&restartButton ,
						&stopButton, &pauseButton, &playButton,
						&backwardButton ,&forwardButton,
						&endButton , &gotostartButton,
						&muteButton,
						&LoopingButton,
						&SegmentButton,
						&Marker,&View_marker })
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

	progressSlider.addListener(this);
	addAndMakeVisible(progressSlider);
	progressSlider.textFromValueFunction = [](double value)
		{	int totalSeconds = static_cast<int>(value);
	int minutes = totalSeconds / 60; int seconds = totalSeconds % 60;
	return juce::String::formatted("%02d:%02d", minutes, seconds); };

	titleButton.setText("Title: N/A", juce::dontSendNotification);
	ArtistButton.setText("Artist: N/A", juce::dontSendNotification);
	DurationButton.setText("Duration: 00:00", juce::dontSendNotification);

	addAndMakeVisible(titleButton);
	addAndMakeVisible(ArtistButton);
	addAndMakeVisible(DurationButton);

	addAndMakeVisible(Marker);
	Marker.addListener(this);
	addAndMakeVisible(Markerlist);
	View_marker.addListener(this);

	auto* model = new MarkerListModel(MarkersName, markers_pos, playerAudio);
	Markerlist.setModel(model);
	Markerlist.updateContent();

	SegmentSliderStart.addListener(this);
	addAndMakeVisible(SegmentSliderStart);
	SegmentSliderStart.setVisible(false);
	SegmentSliderStart.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);


	SegmentSliderEnd.addListener(this);
	addAndMakeVisible(SegmentSliderEnd);
	SegmentSliderEnd.setVisible(false);
	SegmentSliderEnd.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(playlist);
	playlist.setRowHeight(20);
	playlist.setColour(juce::ListBox::backgroundColourId, juce::Colours::black);
	playlist.setColour(juce::ListBox::textColourId, juce::Colours::white);

	startTimerHz(10);
	formatManager.registerBasicFormats();
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

	int BLY1 = 220;
	int BLY2 = BLY1 + 50;
	int BLX = 130;

	int W = 80;
	int H = 45;

	loadButton.setBounds(BLX + 2 * W + 55, BLY1, W + 20, H - 5);

	gotostartButton.setBounds(BLX, BLY2, W, H);
	stopButton.setBounds(BLX + W + 10, BLY2, W, H);
	playButton.setBounds(BLX + 2 * (W + 10), BLY2, W, H);
	pauseButton.setBounds(BLX + 3 * (W + 10), BLY2, W, H);
	restartButton.setBounds(BLX + 4 * (W + 10), BLY2, W, H);
	endButton.setBounds(BLX + 5 * (W + 10), BLY2, W, H);

	muteButton.setBounds(BLX + W + 45, BLY1, W, H - 5);
	muteButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(105, 105, 105));
	muteButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(3, 150, 90));
	muteButton.setClickingTogglesState(true);

	LoopingButton.setBounds(BLX + 3 * W + 85, BLY1, W, H - 5);
	LoopingButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(105, 105, 105));
	LoopingButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(3, 150, 90));
	LoopingButton.setClickingTogglesState(true);

	titleButton.setBounds(300, BLY2 + H + 120, 100, 25);
	ArtistButton.setBounds(140, BLY2 + H + 120, 100, 25);
	DurationButton.setBounds(15, BLY2 + H + 120, 100, 25);

	Marker.setBounds(10, 210, 100, 80);
	Markerlist.setBounds(getWidth() - 110, 210, 100, 120);

	SegmentButton.setBounds(530, 530, 160, 40);
	SegmentButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(105, 105, 105));
	SegmentButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(3, 150, 90));
	SegmentButton.setClickingTogglesState(true);

	backwardButton.setBounds(BLX, BLY1, W + 35, H - 5);
	forwardButton.setBounds(BLX + 4 * W + 95, BLY1, W + 35, H - 5);

	LoadPlayList.setBounds(530, 470, 160, 40);


	progressSlider.setBounds(20, BLY2 + H + 20, 750, 20);
	speedSlider.setBounds(20, BLY2 + H + 50, 750, 20);
	volumeSlider.setBounds(20, BLY2 + H + 80, 750, 20);

	SegmentSliderStart.setBounds(0, 5, 780, 20);
	SegmentSliderEnd.setBounds(0, 185, 780, 20);

	playlist.setBounds(10, 470, 500, 100);

}

void PlayerGUI::loadLocation()
{
	playerAudio.loadLocation();
}

void PlayerGUI::saveLocation()
{
	playerAudio.saveLocation();
}

juce::File PlayerGUI::getLastFile() const {
	return playerAudio.getLastFile();
}


int PlayerGUI::getNumRows()
{
	return playlistFiles.size();
}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
	if (rowIsSelected)
		g.fillAll(juce::Colours::darkslategrey);

	g.setColour(juce::Colours::white);
	if (rowNumber >= 0 && rowNumber < playlistFiles.size())
	{
		g.drawText(playlistFiles[rowNumber].getFileName(),
			5, 0, width - 10, height,
			juce::Justification::centredLeft);
	}
}

void PlayerGUI::listBoxItemDoubleClicked(int row, const juce::MouseEvent&)
{
	if (row >= 0 && row < playlistFiles.size())
	{
		auto f = playlistFiles[row];
		playerAudio.loadFile(f);
		progressSlider.setRange(0.0, playerAudio.getLength());
		progressSlider.setValue(0.0);
		SegmentSliderStart.setRange(0.0, playerAudio.getLength());
		SegmentSliderEnd.setRange(0.0, playerAudio.getLength());
		thumbnail.setSource(new juce::FileInputSource(f));
	}
}

void PlayerGUI::setupAfterFileLoad(const juce::File& file) {
	progressSlider.setRange(0.0, playerAudio.getLength());
	progressSlider.setValue(0.0);
	SegmentSliderStart.setRange(0.0, playerAudio.getLength());
	SegmentSliderStart.setValue(0.0);
	SegmentSliderEnd.setRange(0.0, playerAudio.getLength());
	SegmentSliderEnd.setValue(playerAudio.getLength());
	thumbnail.setSource(new juce::FileInputSource(file));
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
					setupAfterFileLoad(file);

					titleButton.setText("Title: " + playerAudio.getTitle(), juce::dontSendNotification);
					ArtistButton.setText("Artist: " + playerAudio.getArtist(), juce::dontSendNotification);

					int totalMinutes = int(playerAudio.getDuration()) / 60;
					int totalSeconds = int(playerAudio.getDuration()) % 60;
					juce::String durationString = juce::String::formatted("Duration: %02d:%02d", totalMinutes, totalSeconds);
					DurationButton.setText(durationString, juce::dontSendNotification);

				}
			}
		);
	}

	else if (button == &Marker)
	{
		double current_sec = playerAudio.getPosition();
		juce::String markerName = "Marker " + juce::String(markers_pos.size() + 1)
			+ " (" + juce::String(current_sec, 2) + "s)";
		markers_pos.push_back(current_sec);
		MarkersName.add(markerName);
		Markerlist.updateContent();
	}
	else if (button == &View_marker)
	{
		DBG("Markers list:");
		for (int i = 0; i < markers_pos.size(); ++i)
			DBG("Marker " + juce::String(i + 1) + ": " + juce::String(markers_pos[i]) + " seconds");
	}

	else if (button == &LoadPlayList)
	{
		fileChooser = std::make_unique<juce::FileChooser>("Select audio files...", juce::File{}, "*.wav;*.mp3");

		fileChooser->launchAsync(
			juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::canSelectMultipleItems,
			[this](const juce::FileChooser& fc)
			{
				auto files = fc.getResults();

				if (files.isEmpty())
					return;
				playlistFiles = files;

				playlist.updateContent();
			});

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

	else if (button == &SegmentButton) {
		SegmentSliderStart.setVisible(!SegmentSliderStart.isVisible());
		SegmentSliderStart.setValue(0.0);
		SegmentSliderEnd.setVisible(!SegmentSliderEnd.isVisible());
		SegmentSliderEnd.setValue(playerAudio.getLength());
	}

}


void PlayerGUI::sliderDragStarted(juce::Slider* slider)
{
	if (slider == &progressSlider || slider == &SegmentSliderStart || slider == &SegmentSliderEnd)
	{
		if (playerAudio.isPlaying())
		{
			wasPlayingBeforeDrag = true;
		}
		else
		{
			wasPlayingBeforeDrag = false;
		}
		playerAudio.stop();
	}
}

void PlayerGUI::sliderDragEnded(juce::Slider* slider)
{
	if ((slider == &progressSlider || slider == &SegmentSliderStart || slider == &SegmentSliderEnd) && wasPlayingBeforeDrag)
	{
		playerAudio.play();
	}
}


void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &volumeSlider) { playerAudio.setGain((float)volumeSlider.getValue()); }
	else if (slider == &speedSlider) { playerAudio.setSpeed(speedSlider.getValue()); }
	else if (slider == &progressSlider) { playerAudio.setPosition(progressSlider.getValue()); }
	else if (slider == &SegmentSliderEnd)
	{
		if (SegmentSliderStart.getValue() > SegmentSliderEnd.getValue())
		{
			SegmentSliderStart.setValue(SegmentSliderEnd.getValue() - 0.1, juce::dontSendNotification);
		}
		repaint();
	}

	else if (slider == &SegmentSliderStart)
	{
		if (SegmentSliderStart.getValue() > SegmentSliderEnd.getValue())
		{
			SegmentSliderEnd.setValue(SegmentSliderStart.getValue() + 0.1, juce::dontSendNotification);
		}
		repaint();
	}


}

void PlayerGUI::timerCallback()
{
	auto currentPosition = playerAudio.getPosition();
	auto length = playerAudio.getLength();
	if (length > 0.0)
	{
		progressSlider.setEnabled(true);
		progressSlider.setValue(currentPosition, juce::dontSendNotification);

		if (SegmentButton.getToggleState())
		{
			double start = SegmentSliderStart.getValue();
			double end = SegmentSliderEnd.getValue();

			if (currentPosition < start)
				currentPosition = start;
			else if (currentPosition >= end)
				if (LoopingButton.getToggleState())
				{
					currentPosition = start;
					if (wasPlayingBeforeEnd) playerAudio.play();
				}
				else
				{
					currentPosition = end + 1e-4;
					wasPlayingBeforeEnd = true;
					playerAudio.stop();
				}
		}

		playerAudio.setPosition(currentPosition);

	}
	else
	{
		progressSlider.setValue(0.0, juce::dontSendNotification);
		progressSlider.setEnabled(false);
	}
	repaint();
}

void PlayerGUI::paint(juce::Graphics& g)
{
	juce::Rectangle<int> thumbnailArea(10, 30, 800 - 40, 150);
	g.setColour(juce::Colours::black);
	g.fillRect(thumbnailArea);
	g.setColour(juce::Colours::lightblue);
	thumbnail.drawChannels(g, thumbnailArea.reduced(2), 0.0, playerAudio.getLength(), 1.0f);

	double currentPosition = playerAudio.getPosition();
	double length = playerAudio.getLength();
	if (length > 0.0)
	{
		float drawPosition = (float)(currentPosition / length) * (float)thumbnailArea.getWidth() + (float)thumbnailArea.getX();
		g.setColour(juce::Colours::red);
		g.drawLine(drawPosition, (float)thumbnailArea.getY(), drawPosition, (float)thumbnailArea.getBottom(), 4.0f);

		if (SegmentButton.getToggleState())
		{
			float startX = (float)(SegmentSliderStart.getValue() / length) * (float)thumbnailArea.getWidth() + (float)thumbnailArea.getX();
			float endX = (float)(SegmentSliderEnd.getValue() / length) * (float)thumbnailArea.getWidth() + (float)thumbnailArea.getX();

			g.setColour(juce::Colours::green);
			g.drawLine(startX, (float)thumbnailArea.getY(), startX, (float)thumbnailArea.getBottom(), 2.0f);

			g.setColour(juce::Colours::green);
			g.drawLine(endX, (float)thumbnailArea.getY(), endX, (float)thumbnailArea.getBottom(), 2.0f);

			juce::Rectangle<int> SegmentArea(10 + 760 * (SegmentSliderStart.getValue() / playerAudio.getLength()),
				30,
				760 * ((SegmentSliderEnd.getValue() / playerAudio.getLength()) - (SegmentSliderStart.getValue() / playerAudio.getLength())),
				150);
			g.setColour(juce::Colour::fromFloatRGBA(3 / 255.0f, 150 / 255.0f, 90 / 255.0f, 100 / 255.0f));
			g.fillRect(SegmentArea);

		}

	}

}
