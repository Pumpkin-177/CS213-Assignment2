#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);
	player2.setVisible(false);
    setAudioChannels(0, 2);
    player1.loadLocation("location1");
    player2.loadLocation("location2");
    player1.setupAfterFileLoad(player1.getLastFile());
    player2.setupAfterFileLoad(player2.getLastFile());

    DualPlayerButton.setLookAndFeel(this);
    addAndMakeVisible(DualPlayerButton);
    DualPlayerButton.addListener(this);

    ThemeChangeButton.setLookAndFeel(this);
    addAndMakeVisible(ThemeChangeButton);
    ThemeChangeButton.addListener(this);

    getLookAndFeel().setColour(juce::TooltipWindow::backgroundColourId, juce::Colour::fromRGB(3, 150, 90));
    getLookAndFeel().setColour(juce::TooltipWindow::outlineColourId, juce::Colour::fromRGB(3, 150, 90));

}

MainComponent::~MainComponent()
{
	player1.saveLocation("location1");
    player2.saveLocation("location2");
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
    player1.setBounds(10, 10, getWidth() - 20, getHeight() - 20);
    player2.setBounds((getWidth() / 2) + 10, 10, (getWidth() / 2) - 20, getHeight() - 20);

    ThemeChangeButton.setBounds(getWidth()-62, 0, 31, 27);
    ThemeChangeButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGBA(105, 105, 105, 0));
    ThemeChangeButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(3, 150, 90));
    ThemeChangeButton.setClickingTogglesState(true);

    DualPlayerButton.setBounds(getWidth()-31, 0, 31, 27);
    DualPlayerButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGBA(105, 105, 105, 0));
    DualPlayerButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour::fromRGB(3, 150, 90));
    DualPlayerButton.setClickingTogglesState(true);
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if (button == &DualPlayerButton) {
        player2.setVisible(!player2.isVisible());

        if (player2.isVisible())
             getTopLevelComponent()->centreWithSize(750*2, getTopLevelComponent()->getHeight());
        else 
	         getTopLevelComponent()->centreWithSize(750, getTopLevelComponent()->getHeight());
    }

    else if (button == &ThemeChangeButton) 
    {
        auto* window = findParentComponentOfClass<juce::DocumentWindow>();

        player1.SwitchTheme(ThemeChangeButton.getToggleState());
		player2.SwitchTheme(ThemeChangeButton.getToggleState());
        


    }
	repaint();
}

void MainComponent::drawButtonBackground(juce::Graphics& g, juce::Button& button,
    const juce::Colour& backgroundColour,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown) 
{
    auto cornerSize = 0.0f;
    auto bounds = button.getLocalBounds().toFloat();

    auto baseColour = backgroundColour.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.4f : 0.9f)
        .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
        baseColour = (button.getToggleState() ? juce::Colour::fromRGB(2, 140, 80) : juce::Colour::fromRGB(28, 50, 46)); //<--

    g.setColour(baseColour);

    auto flatOnLeft = button.isConnectedOnLeft();
    auto flatOnRight = button.isConnectedOnRight();
    auto flatOnTop = button.isConnectedOnTop();
    auto flatOnBottom = button.isConnectedOnBottom();

    if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom)
    {
        juce::Path path;
        path.addRoundedRectangle(bounds.getX(), bounds.getY(),
            bounds.getWidth(), bounds.getHeight(),
            cornerSize, cornerSize,
            !(flatOnLeft || flatOnTop),
            !(flatOnRight || flatOnTop),
            !(flatOnLeft || flatOnBottom),
            !(flatOnRight || flatOnBottom));

        g.fillPath(path);

        g.setColour(button.findColour(juce::ComboBox::outlineColourId));
        g.strokePath(path, juce::PathStrokeType(0.0f));
    }
    else
    {
        g.fillRoundedRectangle(bounds, cornerSize);

        g.setColour((juce::Colour::fromRGBA(0, 0, 0, 0))); //<--
        g.drawRoundedRectangle(bounds, 0.0f, 0.0f); //<--
    }
}
//forced the outline to be invisible and tweaked the down/highlighted color

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(ThemeChangeButton.getToggleState() ? juce::Colour::fromRGB(235, 237, 240)
                                                 : juce::Colour::fromRGB(56, 68, 77));

    g.setColour(juce::Colour::fromRGB(38, 50, 56));
    g.fillRect(getWidth()-62, 0, 62, 27);

}