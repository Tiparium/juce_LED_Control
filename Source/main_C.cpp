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
    _nodeMCUServerHandler(),
    _pHueRestHandler(params::_httpTarget, params::_apiTarget, params::_apiGetTarget, params::_apiPutTarget),
    _favsHandler("../../resources/favSlots.json")
{
    // Individual LED controls
    _pHueLEDCount = _pHueRestHandler.getRefNumLights();
    _pHueLEDPickers.push_back(&_allPHueLEDSButton);
    _allPHueLEDSButton.addListener(this);
    for (int i = 1; i <= *_pHueLEDCount; i++)
    {
        _pHueLEDPickers.push_back(new juce::TextButton);
        _pHueLEDPickers[i]->addListener(this);
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
    _nodeMCUServerHandler.pushToServer(TIP_RGB(0, 0, 0).colorCorrect());
}

Main_C::~Main_C()
{
    // Handle Persistence
    nlohmann::json jsonToFile = nlohmann::json::array();

    // Destroy LED picker
    _allPHueLEDSButton.removeListener(this);
    for (int i = 1; i < _pHueLEDPickers.size(); i++)
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
}

// Runs when slider value is changed
void Main_C::sliderValueChanged(juce::Slider* slider) {

    _rgb.r = _rSlider.getValue();
    _rgb.g = _gSlider.getValue();
    _rgb.b = _bSlider.getValue();

    repaint();
}

// Runs when mouse is lifted from a slider
void Main_C::sliderDragEnded(juce::Slider* slider) {
    _pHueRestHandler.pushUpdateToMultipleLights(_rgb);
    _nodeMCUServerHandler.pushToServer(_rgb.colorCorrect());
}

void Main_C::buttonClicked(juce::Button* button)
{
    bool check;
    check = checkFavoritesButtons(button);
    if (check) { return; }

    check = checkLEDControlButtons(button);
    if (check) { return; }

    DBG("Something went wrong: An Unhandeled button has been pushed.");
}

bool Main_C::checkFavoritesButtons(juce::Button* button)
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
            _pHueRestHandler.pushUpdateToMultipleLights(rgb);
            _nodeMCUServerHandler.pushToServer(rgb.colorCorrect());

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

bool Main_C::checkLEDControlButtons(juce::Button* button)
{
    if (button == &_allPHueLEDSButton) { _currentLight = 0; return true; }
    for (int i = 1; i <= *_pHueLEDCount; i++)
    {
        if (button == _pHueLEDPickers[i])
        {
            _currentLight = i;
            return true;
        }
    }
    return false;
}

void Main_C::paint(juce::Graphics& g) {
    g.drawRect(getLocalBounds(), 1);
    // Generate a corrected color & excract rgb components
    TIP_RGB correctedRGB = _rgb.colorCorrect();
    juce::uint8 cR = correctedRGB.r;
    juce::uint8 cG = correctedRGB.g;
    juce::uint8 cB = correctedRGB.b;

    paintFavorites(g);
    paintSliders(g);
    paintLEDSelector(g);

    g.fillAll(juce::Colour(cR, cG, cB));
}

void Main_C::paintFavorites(juce::Graphics& g)
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
void Main_C::paintSliders(juce::Graphics& g)
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
void Main_C::paintLEDSelector(juce::Graphics& g)
{
    int numSlots = *_pHueLEDCount;
    float keyButtonWidth = getWidth() / 10;

    float relativeWidth = (getWidth() - keyButtonWidth) / numSlots;
    float relativeHeight = getHeight() / 14;
    float relativeXPos = 0;
    float relativeYPos = getHeight() - relativeHeight;

    // Draw All Button
    addAndMakeVisible(_pHueLEDPickers[0]);
    _pHueLEDPickers[0]->setBounds(0, relativeYPos, keyButtonWidth, relativeHeight);
    relativeXPos += keyButtonWidth;

    // Draw remaining buttons
    for (int i = 1; i < _pHueLEDPickers.size(); i++)
    {
        addAndMakeVisible(_pHueLEDPickers[i]);
        _pHueLEDPickers[i]->setBounds(relativeXPos, relativeYPos, relativeWidth, relativeHeight);
        relativeXPos += relativeWidth;
    }
}

void Main_C::resized()
{
}

// Getters / Setters
void Main_C::setSliderValues(TIP_RGB rgb) {
    _rSlider.setValue(rgb.r);
    _gSlider.setValue(rgb.g);
    _bSlider.setValue(rgb.b);
}
//* Getters / Setters