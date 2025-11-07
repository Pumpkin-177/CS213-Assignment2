#include <JuceHeader.h>
#include "PlayerAudio.h"
#include <vector>

class MarkerListModel : public juce::ListBoxModel

{
public:
	juce::StringArray& markers;
	std::vector<double>& positions;
	PlayerAudio& playerAudio;
	bool& DarkTheme;

	MarkerListModel(juce::StringArray& m, std::vector<double>& p, PlayerAudio& audio, bool& theme);


	int getNumRows() override;
	void paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
	void selectedRowsChanged(int lastRowSelected) override;

};


class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener,
	public juce::Timer,
	public juce::ListBoxModel,
	public juce::LookAndFeel_V4



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
	void loadLocation(const juce::String& player);
	void saveLocation(const juce::String& player);
	void setupAfterFileLoad(const juce::File& file);
	void SwitchTheme(bool isDark);
	int FindInPlayList(const juce::String& Name);

	juce::File PlayerGUI::getLastFile() const;
	juce::AudioSource* getAudioSource();

	int getNumRows() override;
	void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
	void listBoxItemDoubleClicked(int row, const juce::MouseEvent&) override;

	juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override
	{
		return juce::Component::withDefaultMetrics (juce::FontOptions{25.0f});
	};

private:
	PlayerAudio playerAudio;


	bool DarkTheme = true;
	int loopingState = 0; 


	juce::TextButton loadButton{ "Load" };
	juce::TextButton restartButton{ "Restart" };
	juce::TextButton stopButton{ "Stop" };
	juce::TextButton playButton{ "Play" };
	juce::TextButton endButton{ "End >|" };
	juce::TextButton gotostartButton{ "|< Start" };
	juce::TextButton pauseButton{ "Pause" };
	juce::TextButton forwardButton{ " 10s-> " };
	juce::TextButton backwardButton{ " <-10s " };
	juce::TextButton LoadPlayList{ "PlayList", "Load files into a PlayList."};


	juce::TextButton muteButton{ "Mute" };
	juce::TextButton LoopingButton{ "Loop Not", "Toggle between repeating the track, repeating the playlist, or repeating not."};
	juce::TextButton SegmentButton{ "Segment Mode", "Set a playback Segment of the track to either play once or repeat."};

	juce::TextButton Marker{ "M", "Set a marker at the current playback position."};


	juce::ShapeButton ShowMarkers{ "",
	juce::Colours::grey, juce::Colours::lightgrey, juce::Colours::darkgrey};


	juce::ListBox Markerlist;
	juce::StringArray MarkersName;
	std::vector<double> markers_pos;

	juce::Label titleButton;
	juce::Label ArtistButton;
	juce::Label DurationButton;
	juce::Label SegmentTime;

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

	juce::TooltipWindow tooltipWindow{ this, 500 };


	bool wasPlayingBeforeDrag = false;
	bool wasPlayingBeforeEnd = false;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};


