/*
  ==============================================================================

    Main_C.cpp
    Created: 9 Oct 2022 12:05:31am
    Author:  Tiparium

    Contains & Displays subcomponents both for data processing & UI display.

  ==============================================================================
*/

#include "Main_C.h"

Main_C::Main_C() :
    _dumbRGBHandler(),
    _pHueRestHandler(params::_httpTarget, params::_apiTarget, params::_apiGetTarget, params::_apiPutTarget),
    _favsHandler("../../resources/favSlots.json")
{
    nlohmann::json jsonFromFile = _favsHandler.readJSONFromFile();
    for (auto i = 0; i < jsonFromFile.size(); i++) {
        TIP_RGB newRGB = TIP_RGB(jsonFromFile[i]["r"], jsonFromFile[i]["g"], jsonFromFile[i]["b"]);
        TIP_RGB hRGB = newRGB.colorCorrect();
        FavoritesSlot* newSlot = new FavoritesSlot(hRGB);
        _favSlots.push_back(newSlot);
        newSlot->getButton(0).addListener(this);
        newSlot->getButton(1).addListener(this);
    }
    _newFavButton.addListener(this);
}

Main_C::~Main_C()
{
    nlohmann::json jsonToFile = nlohmann::json::array();
    _newFavButton.removeListener(this);
    for (auto i = 0; i < _favSlots.size(); i++) {
        nlohmann::json favSlotJSON;
        TIP_RGB favSlotRGB = _favSlots[i]->getRGB();
        favSlotJSON["r"] = favSlotRGB.r;
        favSlotJSON["g"] = favSlotRGB.g;
        favSlotJSON["b"] = favSlotRGB.b;
        jsonToFile.push_back(favSlotJSON);
    }
    for (unsigned int i = 0; i < _favSlots.size(); i++) {
        _favSlots[i]->getButton(0).removeListener(this);
        _favSlots[i]->getButton(1).removeListener(this);

        delete _favSlots[i];
    }
    DBG("JSON TO PUSH TO FILE: " + jsonToFile.dump());
    _favsHandler.saveJSONToFile(jsonToFile);

}

// Runs when slider value is changed
void Main_C::sliderValueChanged(juce::Slider* slider) {
    if (slider == &_rSlider) {
        _pHueRestHandler.setR(_rSlider.getValue());
    }
    else if (slider == &_gSlider) {
        _pHueRestHandler.setG(_gSlider.getValue());
    }
    else {
        _pHueRestHandler.setB(_bSlider.getValue());
    }
    repaint();
}

// Runs when mouse is lifted from a slider
void Main_C::sliderDragEnded(juce::Slider* slider) {
    _pHueRestHandler.grabColorPushUpdate();
}

void Main_C::buttonClicked(juce::Button* button) {
    TIP_RGB rgb;
    TIP_RGB hRGB = _pHueRestHandler.getRGB().colorCorrect();
    // Create new Favorite Slot
    if (button == &_newFavButton) {
        FavoritesSlot* newSlot = new FavoritesSlot(hRGB);
        _favSlots.push_back(newSlot);
        newSlot->getButton(0).addListener(this);
        newSlot->getButton(1).addListener(this);
        return;
    }
    for (unsigned int i = 0; i < _favSlots.size(); i++) {\
        // Call Favorite Slot
        if (button == &_favSlots[i]->getButton(0)) {
            rgb = _favSlots[i]->getRGB();
            _pHueRestHandler.takeColorPushUpdate(rgb);
            setSliderValues(rgb);
            return;
        }
        // Delete Favorite Slot
        else if (button == &_favSlots[i]->getButton(1)) {
            _favSlots[i]->getButton(0).removeListener(this);
            _favSlots[i]->getButton(1).removeListener(this);
            removeChildComponent(_favSlots[i]);
            FavoritesSlot* temp = _favSlots[i];
            _favSlots.erase(_favSlots.begin() + i);
            delete temp;
            return;
        }
    }
    DBG("Something went wrong: This loop should never fully terminate.");
}

