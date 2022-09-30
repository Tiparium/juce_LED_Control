/*
  ==============================================================================

    Vars.h
    Created: 29 Sep 2022 9:24:24pm
    Author:  Tiparium

    Contains Private Data that ought not to be shared :)
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DataVault {
public:

    static juce::String getAPITarget() {
        return _apiTarget;
    }

private:
    static const juce::String _apiTarget =
        "/api/5Gpo7VxAqssVu2E8lvpcZTRCVzqfmngwBbXqvJpO";

    const juce::String _httpTarget = "http://10.0.0.139"
};