#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    formatManager.registerBasicFormats();

    for (auto* btn : { &loadButton, &restartButton, &stopButton, &pauseButton, &playButton,
                       &backwardButton, &forwardButton, &endButton, &gotostartButton,
                       &muteButton, &LoopingButton, &Marker, &View_marker, &LoadPlayList })
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

    timeLabel.setText("00:00 / 00:00", juce::dontSendNotification);
    addAndMakeVisible(timeLabel);

    titleButton.setText("Title: N/A", juce::dontSendNotification);
    ArtistButton.setText("Artist: N/A", juce::dontSendNotification);
    DurationButton.setText("Duration: 00:00", juce::dontSendNotification);

    addAndMakeVisible(titleButton);
    addAndMakeVisible(ArtistButton);
    addAndMakeVisible(DurationButton);

    startTimerHz(10);

    addAndMakeVisible(Marker);
    Marker.addListener(this);
    addAndMakeVisible(Markerlist);
    View_marker.addListener(this);

    auto* model = new MarkerListModel(MarkersName, markers_pos, playerAudio);
    Markerlist.setModel(model);
    Markerlist.updateContent();

    addAndMakeVisible(playlist);
    playlist.setRowHeight(20);
    playlist.setColour(juce::ListBox::backgroundColourId, juce::Colours::black);
    playlist.setColour(juce::ListBox::textColourId, juce::Colours::white);

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

    loadButton.setBounds((getWidth() / 2) - (5 * y / 2), 250, 5 * y, 2 * y);

    gotostartButton.setBounds((getWidth() / 2) - 260, x + 130, 80, 45);
    stopButton.setBounds((getWidth() / 2) - 170, x + 130, 80, 45);
    playButton.setBounds((getWidth() / 2) - 80, x + 130, 80, 45);
    pauseButton.setBounds((getWidth() / 2) + 10, x + 130, 80, 45);
    restartButton.setBounds((getWidth() / 2) + 100, x + 130, 80, 45);
    endButton.setBounds((getWidth() / 2) + 190, x + 130, 80, 45);

    muteButton.setBounds((getWidth() / 2) - (5 * y / 2) - 100, 250, 80, 40);
    muteButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(25, 25, 25));
    muteButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(30, 215, 96));
    muteButton.setClickingTogglesState(true);

    LoopingButton.setBounds((getWidth() / 2) + (5 * y / 2) + 20, 250, 80, 40);
    LoopingButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(25, 25, 25));
    LoopingButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(30, 215, 96));
    LoopingButton.setClickingTogglesState(true);

    volumeSlider.setBounds(20, (getHeight() / 2) + 70, getWidth() - 40, 20);
    speedSlider.setBounds(20, (getHeight() / 2) + 100, getWidth() - 40, 20);
    progressSlider.setBounds(20, (getHeight() / 2) + 130, getWidth() - 40, 20);
    timeLabel.setBounds(20, (getHeight() / 2) + 155, getWidth() - 40, 20);

    backwardButton.setBounds((getWidth() / 2) - 130, (getHeight() / 1.25) + 10, 120, 35);
    forwardButton.setBounds((getWidth() / 2) + 10, (getHeight() / 1.25) + 10, 120, 35);

    int infoY = (getHeight() / 2) + 180;
    titleButton.setBounds(20, infoY, getWidth() - 40, 25);
    ArtistButton.setBounds(20, infoY + 25, getWidth() - 40, 25);
    DurationButton.setBounds(20, infoY + 50, getWidth() - 40, 25);

    backwardButton.toFront(false);
    forwardButton.toFront(false);

    Marker.setBounds(20, 240, 120, 140);
    Markerlist.setBounds(getWidth() - 170, 240, 150, 140);

    LoadPlayList.setBounds(530, 440, 160, 40);
    playlist.setBounds(10, 440, 500, 100);

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
        thumbnail.setSource(new juce::FileInputSource(f));
    }
}

void PlayerGUI::setupAfterFileLoad(const juce::File& file) {
    progressSlider.setRange(0.0, playerAudio.getLength());
    progressSlider.setValue(0.0);
    thumbnail.setSource(new juce::FileInputSource(file));
}


void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...", juce::File{}, "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    thumbnail.clear();
                    playerAudio.loadFile(file);
                    progressSlider.setRange(0.0, playerAudio.getLength());
                    progressSlider.setValue(0.0);
                    thumbnail.setSource(new juce::FileInputSource(file));
                    repaint();

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


    else if (button == &restartButton) playerAudio.setPosition(0.0), playerAudio.play();
    else if (button == &stopButton) playerAudio.stop(), playerAudio.setPosition(0.0);
    else if (button == &playButton) playerAudio.play();
    else if (button == &pauseButton) playerAudio.pause();
    else if (button == &endButton) playerAudio.end();
    else if (button == &gotostartButton) playerAudio.gotostart();
    else if (button == &forwardButton) playerAudio.skipForward(10.0);
    else if (button == &backwardButton) playerAudio.skipBackward(10.0);
    else if (button == &muteButton)
    {
        playerAudio.toggleMute();
        muteButton.setButtonText(playerAudio.getMuteState() ? "Unmute" : "Mute");
    }
    else if (button == &LoopingButton) playerAudio.ToggleLoopingState();
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider) playerAudio.setGain((float)volumeSlider.getValue());
    else if (slider == &speedSlider) playerAudio.setSpeed(speedSlider.getValue());
    else if (slider == &progressSlider) playerAudio.setPosition(progressSlider.getValue());
}

void PlayerGUI::timerCallback()
{
    auto currentPosition = playerAudio.getPosition();
    auto length = playerAudio.getLength();
    if (length > 0.0)
    {
        progressSlider.setValue(currentPosition, juce::dontSendNotification);
        int currentMinutes = int(currentPosition) / 60;
        int currentSeconds = int(currentPosition) % 60;
        int totalMinutes = int(length) / 60;
        int totalSeconds = int(length) % 60;
        juce::String timeString = juce::String::formatted("%02d:%02d / %02d:%02d",
            currentMinutes, currentSeconds, totalMinutes, totalSeconds);
        timeLabel.setText(timeString, juce::dontSendNotification);
    }
    else
    {
        progressSlider.setValue(0.0, juce::dontSendNotification);
        timeLabel.setText("00:00 / 00:00", juce::dontSendNotification);
    }
    repaint();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(18, 18, 18));

    juce::Rectangle<int> thumbnailArea(10, 10, getWidth() - 20, getHeight() / 3);
    g.setColour(juce::Colour::fromRGB(25, 25, 25));
    g.fillRect(thumbnailArea);
    g.setColour(juce::Colour::fromRGB(30, 215, 96));
    thumbnail.drawChannels(g, thumbnailArea.reduced(2), 0.0, playerAudio.getLength(), 1.0f);

    double currentPosition = playerAudio.getPosition();
    double length = playerAudio.getLength();
    if (length > 0.0)
    {
        float drawPosition = (float)(currentPosition / length) * (float)thumbnailArea.getWidth() + (float)thumbnailArea.getX();
        g.setColour(juce::Colour::fromRGB(30, 215, 96));
        g.drawLine(drawPosition, (float)thumbnailArea.getY(), drawPosition, (float)thumbnailArea.getBottom(), 2.0f);
    }
}