void Main_C::paint(juce::Graphics& g) {
    g.drawRect(getLocalBounds(), 1);
    float relativePos;
    float relativeWidth;
    float relativeHeight;
    // Generate a corrected color & excract rgb components
    TIP_RGB correctedRGB = _pHueRestHandler.getRGB().colorCorrect();
    juce::uint8 cR = correctedRGB.r;
    juce::uint8 cG = correctedRGB.g;
    juce::uint8 cB = correctedRGB.b;

    // Red Slider
    addAndMakeVisible(_rSlider);
    _rSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _rSlider.setRange(0, 255, 1);
    _rSlider.setTextValueSuffix(" R");

    addAndMakeVisible(_rLabel);
    _rLabel.setText("RED", juce::NotificationType::dontSendNotification);
    _rLabel.attachToComponent(&_rSlider, true);
    _rSlider.addListener(this);
    //* Red Slider

    // Green Slider
    addAndMakeVisible(_gSlider);
    _gSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _gSlider.setRange(0, 255, 1);
    _gSlider.setTextValueSuffix(" G");

    addAndMakeVisible(_gLabel);
    _gLabel.setText("GREEN", juce::NotificationType::dontSendNotification);
    _gLabel.attachToComponent(&_gSlider, true);
    _gSlider.addListener(this);
    //* Green Slider

    // Blue Slider
    addAndMakeVisible(_bSlider);
    _bSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _bSlider.setRange(0, 255, 1);
    _bSlider.setTextValueSuffix(" B");

    addAndMakeVisible(_bLabel);
    _bLabel.setText("BLUE", juce::NotificationType::dontSendNotification);
    _bLabel.attachToComponent(&_bSlider, true);
    _bSlider.addListener(this);
    //* Blue Slider

    // Set labels to black
    g.setColour(juce::Colours::black);
    _rLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    _rSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
    _gLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    _gSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);
    _bLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    _bSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::black);

    g.setFont(30.0f);

    // Size RGBSliders
    relativeHeight = getHeight() / 2;
    relativePos = 100;
    _rSlider.setBounds(relativePos, relativeHeight, getWidth() - 2 * relativePos, 20);
    _gSlider.setBounds(relativePos, relativeHeight + 20, getWidth() - 2 * relativePos, 20);
    _bSlider.setBounds(relativePos, relativeHeight + 40, getWidth() - 2 * relativePos, 20);
    //* RGBSliders

    // Create Fav button
    addAndMakeVisible(_newFavButton);
    float newFavButtonWidth = getWidth() / 10;
    float favBarHeight = getHeight() / 7;
    if (_favSlots.size() > 0) {
        _newFavButton.setBounds(0, 0, newFavButtonWidth, getHeight() / 7);
        _newFavButton.setButtonText("+");
    }
    else {
        _newFavButton.setBounds(0, 0, getWidth(), getHeight() / 7);
        _newFavButton.setButtonText("Click Here to Create a Favorite");
    }


    // Draw and size FavoritesSlots
    int numSlots = _favSlots.size();
    relativePos = newFavButtonWidth;
    relativeWidth = (getWidth() - newFavButtonWidth) / numSlots;
    relativeHeight = getHeight() / 7;
    for (int i = 0; i < _favSlots.size(); i++) {
        addAndMakeVisible(_favSlots[i]);
        _favSlots[i]->setBounds(relativePos, 0, relativeWidth, favBarHeight);
        relativePos += relativeWidth;
    }

    g.fillAll(juce::Colour(cR, cG, cB));
}

void Main_C::resized() {
}

// Getters / Setters
void Main_C::setSliderValues(TIP_RGB rgb) {
    _rSlider.setValue(rgb.r);
    _gSlider.setValue(rgb.g);
    _bSlider.setValue(rgb.b);
}
//* Getters / Setters