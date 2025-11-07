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
						&SegmentButton})
	{
		btn->addListener(this);
		addAndMakeVisible(btn);
		btn->setLookAndFeel(this);

	}
	Marker.addListener(this);
	addAndMakeVisible(Marker);

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
			return juce::String::formatted("%02d:%02d", minutes, seconds);};

	titleButton.setText("Title: N/A", juce::dontSendNotification);
	ArtistButton.setText("Artist: N/A", juce::dontSendNotification);
	DurationButton.setText("Duration: 00:00", juce::dontSendNotification);

	SegmentTime.setText("00:00 | 00:00", juce::dontSendNotification);

	addAndMakeVisible(titleButton);
	addAndMakeVisible(ArtistButton);
	addAndMakeVisible(DurationButton);

	addAndMakeVisible(SegmentTime);

	addAndMakeVisible(Marker);
	Marker.addListener(this);

	juce::Path triangle;
	triangle.startNewSubPath(7.5f, 20.0f);
	triangle.lineTo(15.0f, 5.0f);
	triangle.lineTo(0.0f, 5.0f);
	triangle.closeSubPath();
	ShowMarkers.setShape(triangle, true, true, false);
	ShowMarkers.addListener(this);
	addAndMakeVisible(ShowMarkers);

	ShowMarkers.setColours(
		juce::Colour::fromRGB(40, 51, 56),
		juce::Colour::fromRGB(60, 71, 76),
		juce::Colour::fromRGB(3, 150, 90)
	);
	ShowMarkers.setOutline(juce::Colour::fromRGB(200, 200, 200), 0.5f);
	ShowMarkers.setTooltip("View all markers position on the track.");



	addAndMakeVisible(Markerlist);
	auto* model = new MarkerListModel(MarkersName, markers_pos, playerAudio, DarkTheme);
	Markerlist.setModel(model);
	Markerlist.updateContent();
	Markerlist.setVisible(false);
	Markerlist.getVerticalScrollBar().setColour(juce::ScrollBar::thumbColourId, juce::Colour::fromRGB(3, 150, 90));

	SegmentSliderStart.addListener(this);
	addAndMakeVisible(SegmentSliderStart);
	SegmentSliderStart.setVisible(false);
	SegmentSliderStart.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);


	SegmentSliderEnd.addListener(this);
	addAndMakeVisible(SegmentSliderEnd);
	SegmentSliderEnd.setVisible(false);
	SegmentSliderEnd.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

	addAndMakeVisible(playlist);
	playlist.setRowHeight(30);
	playlist.setColour(juce::ListBox::backgroundColourId, juce::Colour::fromRGB(50, 60, 70));
	playlist.setColour(juce::ListBox::textColourId, juce::Colours::white);
	playlist.setVisible(false);
	playlist.getVerticalScrollBar().setColour(juce::ScrollBar::thumbColourId, juce::Colour::fromRGB(3, 150, 90));


	startTimerHz(30);
	formatManager.registerBasicFormats();

	getLookAndFeel().setColour(juce::TooltipWindow::backgroundColourId, juce::Colour::fromRGB(3,150,90));
	getLookAndFeel().setColour(juce::TooltipWindow::outlineColourId, juce::Colour::fromRGB(3, 150, 90));

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
	// === Base constants ===
	const int baseX = 10;
	const int baseY1 = 255;
	const int baseY2 = baseY1 + 60;
	const int buttonWidth = 105;
	const int buttonHeight = 55;
	const int spacing = 16;
	const int extraW1 = 20;
	const int extraW2 = 35;

	// === First row ===
	backwardButton.setBounds(baseX, baseY1, buttonWidth + extraW2, buttonHeight - 5);
	muteButton.setBounds(baseX + buttonWidth + 47, baseY1, buttonWidth + 15, buttonHeight - 5);
	loadButton.setBounds(baseX + 2 * buttonWidth + 55 + 10+10, baseY1, buttonWidth + extraW1 + 15, buttonHeight - 5);
	LoadPlayList.setBounds(baseX + 3 * buttonWidth + 85 + 2 * 15 + 7, baseY1, buttonWidth + 15, buttonHeight - 5);
	forwardButton.setBounds(baseX + 4 * buttonWidth + 95 + 3*15+10, baseY1, buttonWidth + extraW2, buttonHeight - 5);

	// === Second row (main controls) ===
	gotostartButton.setBounds(baseX, baseY2, buttonWidth, buttonHeight);
	stopButton.setBounds(baseX + (buttonWidth + spacing), baseY2, buttonWidth, buttonHeight);
	playButton.setBounds(baseX + 2 * (buttonWidth + spacing), baseY2, buttonWidth, buttonHeight);
	pauseButton.setBounds(baseX + 3 * (buttonWidth + spacing), baseY2, buttonWidth, buttonHeight);
	restartButton.setBounds(baseX + 4 * (buttonWidth + spacing), baseY2, buttonWidth, buttonHeight);
	endButton.setBounds(baseX + 5 * (buttonWidth + spacing), baseY2, buttonWidth, buttonHeight);

	const int metaY = baseY2 + buttonHeight + 120;
	DurationButton.setBounds(10, metaY + 7, 150, 25);
	ArtistButton.setBounds(135, metaY + 7, 150, 25);
	titleButton.setBounds(5, -12, getWidth() - 60, 33);

	SegmentTime.setBounds(10, 215, 120, 20);

	// === Sliders ===
	const int sliderX = 10;
	const int sliderWidth = 710;
	const int sliderHeight = 20;
	const int sliderSpacing = 30;
	const int sliderBaseY = baseY2 + buttonHeight + 20;

	progressSlider.setBounds(sliderX, sliderBaseY, sliderWidth + sliderX, sliderHeight);
	speedSlider.setBounds(sliderX, sliderBaseY + sliderSpacing, sliderWidth + sliderX, sliderHeight);
	volumeSlider.setBounds(sliderX, sliderBaseY + 2 * sliderSpacing, sliderWidth + sliderX, sliderHeight);

	SegmentSliderStart.setBounds(0, 23, 730, 20);
	SegmentSliderEnd.setBounds(0, 187, 730, 20);

	playlist.setBounds(10, metaY + 65, 730-20, 160);

	LoopingButton.setBounds(730 - 170, 40 + sliderBaseY + 2 * sliderSpacing, 160, 40);

	SegmentButton.setBounds(730 - 350, 40 + sliderBaseY + 2 * sliderSpacing, 160, 40);

	Marker.setBounds(730 - 70, 210, 30, 30);
	ShowMarkers.setBounds(730 - 35, 208, 25, 30);
	Marker.setConnectedEdges(juce::Button::ConnectedOnTop);

	for (auto* btn : { &muteButton, &LoopingButton, &SegmentButton })
	{
		btn->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(105, 105, 105));
		btn->setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(3, 150, 90));
		btn->setColour(juce::TextButton::textColourOffId, juce::Colour::fromRGB(190, 190, 190));
		btn->setClickingTogglesState(true);
	}

	for (auto* s : { &progressSlider, &speedSlider, &volumeSlider, &SegmentSliderStart, &SegmentSliderEnd })
		s->setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(4, 160, 100));

}

