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
    _restHandler()
{
    _newFavButton.addListener(this);
}

Main_C::~Main_C()
{
    _newFavButton.removeListener(this);
    for (unsigned int i = 0; i < _favSlots.size(); i++) {
        _favSlots[i]->getButton(0).removeListener(this);
        _favSlots[i]->getButton(1).removeListener(this);
        delete(_favSlots[i]);
    }
}

// Runs when slider value is changed
void Main_C::sliderValueChanged(juce::Slider* slider) {
    if (slider == &_rSlider) {
        _restHandler.setR(_rSlider.getValue());
    }
    else if (slider == &_gSlider) {
        _restHandler.setG(_gSlider.getValue());
    }
    else {
        _restHandler.setB(_bSlider.getValue());
    }
    repaint();
}

// Runs when mouse is lifted from a slider
void Main_C::sliderDragEnded(juce::Slider* slider) {
    _restHandler.grabColorPushUpdate();
}

void Main_C::buttonClicked(juce::Button* button) {
    RGB rgb;
    RGB hRGB = _restHandler.getRGB().colorCorrect();
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
            _restHandler.takeColorPushUpdate(rgb);
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
    RGB correctedRGB = _restHandler.getRGB().colorCorrect();
    juce::uint8 cR = correctedRGB._r;
    juce::uint8 cG = correctedRGB._g;
    juce::uint8 cB = correctedRGB._b;

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
    _newFavButton.setBounds(0, 0, newFavButtonWidth, getHeight() / 7);


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
void Main_C::setSliderValues(RGB rgb) {
    _rSlider.setValue(rgb._r);
    _gSlider.setValue(rgb._g);
    _bSlider.setValue(rgb._b);
}
//* Getters / Setters