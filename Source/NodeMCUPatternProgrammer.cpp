/*
  ==============================================================================

    NodeMCUPatternProgrammer.cpp
    Created: 26 Dec 2022 5:23:42pm
    Author:  Tiparium

  ==============================================================================
*/

#include "NodeMCUPatternProgrammer.h"

NodeMCUPatternProgrammer::NodeMCUPatternProgrammer(juce::Component* parent, WebServerHandler* webServerHandler, PHueHandler* pHueHandler, PersistenceJSONHandler* persistenceJSONHandler, TIP_RGB* uiRGB, TIP_RGB* ledRGB, bool* overrideMode) :
    _webServerHandler_Ref(webServerHandler),
    _pHuePHueHandler_Ref(pHueHandler),
    _persistenceJSONHandler_Ref(persistenceJSONHandler),
    _parent(parent),
    _uiRGB_Ref(uiRGB),
    _ledRGB_Ref(ledRGB),
    _patternOverrideMode_Ref(overrideMode)
{
    // Handle persistence

    // Create button listeners
    _newNode_B.addListener(this);
    _uploadPattern_B.addListener(this);
    _togglePatternOverride_B.addListener(this);
    _multiplierUp_B.addListener(this);
    _multiplierDown_B.addListener(this);
    _scrollSpeedUp_B.addListener(this);
    _scrollSpeedDown_B.addListener(this);
    _savePattern_B.addListener(this);
    _loadPattern_B.addListener(this);
}

NodeMCUPatternProgrammer::~NodeMCUPatternProgrammer()
{
    // TODO: Handle persistance

    // Remove listeners
    _newNode_B.removeListener(this);
    _uploadPattern_B.removeListener(this);
    _togglePatternOverride_B.removeListener(this);
    _multiplierUp_B.removeListener(this);
    _multiplierDown_B.removeListener(this);
    _scrollSpeedUp_B.removeListener(this);
    _scrollSpeedDown_B.removeListener(this);
    _savePattern_B.removeListener(this);
    _loadPattern_B.removeListener(this);

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
    paintPatternOptions(g);
    paintSaveLoadUpload(g);

    g.fillAll(juce::Colour(_localRGB.r, _localRGB.g, _localRGB.b));
}

void NodeMCUPatternProgrammer::paintPatternGenerator(juce::Graphics& g)
{
    int numSlots = _patternPickers.size();

    float keyButtonWidth = getWidth() / 10;
    float relativeXPos = 0;
    float relativeYPos = keyButtonWidth;
    float relativeWidth = (getWidth() - keyButtonWidth) / numSlots;
    float relativeHeight = getHeight() / 7;
    // Create Fav button
    addAndMakeVisible(_newNode_B);
    if (_patternPickers.size() > 0) {
        _newNode_B.setBounds(0, 0, keyButtonWidth, getHeight() / 7);
        _newNode_B.setButtonText("+");
    }
    else {
        _newNode_B.setBounds(0, 0, getWidth(), getHeight() / 7);
        _newNode_B.setButtonText("Click Here to Create a Pattern Node");
    }

    // Draw and size FavoritesSlots
    for (int i = 0; i < _patternPickers.size(); i++) {
        addAndMakeVisible(_patternPickers[i]);
        _patternPickers[i]->setBounds(relativeYPos, relativeXPos, relativeWidth, relativeHeight);
        relativeYPos += relativeWidth;
    }
}

void NodeMCUPatternProgrammer::paintSliders(juce::Graphics& g)
{
    float xPos;
    float yPos;
    float width;
    float height;

    // Red Slider
    addAndMakeVisible(_rSlider);
    _rSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _rSlider.setRange(0, 255, 1);
    _rSlider.setTextValueSuffix(" R");

    addAndMakeVisible(_rLabel);
    _rLabel.setText("R", juce::NotificationType::dontSendNotification);
    _rLabel.attachToComponent(&_rSlider, true);
    _rSlider.addListener(this);
    //* Red Slider

    // Green Slider
    addAndMakeVisible(_gSlider);
    _gSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _gSlider.setRange(0, 255, 1);
    _gSlider.setTextValueSuffix(" G");

    addAndMakeVisible(_gLabel);
    _gLabel.setText("G", juce::NotificationType::dontSendNotification);
    _gLabel.attachToComponent(&_gSlider, true);
    _gSlider.addListener(this);
    //* Green Slider

    // Blue Slider
    addAndMakeVisible(_bSlider);
    _bSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _bSlider.setRange(0, 255, 1);
    _bSlider.setTextValueSuffix(" B");

    addAndMakeVisible(_bLabel);
    _bLabel.setText("B", juce::NotificationType::dontSendNotification);
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
    width = getWidth() / 1.5;
    height = 20;
    yPos = getHeight() / 2;
    xPos = getWidth() / 2 - (width / 2);
    _rSlider.setBounds(xPos, yPos + 00, width, height);
    _gSlider.setBounds(xPos, yPos + 20, width, height);
    _bSlider.setBounds(xPos, yPos + 40, width, height);
    //* RGBSliders
} // PaintSlider

