/*
  ==============================================================================

    FileChooserJSONHandler.cpp
    Created: 4 Feb 2023 8:43:28am
    Author:  Tiparium

  ==============================================================================
*/

#include "FileChooserJSONHandler.h"
FileChooserJSONHandler::FileChooserJSONHandler()
{

}

FileChooserJSONHandler::~FileChooserJSONHandler()
{

}

void FileChooserJSONHandler::writeDataToJSONFile(std::vector<PatternPickerSlot*> pattern, int multiplier)
{
    // Grab multiplier data
    nlohmann::json outputJSON;
    outputJSON["multiplier"] = multiplier;

    // Convert pattern to string in JSON format
    nlohmann::json patternAsJSON;
    for (int i = 0; i < pattern.size(); i++)
    {
        TIP_RGB tempRGB = pattern[i]->getTrueRGB();
        patternAsJSON.push_back(tempRGB.toJSON());
    }
    outputJSON["pattern"] = patternAsJSON;
    juce::String patternAsJSONString = outputJSON.dump();

    // Create a file with JSON contents
    juce::File tempFile = juce::File::createTempFile("json");
    if (!tempFile.createDirectory().wasOk()){
        DBG("Error saving file!");
        throw new std::exception("Error Saving File!");
    }

    // Reset & trigger file chooser
    _fileChooser.reset(new juce::FileChooser("Choose a file to save...",
        juce::File::getCurrentWorkingDirectory().getChildFile(tempFile.getFileName()),
        "*json", true));

    // Write file contents to permanent file
    _fileChooser->launchAsync(juce::FileBrowserComponent::saveMode |
        juce::FileBrowserComponent::canSelectFiles,
        [tempFile, patternAsJSONString](const juce::FileChooser& chooser)
    {
        auto result = chooser.getURLResult();
        auto name = result.isEmpty() ? juce::String()
            : (result.isLocalFile() ? result.getLocalFile().getFullPathName() : result.toString(true));

        juce::File file(name);
        patternAsJSONString != "null" ? 
            file.replaceWithText(patternAsJSONString) : file.replaceWithText("[]");
        file.create();
    });
}

nlohmann::json FileChooserJSONHandler::readDataFromJSONFile()
{
    _fileChooser.reset(new juce::FileChooser("Choose a file to open...", juce::File::getCurrentWorkingDirectory(),
        "*", true));
    nlohmann::json fileContentsAsJSON;

    _fileChooser->launchAsync(juce::FileBrowserComponent::openMode
        | juce::FileBrowserComponent::canSelectFiles,
        [fileContentsAsJSON](const juce::FileChooser& chooser)
    {
        juce::String chosen;
        auto results = chooser.getURLResults();

        for (auto result : results)
            chosen << (result.isLocalFile() ? result.getLocalFile().getFullPathName()
                : result.toString(false));

        juce::AlertWindow::showAsync(juce::MessageBoxOptions()
            .withIconType(juce::MessageBoxIconType::InfoIcon)
            .withTitle("File Chooser...")
            .withMessage("You picked: " + chosen)
            .withButton("OK"),
            nullptr);

        std::ifstream ifs(chosen.toStdString());
        nlohmann::json temp = nlohmann::json::parse(ifs);
        DBG(temp.dump());
    });
    nlohmann::json tempReturn;
    tempReturn["multiplier"] = 1;
    TIP_RGB tempRGB(255, 0, 0);
    nlohmann::json tempPattern;
    tempPattern.push_back(tempRGB.toJSON());
    tempReturn["pattern"] = tempPattern;

    return tempReturn;
    //return fileContentsAsJSON;
}

void FileChooserJSONHandler::holder()
{
}