/*
  ==============================================================================

    NodeMCUPatternProgrammer.cpp
    Created: 26 Dec 2022 5:23:42pm
    Author:  Tiparium

  ==============================================================================
*/

#include "NodeMCUPatternProgrammer.h"

NodeMCUPatternProgrammer::NodeMCUPatternProgrammer(juce::Component* parent, WebServerHandler* webServerHandler, PHueHandler* pHueHandler, PersistenceJSONHandler* persistenceJSONHandler, TIP_RGB* uiRGB, TIP_RGB* ledRGB) :
    _webServerHandler_Ref(webServerHandler),
    _pHuePHueHandler_Ref(pHueHandler),
    _persistenceJSONHandler_Ref(persistenceJSONHandler),
    _parent(parent),
    _uiRGB_Ref(uiRGB),
    _ledRGB_Ref(ledRGB)
{
    // Handle persistence

    // Create button listeners
    _newNodeButton.addListener(this);

}

NodeMCUPatternProgrammer::~NodeMCUPatternProgrammer()
{
    // Handle persistance

    // Remove listeners
    _newNodeButton.removeListener(this);
    // Delete any dynamic variables
    // Free & Delete pattern picker nodes
    for (int i = 0; i < _patternPickers.size(); i++)
    {
        _patternPickers[i]->getButton(0).removeListener(this);
        _patternPickers[i]->getButton(1).removeListener(this);
        _patternPickers[i]->getButton(2).removeListener(this);
        _patternPickers[i]->getButton(-1).removeListener(this);

        delete _patternPickers[i];
    }
}

void NodeMCUPatternProgrammer::handleCommandMessage(int commandId)
{
    switch (commandId)
    {
    case pattern_picker_mode_updated:
        _localRGB = _currentSlot->getRGB();
        repaint();
        break;

    default:
        throw std::invalid_argument("Invalid Command Message");
        break;
    }
}

void NodeMCUPatternProgrammer::paint(juce::Graphics& g)
{
    paintSliders(g);
    paintPatternGenerator(g);

    g.fillAll(juce::Colour(_localRGB.r, _localRGB.g, _localRGB.b));
}

void NodeMCUPatternProgrammer::paintSliders(juce::Graphics& g)
{
    float relativeXPos;
    float relativeWidth;
    float relativeYPos;

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
    relativeYPos = getHeight() / 2;
    relativeXPos = 100;
    _rSlider.setBounds(relativeXPos, relativeYPos, getWidth() - (2 * relativeXPos), 20);
    _gSlider.setBounds(relativeXPos, relativeYPos + 20, getWidth() - (2 * relativeXPos), 20);
    _bSlider.setBounds(relativeXPos, relativeYPos + 40, getWidth() - (2 * relativeXPos), 20);
    //* RGBSliders
} // PaintSlider

void NodeMCUPatternProgrammer::paintPatternGenerator(juce::Graphics& g)
{
    int numSlots = _patternPickers.size();

    float keyButtonWidth = getWidth() / 10;
    float relativeXPos = 0;
    float relativeYPos = keyButtonWidth;
    float relativeWidth = (getWidth() - keyButtonWidth) / numSlots;
    float relativeHeight = getHeight() / 7;
    // Create Fav button
    addAndMakeVisible(_newNodeButton);
    if (_patternPickers.size() > 0) {
        _newNodeButton.setBounds(0, 0, keyButtonWidth, getHeight() / 7);
        _newNodeButton.setButtonText("+");
    }
    else {
        _newNodeButton.setBounds(0, 0, getWidth(), getHeight() / 7);
        _newNodeButton.setButtonText("Click Here to Create a Pattern Node");
    }

    // Draw and size FavoritesSlots
    for (int i = 0; i < _patternPickers.size(); i++) {
        addAndMakeVisible(_patternPickers[i]);
        _patternPickers[i]->setBounds(relativeYPos, relativeXPos, relativeWidth, relativeHeight);
        relativeYPos += relativeWidth;
    }
}

