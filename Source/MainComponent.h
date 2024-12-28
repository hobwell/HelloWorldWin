#pragma once

#include <JuceHeader.h>


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
// owned array component template:
struct OwnedArrayComponent : public juce::Component
{
    OwnedArrayComponent() 
    {
        for (int i = 0; i < 10; ++i)
        {
            addWidget();
        }
    }

    void addWidget()
    {
        addAndMakeVisible (widgets.add (new Widget (widgets.size())));
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::purple);
    }

    void resized() override
    {
        auto width = getWidth() / static_cast<float> (widgets.size());
        auto height = getHeight();
        int x = 0;

        for (auto* widget : widgets)
        {
            widget->setBounds(x, 0, width, height);
            x += width;
        }
    }

    juce::OwnedArray<Widget> widgets;
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
    OwnedArrayComponent ownedArrayComp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
