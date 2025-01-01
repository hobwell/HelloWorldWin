#pragma once

#include <JuceHeader.h>
#include "DataRace.h"

//==============================================================================
// thread sample:
struct MyThread : juce::Thread
{
    MyThread() : Thread("MyThread") 
    {
        startThread(); // calls run() method
    }

    ~MyThread() override 
    { 
        stopThread(100); 
    }

    void run() override
    {
        
        while (true)
        {
            if (threadShouldExit()) // returns true if thread should exit
                break;

            /*
             do some work here
            */

            if (threadShouldExit()) // returns true if thread should exit
                break;

            /*
             do some work here
            */

            if (threadShouldExit()) // returns true if thread should exit
                break;

            wait(10); // waits for 10 milliseconds
            // or
            wait(-1); // waits indefinitely

            /* from outside:
             MyThread mt;
             mt.notify(); // notifies the thread
            */
        }
    }
};

//==============================================================================
// image processing thread
struct ImageProcessingThread : juce::Thread
{
    using ImagePassingFunc = std::function<void(juce::Image&, ImageProcessingThread& thread)>;
    ImageProcessingThread(int width_, int height_, ImagePassingFunc f);
    ~ImageProcessingThread();
    void run() override;
    //setUpdateRenderer(std::function<void(juce::Image&&)> func);
private:
    int width{ 0 };
    int height{ 0 };
    juce::Random rand;
    ImagePassingFunc updateRenderer;
};

//==============================================================================
// example custom timer
struct LambdaTimer : juce::Timer
{
    LambdaTimer(int ms, std::function<void()> func);
    ~LambdaTimer();
    void timerCallback() override;
private:
    std::function<void()> callback;
};

//==============================================================================
// image buffer
template <int Max> // varying size ImageBuffer?
struct ImageBuffer
{
    void push(juce::Image image)
    {
        const juce::ScopedWriteLock swl(readWriteLock);
        images[(++index) % Max] = image;
    }
    juce::Image read()
    {
        const juce::ScopedReadLock srl(readWriteLock);
        return images[index % Max];
    }
private:
    juce::ReadWriteLock readWriteLock;
    size_t index = 0;
    std::array<juce::Image, Max> images;
};

//==============================================================================
// renderer
// when can this be created? After the component that it is tied to has been added and sized
#include <array>
struct Renderer : public juce::Component, juce::Timer
{
    Renderer();
    ~Renderer() override;
    void paint(juce::Graphics& g) override;
    void timerCallback() override;
private:
    std::unique_ptr<ImageProcessingThread> processingThread;
    std::unique_ptr<LambdaTimer> timer;
    ImageBuffer<5> imageBuffer;
};

//==============================================================================
// renderer2
// when can this be created? After the component that it is tied to has been added and sized
#include <array>
struct Renderer2 : public juce::Component
{
    Renderer2();
    void paint(juce::Graphics& g) override;
private:
    void loop();
    ImageBuffer<5> imageBuffer;
};
//==============================================================================
// dual button:
struct RepeatingThing;
struct DualButton : public juce::Component
{
    DualButton();
    void resized() override;

    void setHandler1(std::function<void()> handler);
    void setHandler2(std::function<void()> handler);

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
// async updater
struct AsyncHiResGui : public juce::Component, juce::AsyncUpdater, juce::HighResolutionTimer
{
    AsyncHiResGui()
    {
        startTimer(1000 / 5);
    }

    ~AsyncHiResGui() override
    {
        stopTimer();
        cancelPendingUpdate();
    }

    
    void hiResTimerCallback() override 
    {
        triggerAsyncUpdate();
    };

    void handleAsyncUpdate() override
    {
        paintColour = (paintColour + 1) % maxColours;
        repaint();
    }

    void paint(juce::Graphics& g) override
    {
        switch (paintColour)
        {
        case 0:
            g.setColour(juce::Colours::orange);
            break;
        case 1:
            g.setColour(juce::Colours::purple);
            break;
        case 2:
            g.setColour(juce::Colours::grey);
            break;
        }
        g.fillAll();
    }

private:
    int paintColour = 0;
    const int maxColours {3};

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
    RepeatingThing repeatingThing;
    DualButton dualButton;
    AsyncHiResGui hiResGui;
    Renderer renderer;
    Renderer2 renderer2;
    // Test Test;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