void PlayerGUI::loadLocation(const juce::String& player)
{
	playerAudio.loadLocation(player);
}

void PlayerGUI::saveLocation(const juce::String& player)
{
	playerAudio.saveLocation(player);
}

juce::File PlayerGUI::getLastFile() const {
	return playerAudio.getLastFile();
}

juce::AudioSource* PlayerGUI::getAudioSource() {
	return playerAudio.getAudioSource();
}


int PlayerGUI::getNumRows()
{
	return playlistFiles.size();
}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
		g.fillAll(DarkTheme ? juce::Colour::fromRGB(45, 55, 60) : juce::Colour::fromRGB(210, 215, 220));

		if (rowIsSelected){
			g.fillAll(juce::Colour::fromRGB(3, 150, 90));
			g.setColour(juce::Colours::white);
		}
		else{g.setColour(DarkTheme ? juce::Colour::fromRGB(240, 240, 240) : juce::Colour::fromRGB(10, 10, 10));}

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
		auto file = playlistFiles[row];
		playerAudio.loadFile(file);
		setupAfterFileLoad(file);
	}
}


MarkerListModel::MarkerListModel(juce::StringArray& m, std::vector<double>& p, PlayerAudio& audio, bool& theme)
	: markers(m), positions(p), playerAudio(audio), DarkTheme(theme)
{
}

