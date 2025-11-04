#include <JuceHeader.h>
#include "PlayerAudio.h"
#include <vector>

class MarkerListModel : public juce::ListBoxModel
{
public:
	juce::StringArray& markers;
	std::vector<double>& positions;
	PlayerAudio& playerAudio;

	MarkerListModel(juce::StringArray& m, std::vector<double>& p, PlayerAudio& audio)
		: markers(m), positions(p), playerAudio(audio) {
	}

	int getNumRows() override { return markers.size(); }

	void paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool rowIsSelected) override
	{
		if (rowIsSelected)
			g.fillAll(juce::Colour::fromRGB(30, 215, 96));
		else
			g.fillAll(juce::Colour::fromRGB(25, 25, 25));

		g.setColour(juce::Colours::white);
		g.drawText(markers[row], 4, 0, width, height, juce::Justification::centredLeft);
	}

	void selectedRowsChanged(int lastRowSelected) override
	{
		if (lastRowSelected >= 0 && lastRowSelected < positions.size())
		{
			playerAudio.setPosition(positions[lastRowSelected]);
			playerAudio.play();
		}
	}
};

class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener,
	public juce::Timer,
	public juce::ListBoxModel


{
public:
	PlayerGUI();
	~PlayerGUI() override;

	void resized() override;
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();
	void timerCallback();
	void paint(juce::Graphics& g) override;
	void loadLocation();
	void saveLocation();
	void setupAfterFileLoad(const juce::File& file);
	juce::File PlayerGUI::getLastFile() const;



	int getNumRows() override;
	void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
	void listBoxItemDoubleClicked(int row, const juce::MouseEvent&) override;

private:
	PlayerAudio playerAudio;

	juce::TextButton loadButton{ "Load" };
	juce::TextButton restartButton{ "Restart" };
	juce::TextButton stopButton{ "Stop" };
	juce::TextButton playButton{ "Play" };
	juce::TextButton endButton{ "End |>" };
	juce::TextButton gotostartButton{ "<| Start" };
	juce::TextButton pauseButton{ "Pause" };
	juce::TextButton forwardButton{ " 10s-> " };
	juce::TextButton backwardButton{ " <-10s " };
	juce::TextButton LoadPlayList{ "Load a PlayList" };


	juce::TextButton muteButton{ "Mute" };
	juce::TextButton LoopingButton{ "Loop" };
	juce::TextButton SegmentButton{ "Segment Mode" };

	juce::TextButton Marker{ "Add Marker" };
	juce::TextButton View_marker{ "View Markers" };

	juce::ListBox Markerlist;
	juce::StringArray MarkersName;
	std::vector<double> markers_pos;

	juce::Label titleButton;
	juce::Label ArtistButton;
	juce::Label DurationButton;

	juce::Slider volumeSlider;
	juce::Slider speedSlider;
	juce::Slider progressSlider;
	juce::Slider SegmentSliderStart;
	juce::Slider SegmentSliderEnd;

	juce::AudioFormatManager formatManager;
	juce::AudioThumbnailCache thumbnailCache{ 5 };
	juce::AudioThumbnail thumbnail{ 512, formatManager, thumbnailCache };
	
	juce::Array<juce::File> playlistFiles;
	juce::ListBox playlist{ "Playlist", this };

	std::unique_ptr<juce::FileChooser> fileChooser;

	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;
	void sliderDragStarted(juce::Slider* slider) override;
	void sliderDragEnded(juce::Slider* slider) override;

	bool wasPlayingBeforeDrag = false;
	bool wasPlayingBeforeEnd = false;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
