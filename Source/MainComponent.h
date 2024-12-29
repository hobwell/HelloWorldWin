#pragma once

#include <JuceHeader.h>


struct DualButton : public juce::Component
{
    DualButton();
    void resized() override;
private:
    juce::TextButton button1{ "Button 1" };
    juce::TextButton button2{ "Button 2" };
};

//==============================================================================
// component for array:
struct Widget : public juce::Component
{
    Widget (int i) : index (i) { }

    void paint(juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::orange);
        g.setColour (juce::Colours::purple);
        g.drawRect (getLocalBounds().reduced (2));
        g.drawFittedText (juce::String (index), 
                          getLocalBounds(), 
                          juce::Justification::centred, 
                          1);
    }

    int index;
};

//==============================================================================
// repeating thing
struct RepeatingThing : public juce::Component, juce::Timer
{
    RepeatingThing() { startTimerHz(2); };
    ~RepeatingThing() { stopTimer(); };

    void paint(juce::Graphics& g) override
    {
        g.fillAll(drawOrange ? juce::Colours::orange : juce::Colours::purple);
    }

    void timerCallback() override
    {
        drawOrange = !drawOrange;
        repaint();
    }

private:
    bool drawOrange = true;
};


//==============================================================================
// owned array component template:
struct OwnedArrayComponent : public juce::Component, juce::Button::Listener
{
    OwnedArrayComponent();

    ~OwnedArrayComponent() override;

    void addTextButton();

    void buttonClicked(juce::Button* button) override;

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::purple);
    }

    void resized() override;

    juce::OwnedArray<juce::TextButton> buttons;
};


//==============================================================================
// component template:

struct ExampleComponent : public juce::Component
{
    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::purple);
    }

    void resized() override { }

    void mouseEnter (const juce::MouseEvent& event) override
    {
        DBG("mouse enter");
    }

    void mouseExit(const juce::MouseEvent& event) override
    {
        DBG("mouse exit");
    }

    void mouseMove(const juce::MouseEvent& event) override
    {
        DBG("mouse moved " << ++counter);
    }

    int counter = 0;
};



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void mouseEnter (const juce::MouseEvent& event) override
    {
        //DBG("MainComponent mouse enter");
    }

    void mouseExit(const juce::MouseEvent& event) override
    {
        //DBG("MainComponent mouse exit");
    }

    void mouseMove(const juce::MouseEvent& event) override
    {
        //DBG("MainComponent mouse move " << ++counter);
    }

    void mouseDown(const juce::MouseEvent& event) override
    {
        DBG("mouse down");
    }

    void paint (juce::Graphics&) override;
    void resized() override;
    int counter = 0;
private:
    //==============================================================================
    // Your private member variables go here...
    ExampleComponent exComp;
    OwnedArrayComponent ownedArrayComp;
    DualButton dualButton;
    RepeatingThing repeatingThing;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
