/*
  ==============================================================================

    CoreLEDControlPanel.cpp
    Created: 18 Dec 2022 4:47:03pm
    Author:  Tiparium

  ==============================================================================
*/

#include "CoreLEDControlPanel.h"

CoreLEDControlPanel::CoreLEDControlPanel(juce::Component* parent) :
    _nodeMCUThreadWrapper(&_rgb),
    _pHuePHueHandler(params::_httpTarget, params::_apiTarget, params::_apiGetTarget, params::_apiPutTarget),
    _favsHandler("../../resources/favSlots.json")
{
    _parent = parent;

    // Individual LED controls
    _toggleAllLEDControlButton.addListener(this);
    for (int i = 0; i < _pHuePHueHandler.getNumLights(); i++)
    {
        _pHueLEDPickers.push_back(new juce::TextButton);
        _pHueLEDPickers[i]->addListener(this);
        _listeningLights.push_back(true);
        DBG(i);
    }

    // Handle Persistence
    nlohmann::json jsonFromFile = _favsHandler.readJSONFromFile();
    for (auto i = 0; i < jsonFromFile.size(); i++)
    {
        TIP_RGB newRGB = TIP_RGB(jsonFromFile[i]["r"], jsonFromFile[i]["g"], jsonFromFile[i]["b"]);
        TIP_RGB hRGB = newRGB.colorCorrect();
        FavoritesSlot* newSlot = new FavoritesSlot(hRGB);
        _favSlots.push_back(newSlot);
        newSlot->getButton(0).addListener(this);
        newSlot->getButton(1).addListener(this);
    }
    _newFavButton.addListener(this);
    _nodeMCUThreadWrapper.startThread();
}

CoreLEDControlPanel::~CoreLEDControlPanel()
{
    // Handle Persistence
    nlohmann::json jsonToFile = nlohmann::json::array();

    // Destroy LED picker
    _toggleAllLEDControlButton.removeListener(this);
    for (int i = 0; i < _pHueLEDPickers.size(); i++)
    {
        _pHueLEDPickers[i]->removeListener(this);
        delete _pHueLEDPickers[i];
    }

    // Destroy FavSlots
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

    // Stop NodeMCU thread
    _nodeMCUThreadWrapper.stopThread(500);
}

// Runs when slider value is changed
void CoreLEDControlPanel::sliderValueChanged(juce::Slider* slider) {

    _rgb.r = _rSlider.getValue();
    _rgb.g = _gSlider.getValue();
    _rgb.b = _bSlider.getValue();

    _parent->postCommandMessage(tip_rgb_Values_Updated);
    repaint();
}

// Runs when mouse is lifted from a slider
void CoreLEDControlPanel::sliderDragEnded(juce::Slider* slider) {
    _pHuePHueHandler.pushUpdateToMultipleLights(_rgb, _listeningLights);
}

void CoreLEDControlPanel::buttonClicked(juce::Button* button)
{
    bool check;
    check = checkFavoritesButtons(button);
    if (check) { return; }

    check = checkLEDControlButtons(button);
    if (check) { return; }

    DBG("Something went wrong: An Unhandeled button has been pushed.");
}

bool CoreLEDControlPanel::checkFavoritesButtons(juce::Button* button)
{
    TIP_RGB rgb;
    TIP_RGB hRGB = _rgb.colorCorrect();
    // Create new Favorite Slot
    if (button == &_newFavButton) {
        FavoritesSlot* newSlot = new FavoritesSlot(hRGB);
        _favSlots.push_back(newSlot);
        newSlot->getButton(0).addListener(this);
        newSlot->getButton(1).addListener(this);
        return true;
    }
    for (unsigned int i = 0; i < _favSlots.size(); i++) {
        // Call Favorite Slot
        if (button == &_favSlots[i]->getButton(0)) {
            rgb = _favSlots[i]->getRGB();

            // Update UI
            setSliderValues(rgb);

            // Push colors to respective platforms
            _pHuePHueHandler.pushUpdateToMultipleLights(rgb, _listeningLights);

            return true;
        }
        // Delete Favorite Slot
        else if (button == &_favSlots[i]->getButton(1)) {
            _favSlots[i]->getButton(0).removeListener(this);
            _favSlots[i]->getButton(1).removeListener(this);
            removeChildComponent(_favSlots[i]);
            FavoritesSlot* temp = _favSlots[i];
            _favSlots.erase(_favSlots.begin() + i);
            delete temp;
            return true;
        }
    }
    return false;
}

bool CoreLEDControlPanel::checkLEDControlButtons(juce::Button* button)
{
    // Button pressed was global toggle
    if (button == &_toggleAllLEDControlButton)
    {
        bool allActive = checkLEDControlButtonState();
        if (allActive)
        {
            for (int i = 0; i < _listeningLights.size(); i++)
            {
                _listeningLights[i] = false;
            }
        }
        else {
            for (int i = 0; i < _listeningLights.size(); i++)
            {
                _listeningLights[i] = true;
            }
        }
        return true;
    }
    // Button pressed was an LED toggle
    for (int i = 0; i < _pHuePHueHandler.getNumLights(); i++)
    {
        if (button == _pHueLEDPickers[i])
        {
            _listeningLights[i] = !_listeningLights[i];
            return true;
        }
    }
    return false;
}

