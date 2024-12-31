#include "MainComponent.h"

//==============================================================================
// image processing thread
ImageProcessingThread::ImageProcessingThread(int width_, int height_) :
    Thread("ImageProcessingThread"),
    width(width_),
    height(height_)
{
    startThread();
}

ImageProcessingThread::~ImageProcessingThread()
{
    stopThread(500);
}

void ImageProcessingThread::run()
{
    while (true)
    {
        if (threadShouldExit())
            break;

        auto canvas = juce::Image(juce::Image::RGB, width, height, true);

        if (threadShouldExit())
            break;

        bool shouldExit = false;
        for (int x = 0; x < width; ++x)
        {
            if (threadShouldExit())
            {
                shouldExit = true;
                break;
            }

            for (int y = 0; y < height; ++y)
            {
                canvas.setPixelAt(x, y, juce::Colour(rand.nextFloat(), rand.nextFloat(), rand.nextFloat(), 1.f));
            }
        }

        if (threadShouldExit() || shouldExit)
            break;

        if (updateRenderer)
        {
            updateRenderer(std::move(canvas));
        }

        wait(-1);
    }
}

void ImageProcessingThread::setUpdateRenderer(std::function<void(juce::Image&&)> func)
{
    updateRenderer = std::move(func);
}
//==============================================================================
// custom timer
LambdaTimer::LambdaTimer(int ms, std::function<void()> func) :
    callback(std::move(func))
{
    startTimer(ms);
}

LambdaTimer::~LambdaTimer()
{
    stopTimer();
}

void LambdaTimer::timerCallback()
{
    stopTimer();
    if (callback)
    {
        callback();
    }
}
//==============================================================================
// Renderer

Renderer::Renderer()
{
    timer = std::make_unique<LambdaTimer>(10, 
        [this]()
        {
            processingThread = std::make_unique<ImageProcessingThread>(getWidth(), getHeight());

            processingThread->setUpdateRenderer(
                [this](juce::Image&& image)
                {
                    int renderIndex = firstImage ? 0 : 1;
                    firstImage = !firstImage;
                    imageToRender[renderIndex] = std::move(image);

                    triggerAsyncUpdate();

                    timer = std::make_unique<LambdaTimer>(1000,
                        [this]()
                        {
                            processingThread->notify();
                        }
                    );
                }
            );
        }
    );
}

Renderer::~Renderer()
{
    processingThread.reset();
    timer.reset();
}

void Renderer::paint(juce::Graphics& g)
{
    g.drawImage (
        (firstImage ? imageToRender[0] : imageToRender[1]),
        getLocalBounds().toFloat()
    );
}

void Renderer::handleAsyncUpdate()
{
    repaint();
}

//==============================================================================
//
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

    addAndMakeVisible (hiResGui);

    //addAndMakeVisible(renderer);

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
    hiResGui.setBounds(repeatingThing.getBounds().withX(repeatingThing.getRight() + 5));
    //renderer.setBounds(hiResGui.getBounds().withX(hiResGui.getRight() + 5));
}
