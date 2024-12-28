#pragma once

#include <JuceHeader.h>


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
        DBG("MainComponent mouse enter");
    }

    void mouseExit(const juce::MouseEvent& event) override
    {
        DBG("MainComponent mouse exit");
    }

    void mouseMove(const juce::MouseEvent& event) override
    {
        DBG("MainComponent mouse move " << ++counter);
    }

    void paint (juce::Graphics&) override;
    void resized() override;
    int counter = 0;
private:
    //==============================================================================
    // Your private member variables go here...
    ExampleComponent exComp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
