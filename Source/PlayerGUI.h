#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI  : public juce::Component,
				   public juce::Button::Listener,
				   public juce::Slider::Listener
{ public:
	  PlayerGUI();
	  ~PlayerGUI() override;
	  
	  void resized() override;
	  void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	  void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	  void releaseResources();

  private:
	  PlayerAudio playerAudio;
	  
	  juce::TextButton loadButton{ "Load" };
	  juce::TextButton restartButton{ "Restart" };
	  juce::TextButton stopButton{ "Stop" };
	  juce::TextButton playButton{ "Play" };
	  juce::TextButton endButton{ "End |>" };
	  juce::TextButton gotostartButton{ "<| Start" };
	  juce::TextButton pauseButton{ "Pause" };
	  juce::TextButton forwardButton{ "10s->" };
	  juce::TextButton backwardButton{ "<-10s" };


	  juce::TextButton muteButton{ "Mute" };
	  juce::TextButton LoopingButton{ "Loop" };


	  juce::Slider volumeSlider;
	  std::unique_ptr<juce::FileChooser> fileChooser;
	  
	  void buttonClicked(juce::Button* button) override;
	  void sliderValueChanged(juce::Slider* slider) override;
	  

	  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI) };