int MarkerListModel::getNumRows()
{
	return markers.size();
}

void MarkerListModel::paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool rowIsSelected)
{

    g.fillAll(DarkTheme ? juce::Colour::fromRGB(50, 60, 71)
                        : juce::Colour::fromRGB(210, 215, 220));

    if (rowIsSelected)
    {
        g.fillAll(juce::Colour::fromRGB(3, 150, 90));
        g.setColour(juce::Colours::white);
    }
    else
    {
        g.setColour(DarkTheme ? juce::Colour::fromRGB(240, 240, 240)
                              : juce::Colour::fromRGB(10, 10, 10));
    }

    if (row >= 0 && row < markers.size())
    {
        g.drawText(markers[row],
                   5, 0, width - 10, height,
                   juce::Justification::horizontallyCentred);
    }



}

void MarkerListModel::selectedRowsChanged(int lastRowSelected)
{
	if (lastRowSelected >= 0 && lastRowSelected < positions.size())
	{
		playerAudio.setPosition(positions[lastRowSelected]);
	}
}

int PlayerGUI::FindInPlayList(const juce::String& Name) {

	int currentIndex = -1;
	for (int i = 0; i < playlistFiles.size(); ++i)
	{
		juce::String title = playlistFiles[i].getFileNameWithoutExtension();
		if (title == Name)
		{
			currentIndex = i;
			break;
		}
	}
	return currentIndex;
}


