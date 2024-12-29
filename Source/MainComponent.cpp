#include "MainComponent.h"

DualButton::DualButton()
{
    addAndMakeVisible(button1);
    addAndMakeVisible(button2);
}

void DualButton::resized()
{
    auto bounds = getLocalBounds();
    auto buttonWidth = bounds.getWidth() / 2;
    button1.setBounds(bounds.removeFromLeft(30));
    button2.setBounds(bounds);
}

void DualButton::setHandler1(std::function<void()> handler)
{
    button1.onClick = handler;
}

void DualButton::setHandler2(std::function<void()> handler)
{
    button2.onClick = handler;
}

//==============================================================================

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

    addAndMakeVisible(dualButton);

    dualButton.setHandler1(
        [this]()
        {
            repeatingThing.startTimerHz(2);
        });

    dualButton.setHandler2(
        [this]()
        {
            repeatingThing.startTimerHz(4);
        });

    addAndMakeVisible (repeatingThing);

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
    dualButton.setBounds(exComp.getRight(), exComp.getY(), exComp.getWidth(), exComp.getHeight());
    dualButton.setBounds(exComp.getBounds().withX(exComp.getRight() + 5));
    repeatingThing.setBounds(dualButton.getBounds().withX(dualButton.getRight() + 5));
}
