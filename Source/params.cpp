/*
  ==============================================================================

    paramsTemplate.cpp
    Created: 5 Oct 2022 8:43:02pm
    Author:  Tiparium
    Rename this file to params.cpp & fill in indicated lines below.
    If you don't know where to get the indicated data, follow this tutorial -> https://developers.meethue.com/develop/get-started-2/
    Yes I know there is an automated way to do this, it's on my TODO list.
  ==============================================================================
*/

#include "../resources/params.h"

struct params;

juce::String params::_httpTarget = "http://10.0.0.139";
juce::String params::_apiTarget = "/api/5Gpo7VxAqssVu2E8lvpcZTRCVzqfmngwBbXqvJpO";
juce::String params::_apiGetTarget = "/lights";
juce::String params::_apiPutTarget = "/lights/x/state";