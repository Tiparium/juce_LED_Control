/*
  ==============================================================================
     
     BTODOs:
        - Get NODEMCU talking to this program (Need some kind of webserver library)
        - Color Triangle
        - Favorites Bar &/ Persistency
        - Seperate RGB Sliders from pHue API functionality (See RGBSlider.h)

     An application which allows control over both Philips Hue LEDS,
     and "dumb" LEDS attached to an Arduino or NodeMCU

     External Reliances::
        For making REST requests:           https://github.com/adamski/RestRequest
        For parsing JSON:                   https://github.com/nlohmann/json
        Because I'm Stupid: (RMVD)          https://github.com/gregjesl/simpleson
        For processing RGB to HXY: (DRPCD)  https://github.com/enwi/hueplusplus

     Legend::All tags should be in .h files
        _C:     A component file.
        _B:     A backend infrastucture file.
        _D:     A data file.
        BTODO:  Long term feature goal
        TODO:   A thing to do
        DRPCD:  A depricated featuer I'm keeping for now
        RMVD:   A removed feature stored externally
  ==============================================================================
*/

#include <JuceHeader.h>
#include "Main_C.h"

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

            // Host
            Main_C *main_C = new Main_C();
            setContentOwned(main_C, false);

            setUsingNativeTitleBar (true);
#if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
#else
            setResizable(true, true);
            setResizeLimits(600, 600, 8000, 6000);
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
