/*
  ==============================================================================

     An application which allows control over both Philips Hue LEDS,
     and "dumb" LEDS attached to an Arduino or NodeMCU

     External Reliances::
        For making REST requests:   https://github.com/adamski/RestRequest
        For parsing JSON:           https://github.com/nlohmann/json
        Because I'm Stupid:         https://github.com/gregjesl/simpleson

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"

//==============================================================================
class MainWindowTutorialApplication  : public juce::JUCEApplication
{
public:
    class MainWindow: public:: juce::DocumentWindow
    {
    public:
        MainWindow(juce:: String name) : DocumentWindow(name, juce::Colours::goldenrod, DocumentWindow::allButtons)
        {
            centreWithSize(500, 500);
            // setBounds (50, 50, 100, 100);
            // setBoundsRelative ((0.25f / 2), (0.25f / 2), 0.75f, 0.75f);

            MainComponent *main = new MainComponent();
            setContentOwned(main, false);
            setUsingNativeTitleBar (true);
#if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
#else
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
#endif
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow);
    };

    //==============================================================================
    MainWindowTutorialApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        
        mainWindow.reset(new MainWindow(getApplicationName() + " " + getApplicationVersion()));
    }

    void shutdown() override
    {
        mainWindow = nullptr;
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
private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (MainWindowTutorialApplication)