void PlayerGUI::setupAfterFileLoad(const juce::File & file){

	progressSlider.setRange(0.0, playerAudio.getLength());
	progressSlider.setValue(0.0);

	SegmentSliderStart.setRange(0.0, playerAudio.getLength());
	SegmentSliderStart.setValue(0.0);

	SegmentSliderEnd.setRange(0.0, playerAudio.getLength());
	SegmentSliderEnd.setValue(playerAudio.getLength());

	thumbnail.setSource(new juce::FileInputSource(file));

	markers_pos.clear(); MarkersName.clear();
	Markerlist.updateContent();

	playlist.selectRow(FindInPlayList(playerAudio.getTitle()));
	playlist.repaintRow(FindInPlayList(playerAudio.getTitle()));


	titleButton.setText("Title: " + playerAudio.getTitle(), juce::dontSendNotification);
	ArtistButton.setText("Artist: " + playerAudio.getArtist(), juce::dontSendNotification);
	int totalMinutes = int(playerAudio.getDuration()) / 60;
	int totalSeconds = int(playerAudio.getDuration()) % 60;
	juce::String durationString = juce::String::formatted("Duration: %02d:%02d", totalMinutes, totalSeconds);
	DurationButton.setText(durationString, juce::dontSendNotification);


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
				}
			}
		);
	}

	else if (button == &Marker)
	{
		double current_sec = playerAudio.getPosition();
		int Minutes = int(current_sec) / 60;
		int Seconds = int(current_sec) % 60;
		juce::String current_time = juce::String::formatted("%02d:%02d", Minutes, Seconds);
		juce::String markerName = " (" + current_time + ")";

		std::vector<std::pair<double, juce::String>> combined;

		for (int i = 0; i < markers_pos.size(); i++)
			combined.push_back({ markers_pos[i], MarkersName[i] });

		combined.push_back({ current_sec, markerName });

		std::sort(combined.begin(), combined.end(),
			[](auto& a, auto& b) { return a.first < b.first; });

		combined.erase(
			std::unique(combined.begin(), combined.end(),
				[](const auto& a, const auto& b)
				{
					return std::abs(a.first - b.first) < 0.0001;
				}),
			combined.end());

		markers_pos.clear();
		MarkersName.clear();

		for (auto& p : combined)
		{
			markers_pos.push_back(p.first);
			MarkersName.add(p.second);
		}

		Markerlist.updateContent();
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

				playlist.selectRow(FindInPlayList(playerAudio.getTitle()));
				playlist.repaintRow(FindInPlayList(playerAudio.getTitle()));

				if (playlistFiles.size() > 0) { getTopLevelComponent()->centreWithSize(getTopLevelComponent()->getWidth(), 770); playlist.setVisible(true); }
				else { playlist.setVisible(false); }
				

			});

		if (loopingState == 2 && FindInPlayList(playerAudio.getTitle()) != -1)
		{
			loopingState = 1;
			LoopingButton.setButtonText("Loop");
			LoopingButton.setToggleState(true, juce::dontSendNotification);
			LoopingButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(105, 105, 105));
			LoopingButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(3, 150, 90));
			playerAudio.ToggleLoopingState(true);
		}
	}

	else if (button == &restartButton)
	{
		playerAudio.setPosition(0.0);
		playerAudio.play();
	}
	else if (button == &stopButton)
	{
		playerAudio.stop();
	}

	else if (button == &playButton) { playerAudio.play(); }

	else if (button == &endButton) { playerAudio.end(); }

	else if (button == &gotostartButton) { playerAudio.gotostart(); }

	else if (button == &pauseButton) {playerAudio.pause();}

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
		const int maxStates = (FindInPlayList(playerAudio.getTitle()) != -1) ? 3 : 2;
		loopingState = (loopingState + 1) % maxStates;
		if (loopingState == 2 && FindInPlayList(playerAudio.getTitle()) != -1)
		{
			LoopingButton.setButtonText("Loop All");
			playerAudio.ToggleLoopingState(false);
			LoopingButton.setToggleState(false, juce::dontSendNotification); 
			LoopingButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(0, 255, 156));
			LoopingButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(0, 255, 156));

		}
		else{
			LoopingButton.getToggleState() ? playerAudio.ToggleLoopingState(true) : playerAudio.ToggleLoopingState(false);
			LoopingButton.setButtonText(LoopingButton.getToggleState() ? "Loop" : "Loop Not");
			LoopingButton.setColour(juce::TextButton::buttonColourId, DarkTheme ? juce::Colour::fromRGB(105, 105, 105) : juce::Colour::fromRGB(185, 185, 185));
			LoopingButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(3, 150, 90));

		}
	}

	else if (button == &SegmentButton) {
		SegmentSliderStart.setVisible(!SegmentSliderStart.isVisible());
		SegmentSliderStart.setValue(0.0);
		SegmentSliderEnd.setVisible(!SegmentSliderEnd.isVisible());
		SegmentSliderEnd.setValue(playerAudio.getLength());
	}

	else if (button == &ShowMarkers)
	{
		auto content = std::make_unique<juce::Component>();
		content->setSize(100, 100);

		content->addAndMakeVisible(Markerlist);
		Markerlist.setBounds(0, 0, 100, 100);

		juce::CallOutBox::launchAsynchronously(std::move(content), ShowMarkers.getScreenBounds(), nullptr);
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
	else if (slider == &speedSlider) { playerAudio.setSpeed(speedSlider.getValue());}
	else if (slider == &progressSlider) {playerAudio.setPosition(progressSlider.getValue());}
	else if (slider == &SegmentSliderEnd)
	{	if (SegmentSliderStart.getValue() > SegmentSliderEnd.getValue())
		{SegmentSliderStart.setValue(SegmentSliderEnd.getValue() - 0.1, juce::dontSendNotification);}
		repaint(); }

	else if (slider == &SegmentSliderStart)
	{	if (SegmentSliderStart.getValue() > SegmentSliderEnd.getValue())
		{SegmentSliderEnd.setValue(SegmentSliderStart.getValue() + 0.1, juce::dontSendNotification);}
		repaint(); }


}