void NodeMCUPatternProgrammer::paintPatternOptions(juce::Graphics& g)
{
    float leftEdge = getWidth() / 6;

    // Values are initialized to left hand buttons first
    const float width = (getWidth() / 1.5) / 6;
    const float height = getHeight() / 14;
    const float yPosUpper = getHeight() / 2 + 80;
    const float yPosLower = getHeight() / 2 + 80 + height;
    float xPos = leftEdge;

    _multiplierUp_B.setBounds(xPos, yPosUpper, width, height);
    _multiplierUp_B.setButtonText("* ^");
    _multiplierUp_B.setTooltip("Increases pattern multiplier");
    _multiplierDown_B.setBounds(xPos, yPosLower, width, height);
    _multiplierDown_B.setButtonText("* v");
    _multiplierDown_B.setTooltip("Decreases pattern multiplier");
    addAndMakeVisible(_multiplierUp_B);
    addAndMakeVisible(_multiplierDown_B);

    xPos += width;

    _patternMultiplier_L.setBounds(xPos, yPosUpper, width, height * 2);
    _patternMultiplier_L.setText(std::to_string(_patternMultiplier), juce::dontSendNotification);
    _patternMultiplier_L.setFont(40.0f);
    _patternMultiplier_L.setColour(juce::Label::textColourId, juce::Colours::black);
    _patternMultiplier_L.setColour(juce::Label::outlineColourId, juce::Colours::grey);
    _patternMultiplier_L.setTooltip("Current Pattern Multiplier");
    _patternMultiplier_L.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(_patternMultiplier_L);

    xPos += width;

    _togglePatternOverride_B.setBounds(xPos, yPosUpper, width * 2, height * 2);
    _togglePatternOverride_B.setButtonText("Toggle Pattern\nOverride State");
    addAndMakeVisible(_togglePatternOverride_B);

    xPos += (width * 2);

    juce::String mode = *_patternOverrideMode_Ref ? "Pattern Override" : "Single Color";
    _currentMode_L.setBounds(xPos, yPosUpper, width * 2, height * 2);
    _currentMode_L.setText("Current Mode:\n" + mode, juce::dontSendNotification);
    _currentMode_L.setFont(16.0f);
    _currentMode_L.setJustificationType(juce::Justification::centred);
    _currentMode_L.setColour(juce::Label::textColourId, juce::Colours::black);
    _currentMode_L.setColour(juce::Label::outlineColourId, juce::Colours::grey);
    addAndMakeVisible(_currentMode_L);
}

void NodeMCUPatternProgrammer::paintSaveLoadUpload(juce::Graphics& g)
{
    float width = getWidth() / 4;
    float height = getHeight() / 10;
    float xPos = getWidth() / 2 - (width / 2);
    float yPos = getHeight() - height;

    _uploadPattern_B.setButtonText("Upload\nPattern");
    _uploadPattern_B.setBounds(xPos, yPos, width, height);
    addAndMakeVisible(_uploadPattern_B);

    _savePattern_B.setButtonText("Save\nPattern");
    _savePattern_B.setBounds(0, yPos - height * 0.5, width * 1.505, height * 1.5);
    addAndMakeVisible(_savePattern_B);

    xPos += width;

    _loadPattern_B.setButtonText("Load\nPattern");
    _loadPattern_B.setBounds(xPos, yPos - height * 0.5, width * 1.505, height * 1.5);
    addAndMakeVisible(_loadPattern_B);
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
    // Currently does nothing
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

    check = checkToggleModeButton(button);
    if (check) { return; }

    DBG("\n*****************************\nSomething went wrong, button:\n-----------\n" + button->getButtonText() + "\n-----------\nHas not been handled");
}

bool NodeMCUPatternProgrammer::checkPatternButtons(juce::Button* button)
{
    if (button == &_newNode_B)
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
    if (button == &_multiplierUp_B)
    {
        if (_patternMultiplier < 10)
        {
            _patternMultiplier += 1;
        }
        return true;
    }
    if (button == &_multiplierDown_B)
    {
        if (_patternMultiplier > 1)
        {
            _patternMultiplier -= 1;
        }
        return true;
    }
    return false;
}

bool NodeMCUPatternProgrammer::checkToggleModeButton(juce::Button* button)
{
    if (button == &_togglePatternOverride_B)
    {
        *_patternOverrideMode_Ref = !*_patternOverrideMode_Ref;
        return true;
    }
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
// *G/S