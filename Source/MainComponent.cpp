#include "MainComponent.h"

OwnedArrayComponent::OwnedArrayComponent()
{
    for (int i = 0; i < 10; ++i)
    {
        addTextButton();
    }
}

OwnedArrayComponent::~OwnedArrayComponent()
{
    for (auto* button : buttons)
    {
        button->removeListener (this);
    }
    buttons.clear();
}

void OwnedArrayComponent::addTextButton()
{
    auto* component = buttons.add (new juce::TextButton (std::to_string (buttons.size())));
    addAndMakeVisible (component);
    component->addListener (this);
}

void OwnedArrayComponent::buttonClicked(juce::Button* clickedButton)
{
    if (clickedButton == buttons.getFirst())
    {
        DBG("First button clicked");
    }
    else if (clickedButton == buttons.getLast())
    {
        DBG("Last button clicked");
    }
    else
    {
        DBG("Button " << clickedButton->getName() << " clicked");
    }
}

void OwnedArrayComponent::resized()
{
    auto width = getWidth() / static_cast<float> (buttons.size());
    auto height = getHeight();
    int x = 0;

    for (auto* widget : buttons)
    {
        widget->setBounds(x, 0, width, height);
        x += width;
    }
}

//==============================================================================
MainComponent::MainComponent()
{

    addAndMakeVisible(exComp);
    //exComp.addMouseListener(this, false); // forwards mouse events from ExampleComponent to MainComponent

    addAndMakeVisible(ownedArrayComp);
    ownedArrayComp.addMouseListener(this, true); // forwards mouse events from OwnedArrayComponent to MainComponent

    setSize (600, 400);
}

MainComponent::~MainComponent()
{
    exComp.removeMouseListener(this);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::FontOptions (32.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    exComp.setBounds(50, 50, 100, 100);
    ownedArrayComp.setBounds(exComp.getX(), exComp.getBottom(), getWidth() - exComp.getX(), getHeight() - exComp.getBottom());
}
