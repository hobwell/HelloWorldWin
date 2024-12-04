/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"
struct Foot
{
    int steps = 0;

    void stepForward();
    int stepSize();
};

void Foot::stepForward()
{
    steps += 1;
}

int Foot::stepSize()
{
    return 1;
}

struct Person
{
    int age;// = 0;
    int heightInInches = 0;
    int distanceTraveled = 0;
    float hairLength{ 0.0f };
    float GPA{ 0.0f };
    unsigned int SATScore{ 0 };
    int distanceTravelled = 0;

    int run(int howFast, bool startWithLeftFoot);

    juce::String name;

    Foot leftFoot, rightFoot;

    Person(juce::String personName) : name (personName)
    {
        DBG("Person ctor: " + name);
    }

    Person(int age)
    {
        this->age = age;
    }

    ~Person()
    {
        DBG("Person dtor: " + name);
    }

    void moveAndSetAge(int speed, int newAge);
};

int Person::run(int howFast, bool startWithLeftFoot)
{
    if (startWithLeftFoot == true)
    {
        leftFoot.stepForward();
        rightFoot.stepForward();
    }
    else
    {
        rightFoot.stepForward();
        leftFoot.stepForward();
    }

    distanceTraveled += leftFoot.stepSize() + rightFoot.stepSize();
    return distanceTraveled;
}

void Person::moveAndSetAge(int speed, int newAge)
{
    this->run(speed, true);
    this->age = newAge;
}

void doStuff()
{
    //Person p{ "person" };
    Person p{ 6 };
    p.moveAndSetAge(5, 42);
}

void PersonFunction()
{
    Person p{ "The Dude" };
}

void functionC()
{
    int i = 0;
    int j = 3;
    while (i < 3)
    {
        int a = 0;
        a += i;
        i += 1;
        j += i;
    }
    {
        int i = 1;
        while (i < 3)
        {
            int i = 0;
            int a = 0;
            a += i;
            i += 1;
        }
    }
}

void whileTest()
{
    bool b = true;
    while (b)
    {
        b = false;
        DBG("b is " << (b ? "true" : "false"));
    }
}

struct Family
{
    Family() { DBG("family ctor"); }
    ~Family() { DBG("family dtor"); }

    Person mom{ "mom" };
    Person dad{ "dad" };
    Person childOne{ "Viktor" };
    Person childTwo{ "Henri" };
};

void familyFunction()
{
    Family family;
}

//==============================================================================
class HelloWorldApplication : public juce::JUCEApplication
{
public:
    //==============================================================================
    HelloWorldApplication() {}

    const juce::String getApplicationName() override { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..
//        functionC();
        familyFunction();
        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name)
            : DocumentWindow (name,
                juce::Desktop::getInstance().getDefaultLookAndFeel()
                .findColour (juce::ResizableWindow::backgroundColourId),
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
#else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
#endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (HelloWorldApplication)
