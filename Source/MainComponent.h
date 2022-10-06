/*
  ==============================================================================

    MainComponent.h
    Created: 26 Sep 2022 11:09:01am
    Author:  Tiparium

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <fstream>
#include <iostream>  
#include <string>  
#include "rest_request.h"
#include "nlohmann/json.hpp"
#include "../resources/params.h" // Replace with custom params file

//==============================================================================

class MainComponent : public juce::Component, public juce::Slider::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void size();
    void sliderValueChanged(juce::Slider* slider) override;
    void sliderDragEnded(juce::Slider* slider) override;
    void getPhilipsData();
    void grabRGBPushUpdate();
    void pushUpdate(juce::var xyColor, juce::String target);
    void updateRootJSON();

    nlohmann::json pingAndReceive(juce::String target);

    int getNumLights();

    // Debug Functions:
    void resetColor(); // Reset lights to color they were at time of app being opened

    // I definitely didn't steal these structs. Nope.
    struct XY
    {
        float x;
        float y;

        bool operator==(const XY& other) const { return x == other.x && y == other.y; }
        bool operator!=(const XY& other) const { return !(*this == other); }
    };
    struct XYBrightness
    {
        XY xy;
        float brightness;

        bool operator==(const XYBrightness& other) const { return xy == other.xy && brightness == other.brightness; }
        bool operator!=(const XYBrightness& other) const { return !(*this == other); }
    };
    struct RGB
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;

        bool operator==(const RGB& other) const { return r == other.r && g == other.g && b == other.b; }
        bool operator!=(const RGB& other) const { return !(*this == other); }

        MainComponent::XYBrightness MainComponent::RGB::toXY() const
        {
            if (r == 0 && g == 0 && b == 0)
            {
                // Return white with minimum brightness
                return XYBrightness{ XY {0.32272673f, 0.32902291f}, 0.f };
            }
            const float red = r / 255.f;
            const float green = g / 255.f;
            const float blue = b / 255.f;

            const float redCorrected = (red > 0.04045f) ? pow((red + 0.055f) / (1.0f + 0.055f), 2.4f) : (red / 12.92f);
            const float greenCorrected = (green > 0.04045f) ? pow((green + 0.055f) / (1.0f + 0.055f), 2.4f) : (green / 12.92f);
            const float blueCorrected = (blue > 0.04045f) ? pow((blue + 0.055f) / (1.0f + 0.055f), 2.4f) : (blue / 12.92f);

            const float X = redCorrected * 0.664511f + greenCorrected * 0.154324f + blueCorrected * 0.162028f;
            const float Y = redCorrected * 0.283881f + greenCorrected * 0.668433f + blueCorrected * 0.047685f;
            const float Z = redCorrected * 0.000088f + greenCorrected * 0.072310f + blueCorrected * 0.986039f;

            const float x = X / (X + Y + Z);
            const float y = Y / (X + Y + Z);
            // Set brightness to the brightest channel value (rather than average of them),
            // so full red/green/blue can be displayed
            return XYBrightness{ XY {x, y}, std::max({red, green, blue}) };
        }
        MainComponent::RGB MainComponent::RGB::fromXY(const XYBrightness& xy)
        {
            if (xy.brightness < 1e-4)
            {
                return RGB{ 0, 0, 0 };
            }
            const float z = 1.f - xy.xy.x - xy.xy.y;
            // use a fixed luminosity and rescale the resulting rgb values using brightness
            // randomly sampled conversions shown a minimum difference between original values
            // and values after rgb -> xy -> rgb conversion for Y = 0.3
            // (r-r')^2, (g-g')^2, (b-b')^2:
            // 4.48214,  4.72039,  3.12141
            // Max. Difference:
            // 9,        9,        8
            const float Y = 0.3f;
            const float X = (Y / xy.xy.y) * xy.xy.x;
            const float Z = (Y / xy.xy.y) * z;

            const float r = X * 1.656492f - Y * 0.354851f - Z * 0.255038f;
            const float g = -X * 0.707196f + Y * 1.655397f + Z * 0.036152f;
            const float b = X * 0.051713f - Y * 0.121364f + Z * 1.011530f;

            // Reverse gamma correction
            const float gammaR = r <= 0.0031308f ? 12.92f * r : (1.0f + 0.055f) * pow(r, (1.0f / 2.4f)) - 0.055f;
            const float gammaG = g <= 0.0031308f ? 12.92f * g : (1.0f + 0.055f) * pow(g, (1.0f / 2.4f)) - 0.055f;
            const float gammaB = b <= 0.0031308f ? 12.92f * b : (1.0f + 0.055f) * pow(b, (1.0f / 2.4f)) - 0.055f;

            // Scale color values so that the brightness matches
            const float maxColor = std::max({ gammaR, gammaG, gammaB });
            if (maxColor < 1e-4)
            {
                // Low color values, out of gamut?
                return RGB{ 0, 0, 0 };
            }
            const float rScaled = gammaR / maxColor * xy.brightness * 255.f;
            const float gScaled = gammaG / maxColor * xy.brightness * 255.f;
            const float bScaled = gammaB / maxColor * xy.brightness * 255.f;

            return RGB{ static_cast<uint8_t>(std::round(std::max(0.f, rScaled))),
                static_cast<uint8_t>(std::round(std::max(0.f, gScaled))),
                static_cast<uint8_t>(std::round(std::max(0.f, bScaled))) };
        }
    };

private:
    juce::String _currentSizeAsString;

    // Combine these for a full callable RESTful URL
    juce::String _httpTarget;
    juce::String _apiTarget;
    juce::String _apiGetTarget;
    juce::String _apiPutTarget;
    int _apiPutSplit = 8; // Index of the placeholder character (X) in _apiPutTarget

    juce::Label _rLabel;
    juce::Label _gLabel;
    juce::Label _bLabel;

    juce::Slider _rSlider;
    juce::Slider _gSlider;
    juce::Slider _bSlider;

    juce::uint8 _rVal = 0;
    juce::uint8 _gVal = 0;
    juce::uint8 _bVal = 0;

    int _numLights = -1;

    // holds response of the get all lights RESTful call
    nlohmann::json _rootJSON;

    // For making API calls
    adamski::RestRequest _req;

    // For resetting to original color settings
    float _OGxVal;
    float _OGyVal;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