bool CoreLEDControlPanel::checkLEDControlButtonState()
{
    for (int i = 0; i < _listeningLights.size(); i++)
    {
        if (!_listeningLights[i]) { return false; }
    }
    return true;
}

void CoreLEDControlPanel::paint(juce::Graphics& g) {
    g.drawRect(getLocalBounds(), 1);
    // Generate a corrected color & excract rgb components
    TIP_RGB correctedRGB = _rgb.colorCorrect();
    juce::uint8 cR = correctedRGB.r;
    juce::uint8 cG = correctedRGB.g;
    juce::uint8 cB = correctedRGB.b;

    paintFavorites(g);
    paintSliders(g);
    paintLEDControlButtons(g);

    g.fillAll(juce::Colour(cR, cG, cB));
}

void CoreLEDControlPanel::paintFavorites(juce::Graphics& g)
{
    int numSlots = _favSlots.size();

    float keyButtonWidth = getWidth() / 10;
    float relativeXPos = 0;
    float relativeYPos = keyButtonWidth;
    float relativeWidth = (getWidth() - keyButtonWidth) / numSlots;
    float relativeHeight = getHeight() / 7;
    // Create Fav button
    addAndMakeVisible(_newFavButton);
    if (_favSlots.size() > 0) {
        _newFavButton.setBounds(0, 0, keyButtonWidth, getHeight() / 7);
        _newFavButton.setButtonText("+");
    }
    else {
        _newFavButton.setBounds(0, 0, getWidth(), getHeight() / 7);
        _newFavButton.setButtonText("Click Here to Create a Favorite");
    }

    // Draw and size FavoritesSlots
    for (int i = 0; i < _favSlots.size(); i++) {
        addAndMakeVisible(_favSlots[i]);
        _favSlots[i]->setBounds(relativeYPos, relativeXPos, relativeWidth, relativeHeight);
        relativeYPos += relativeWidth;
    }
}
void CoreLEDControlPanel::paintSliders(juce::Graphics& g)
{
    float relativeXPos;
    float relativeYPos;
    float relativeWidth;
    float relativeHeight;

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
    relativeYPos = 100;
    _rSlider.setBounds(relativeYPos, relativeHeight, getWidth() - 2 * relativeYPos, 20);
    _gSlider.setBounds(relativeYPos, relativeHeight + 20, getWidth() - 2 * relativeYPos, 20);
    _bSlider.setBounds(relativeYPos, relativeHeight + 40, getWidth() - 2 * relativeYPos, 20);
    //* RGBSliders
} // PaintSlider
void CoreLEDControlPanel::paintLEDControlButtons(juce::Graphics& g)
{
    int numSlots = _pHuePHueHandler.getNumLights();
    float keyButtonWidth = getWidth() / 10;

    float relativeWidth = (getWidth() - keyButtonWidth) / numSlots;
    float relativeHeight = getHeight() / 14;
    float relativeXPos = 0;
    float relativeYPos = getHeight() - relativeHeight;

    // Draw All Button
    addAndMakeVisible(_toggleAllLEDControlButton);
    _toggleAllLEDControlButton.setBounds(0, relativeYPos, keyButtonWidth, relativeHeight);
    relativeXPos += keyButtonWidth;

    // Draw remaining buttons
    for (int i = 0; i < _pHueLEDPickers.size(); i++)
    {
        addAndMakeVisible(_pHueLEDPickers[i]);
        _pHueLEDPickers[i]->setBounds(relativeXPos, relativeYPos, relativeWidth, relativeHeight);
        relativeXPos += relativeWidth;

        _pHueLEDPickers[i]->setButtonText(_pHuePHueHandler.getLightNameByID(i + 1));
        _pHueLEDPickers[i]->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::black);

        if (!_listeningLights[i])
        {
            _pHueLEDPickers[i]->setColour(
                juce::TextButton::ColourIds::buttonColourId,
                juce::Colours::darkgrey);
        }
        else
        {
            TIP_RGB rgb = _rgb.colorCorrect();
            _pHueLEDPickers[i]->setColour(
                juce::TextButton::ColourIds::buttonColourId,
                juce::Colour(rgb.r, rgb.g, rgb.b));
        }
    }
}

void CoreLEDControlPanel::resized()
{
}

// Getters / Setters
void CoreLEDControlPanel::setSliderValues(TIP_RGB rgb) {
    _rSlider.setValue(rgb.r);
    _gSlider.setValue(rgb.g);
    _bSlider.setValue(rgb.b);
}

TIP_RGB CoreLEDControlPanel::getRGB() { return _rgb; }
//* Getters / Setters