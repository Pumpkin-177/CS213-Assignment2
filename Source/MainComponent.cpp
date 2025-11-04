#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
	addAndMakeVisible(player2);
    setAudioChannels(0, 2);
    player1.loadLocation();
	player2.loadLocation();
    player1.setupAfterFileLoad(player1.getLastFile());
	player2.setupAfterFileLoad(player2.getLastFile());
}

MainComponent::~MainComponent()
{
    player1.saveLocation();
	player2.saveLocation();
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixersource.addInputSource(player1.getAudioSource(), false);
    mixersource.addInputSource(player2.getAudioSource(), false);
    mixersource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixersource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    mixersource.releaseResources();
}

void MainComponent::resized()
{
    player1.setBounds(10, 10, (getWidth() / 2) - 20, getHeight() - 20);
    player2.setBounds((getWidth() / 2) + 10, 10, (getWidth() / 2) - 20, getHeight() - 20);
}
