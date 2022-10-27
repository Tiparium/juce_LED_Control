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
    FavoritesSlot* slot = new FavoritesSlot();
    FavoritesSlot* slotTwo = new FavoritesSlot();
    FavoritesSlot* slotThree = new FavoritesSlot();
    FavoritesSlot* slotFour = new FavoritesSlot();

    _favSlots.push_back(slot);
    _favSlots.push_back(slotTwo);
    _favSlots.push_back(slotThree);
    _favSlots.push_back(slotFour);

    slot->getButton(0).addListener(this);
    slotTwo->getButton(0).addListener(this);
    slotThree->getButton(0).addListener(this);
    slotFour->getButton(0).addListener(this);

    slot->getButton(1).addListener(this);
    slotTwo->getButton(1).addListener(this);
    slotThree->getButton(1).addListener(this);
    slotFour->getButton(1).addListener(this);
}

Main_C::~Main_C()
{
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
    for (unsigned int i = 0; i < _favSlots.size(); i++) {
        if (button == &_favSlots[i]->getButton(0)) {
            if (!_favSlots[i]->state) {
                // Assign button values & Color
                RGB hRGB = _restHandler.getRGB().colorCorrect();
                _favSlots[i]->setRGB(_restHandler.getRGB());
                _favSlots[i]->getButton(0).setColour(
                    juce::TextButton::ColourIds::buttonColourId, juce::Colour(hRGB._r, hRGB._g, hRGB._b));
                _favSlots[i]->state = true;
                return;
            }
            else {
                rgb = _favSlots[i]->getRGB();
                _restHandler.takeColorPushUpdate(rgb);
                setSliderValues(rgb);
                return;
            }
        }
    }
    DBG("Something went wrong: This loop should never fully terminate.");
}

void Main_C::paint(juce::Graphics& g) {
    g.drawRect(getLocalBounds(), 1);

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
    //* RGBSliders

    // Sizing Occurs below this line. Use these three variables to resize all items as needed.
    float relativePos;
    float relativeWidth;
    float relativeHeight;

    // Draw and size FavoritesSlots
    int numSlots = _favSlots.size();
    relativePos = 0.0f;
    relativeWidth = getWidth() / numSlots;
    relativeHeight = getHeight() / 7;
    for (int i = 0; i < _favSlots.size(); i++) {
        addAndMakeVisible(_favSlots[i]);
        _favSlots[i]->setBounds(relativePos, 0, relativeWidth, relativeHeight);
        relativePos += relativeWidth;
    }

    // Size RGBSliders
    relativeHeight = getHeight() / 2;
    relativePos = 100;
    _rSlider.setBounds(relativePos, relativeHeight, getWidth() - 2 * relativePos, 20);
    _gSlider.setBounds(relativePos, relativeHeight + 20, getWidth() - 2 * relativePos, 20);
    _bSlider.setBounds(relativePos, relativeHeight + 40, getWidth() - 2 * relativePos, 20);

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