void NodeMCUPatternProgrammer::sliderValueChanged(juce::Slider* slider)
{
    if (_patternPickers.size() > 0)
    {
        if (_currentSlot->getMode())
        {
            TIP_RGB tempRGB = buildRGBFromSliders();
            _localRGB = buildRGBFromSliders().colorCorrect();
            _currentSlot->setRGB(tempRGB);
        }
        else
        {
            _localRGB = buildRGBFromSliders();
            _currentSlot->setRGB(_localRGB);
        }
    }
    else
    {
        _localRGB = buildRGBFromSliders().colorCorrect();
    }
    repaint();
}

void NodeMCUPatternProgrammer::sliderDragEnded(juce::Slider* slider)
{
}

TIP_RGB NodeMCUPatternProgrammer::buildRGBFromSliders()
{
    juce::uint8 r = _rSlider.getValue();
    juce::uint8 g = _gSlider.getValue();
    juce::uint8 b = _bSlider.getValue();

    return TIP_RGB(r, g, b);
}

void NodeMCUPatternProgrammer::buttonClicked(juce::Button* button)
{
    bool check;
    check = checkPatternButtons(button);
    if (check) { return; }

    check = checkMultiplierButtons(button);
    if (check) { return; }

    check = checkSaveLoadUploadButtons(button);
    if (check) { return; }

    check = checkScrollingButtons(button);
    if (check) { return; }

    DBG("Something went wrong: An Unhandeled button has been pushed.");
}

bool NodeMCUPatternProgrammer::checkPatternButtons(juce::Button* button)
{
    // TODO
    if (button == &_newNodeButton)
    {
        PatternPickerSlot* temp = new PatternPickerSlot(dynamic_cast<juce::Component*>(this), buildRGBFromSliders());
        temp->getButton(0).addListener(this);
        temp->getButton(1).addListener(this);
        temp->getButton(2).addListener(this);
        temp->getButton(-1).addListener(this);
        _patternPickers.push_back(temp);
        setActiveSlot(_patternPickers.size() - 1);
        return true;
    }
    for (int i = 0; i < _patternPickers.size(); i++)
    {
        // Call core slot (select)
        if (button == &_patternPickers[i]->getButton(0) || button == &_patternPickers[i]->getButton(-1))
        {
            setActiveSlot(i);
            setSliderValues(_patternPickers[i]->getTrueRGB());
            return true;
        }
        // Call toggle mode slot
        if (button == &_patternPickers[i]->getButton(2))
        {
            _patternPickers[i]->toggleMode();
            return true;
        }
        // Call delete slot
        if (button == &_patternPickers[i]->getButton(1))
        {
            _patternPickers[i]->getButton(0).removeListener(this);
            _patternPickers[i]->getButton(1).removeListener(this);
            _patternPickers[i]->getButton(2).removeListener(this);
            removeChildComponent(_patternPickers[i]);
            PatternPickerSlot* temp = _patternPickers[i];
            _patternPickers.erase(_patternPickers.begin() + i);
            delete temp;
            return true;
        }
    }
    return false;
}

bool NodeMCUPatternProgrammer::checkMultiplierButtons(juce::Button* button)
{
    // TODO
    return false;
}

bool NodeMCUPatternProgrammer::checkScrollingButtons(juce::Button* button)
{
    // TODO
    return false;
}

bool NodeMCUPatternProgrammer::checkSaveLoadUploadButtons(juce::Button* button)
{
    // TODO
    return false;
}

void NodeMCUPatternProgrammer::setActiveSlot(int slotIndex)
{
    for (int i = 0; i < _patternPickers.size(); i++)
    {
        if (i == slotIndex)
        {
            _patternPickers[i]->setActive();
            _currentSlot = _patternPickers[i];
        }
        else
        {
            _patternPickers[i]->setInactive();
        }
    }
}

// Getters / Setters
void NodeMCUPatternProgrammer::setSliderValues(TIP_RGB rgb) {
    _rSlider.setValue(rgb.r);
    _gSlider.setValue(rgb.g);
    _bSlider.setValue(rgb.b);
}

// G/S
// *G/S