void PlayerGUI::timerCallback()
{
	auto currentPosition = playerAudio.getPosition();
	auto length = playerAudio.getLength();

	if (length > 0.0)
	{
		progressSlider.setEnabled(true);
		progressSlider.setValue(currentPosition, juce::dontSendNotification);

		double start = SegmentButton.getToggleState() ? SegmentSliderStart.getValue() : 0.0;
		double end = SegmentButton.getToggleState() ? SegmentSliderEnd.getValue() : length;

		if (SegmentButton.getToggleState())
		{

			if (currentPosition < start)
			{
				currentPosition = start;
				playerAudio.setPosition(currentPosition);
			}

			else if (currentPosition >= end) {
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
				playerAudio.setPosition(currentPosition);
			}


		}

		else if (!playerAudio.isPlaying() && currentPosition >= length - 0.001 && loopingState == 2 && playlistFiles.size() > 0)
		{
			int index = FindInPlayList(playerAudio.getTitle());
			if (index != -1)
			{
				int nextIndex = (index + 1) % playlistFiles.size();
				playerAudio.loadFile(playlistFiles[nextIndex]);
				setupAfterFileLoad(playlistFiles[nextIndex]);
				playerAudio.play();
			}	

			else {
				playerAudio.loadFile(playlistFiles[0]);
				setupAfterFileLoad(playlistFiles[0]);
				playerAudio.play();
			}

		}

		auto formatTime = [](double seconds)
			{
				int totalSeconds = static_cast<int>(seconds);
				int minutes = totalSeconds / 60;
				int secs = totalSeconds % 60;
				return juce::String::formatted("%02d:%02d", minutes, secs);
			};

		juce::String startText = formatTime(start);
		juce::String endText = formatTime(end);

		SegmentTime.setText(startText + " | " + endText, juce::dontSendNotification);

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
	double currentPosition = playerAudio.getPosition();
	double length = playerAudio.getLength();

	juce::Rectangle<int> thumbnailArea(10, 40, 710, 150);
	g.setColour(
		(length > 0.0)
		? (DarkTheme ? juce::Colour::fromRGB(56, 68, 77)
			: juce::Colour::fromRGB(235, 237, 240))
		: (DarkTheme ? juce::Colour::fromRGB(50, 60, 70)
			: juce::Colour::fromRGB(216, 221, 227)));

	g.fillRect(thumbnailArea);
	g.setColour(juce::Colour::fromRGB(22, 171, 110));
	thumbnail.drawChannels(g, thumbnailArea.reduced(2), 0.0, playerAudio.getLength(), volumeSlider.getValue() *1.0f);
	thumbnail.drawChannels(g, thumbnailArea.reduced(2), 0.0, playerAudio.getLength(), volumeSlider.getValue() * 1.0f);
	thumbnail.drawChannels(g, thumbnailArea.reduced(2), 0.0, playerAudio.getLength(), volumeSlider.getValue() * 1.0f);
	thumbnail.drawChannels(g, thumbnailArea.reduced(2), 0.0, playerAudio.getLength(), volumeSlider.getValue() * 1.0f);

	if (length > 0.0)
	{

		float drawPosition = (float)(currentPosition / length) * (float)thumbnailArea.getWidth() + (float)thumbnailArea.getX();
		g.setColour(juce::Colour::fromRGB(100, 60, 70));
		g.drawLine(drawPosition, (float)thumbnailArea.getY(), drawPosition, (float)thumbnailArea.getBottom(), 2.0f);


		for (int i = 0; i < markers_pos.size(); ++i)
		{
			double markerPos = markers_pos[i];
			float x = static_cast<float>(((markerPos / playerAudio.getLength()) * 710)+10);

			g.setColour(DarkTheme ? juce::Colour::fromRGB(22, 171, 110) : juce::Colour::fromRGB(22, 171, 110));
			g.drawLine(x, 151.0f, x, (float)thumbnailArea.getBottom(), 1.5f);

			g.setColour(DarkTheme ? juce::Colour::fromRGB(2, 130, 70) : juce::Colour::fromRGB(2, 130, 70));
			g.drawLine(x, 184.0f, x, (float)thumbnailArea.getBottom(), 6.0f);


		}

		if (SegmentButton.getToggleState())
		{

			/*g.setColour(juce::Colours::green);
			g.drawLine(startX, (float)thumbnailArea.getY(), startX, (float)thumbnailArea.getBottom(), 2.0f);

			g.setColour(juce::Colours::green);
			g.drawLine(endX, (float)thumbnailArea.getY(), endX, (float)thumbnailArea.getBottom(), 2.0f);*/

			juce::Rectangle<int> SegmentAreaStart(0, 40, 10 + 710 * (SegmentSliderStart.getValue() / playerAudio.getLength()), 150);
			juce::Rectangle<int> SegmentAreaEnd(710 * ((SegmentSliderEnd.getValue() / playerAudio.getLength())) + 10
				,
				40, 710 - (710 * ((SegmentSliderEnd.getValue() / playerAudio.getLength()))),
				150);

			g.setColour( DarkTheme ? juce::Colour::fromRGBA(56, 68, 77, 200) : juce::Colour::fromRGBA(235, 237, 240, 200));
			g.fillRect(SegmentAreaStart);
			g.fillRect(SegmentAreaEnd);

		}

		
	}

}


void PlayerGUI::SwitchTheme(bool isDark)
{
	DarkTheme = !isDark;

	for (auto* btn : { &loadButton, &LoadPlayList,
					&restartButton ,
					&stopButton, &pauseButton, &playButton,
					&backwardButton ,&forwardButton,
					&endButton , &gotostartButton,
					&Marker }) {

		if (isDark)
		{
			btn->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(255, 255, 255));
			btn->setColour(juce::TextButton::textColourOffId, juce::Colour::fromRGB(30, 35, 40));

		}
		else
		{
			btn->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(40, 51, 56));
			btn->setColour(juce::TextButton::textColourOffId, juce::Colour::fromRGB(246, 246, 246));
		}

	}


	for (auto* btn : { &SegmentButton, &muteButton, &LoopingButton }) {

		if (isDark)
		{
			btn->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(185, 185, 185));
			btn->setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(30, 180, 120));
			btn->setColour(juce::TextButton::textColourOffId, juce::Colour::fromRGB(80, 85, 90));

		}
		else
		{
			btn->setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(105, 105, 105));
			btn->setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(3, 150, 90));
			btn->setColour(juce::TextButton::textColourOffId, juce::Colour::fromRGB(190, 190, 190));

		}

	}

	for (auto* slider : { &volumeSlider, &progressSlider, &speedSlider,
							&SegmentSliderStart, &SegmentSliderEnd }) {


		if (isDark)
		{
			slider->setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(1, 120, 60));
			slider->setColour(juce::Slider::trackColourId, juce::Colour::fromRGB(140, 140, 140));
			slider->setColour(juce::Slider::backgroundColourId, juce::Colour::fromRGB(180, 180, 180));
			slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromRGB(125, 125, 125));
			slider->setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(40, 45, 50));


		}
		else
		{
			slider->setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(5, 180, 120));
			slider->setColour(juce::Slider::trackColourId, juce::Colour::fromRGB(24, 31, 34));
			slider->setColour(juce::Slider::backgroundColourId, juce::Colour::fromRGB(38, 50, 56));
			slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour::fromRGB(142, 152, 155));
			slider->setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(228, 229, 230));

		}
	}

	for (auto* box : { &playlist, &Markerlist })
	{
		if (isDark)
		{
			box->setColour(juce::ListBox::backgroundColourId, juce::Colour::fromRGB(216, 221, 227));  
   
			box->setColour(juce::ListBox::textColourId, juce::Colour::fromRGB(40, 45, 50)); 
		}
		else
		{
			box->setColour(juce::ListBox::backgroundColourId, juce::Colour::fromRGB(50, 60, 70));

			box->setColour(juce::ListBox::textColourId, juce::Colour::fromRGB(228, 229, 230));   
		}
	}

	for (auto* label : { &titleButton, &ArtistButton, &DurationButton, &SegmentTime })
	{
		if (isDark)
		{
			label->setColour(juce::Label::textColourId, juce::Colours::black);
		}
		else
		{
			label->setColour(juce::Label::textColourId, juce::Colours::white);
		}
	}

	ShowMarkers.setColours(
		DarkTheme ? juce::Colour::fromRGB(40, 51, 56) : juce::Colour::fromRGB(255, 255, 255),
		DarkTheme ? juce::Colour::fromRGB(60, 71, 76) : juce::Colour::fromRGB(225, 225, 225),
		juce::Colour::fromRGB(3, 150, 90));

	ShowMarkers.setOutline(DarkTheme ? juce::Colour::fromRGB(200, 200, 200) : juce::Colour::fromRGB(20, 20, 20), 0.5f);

	repaint();
}