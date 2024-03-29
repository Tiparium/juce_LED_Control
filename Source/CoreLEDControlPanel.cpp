/*
  ==============================================================================

    CoreLEDControlPanel.cpp
    Created: 18 Dec 2022 4:47:03pm
    Author:  Tiparium

  ==============================================================================
*/

#include "CoreLEDControlPanel.h"

CoreLEDControlPanel::CoreLEDControlPanel(juce::Component* parent, WebServerHandler* webServerHandler, PHueHandler* pHueHandler, PersistenceJSONHandler* persistenceJSONHandler, TIP_RGB* uiRGB, TIP_RGB* ledRGB):
    _webServerHandler_Ref(webServerHandler),
    _pHuePHueHandler_Ref(pHueHandler),
    _persistenceJSONHandler_Ref(persistenceJSONHandler),
    _parent(parent),
    _uiRGB_Ref(uiRGB),
    _ledRGB_Ref(ledRGB)
{
    nlohmann::json jsonFromFile = _persistenceJSONHandler_Ref->readJSONFromFile();
    
    // Individual LED controls

    _toggleAllLEDControl_B.onClick = [this] { toggleAllLEDControl_B_Clicked(); };
    _newFav_B.onClick = [this] { newFav_B_Clicked(); };

    for (int i = 0; i < _pHuePHueHandler_Ref->getNumLights(); i++)
    {
        juce::TextButton* temp = new juce::TextButton;
        _pHueLEDPickers.push_back(temp);
        _pHueLEDPickers[i]->onClick = [this, temp] { checkLEDControlButtons(temp); };
        try
        {
            _listeningLights.push_back(jsonFromFile["phue_led_bools"][i]);
        }
        catch (nlohmann::json_abi_v3_11_2::detail::type_error)
        {
            _listeningLights.push_back(true);
        }
    }

    // Handle Fav Slots Persistence
    for (auto i = 0; i < jsonFromFile["favorites"].size(); i++)
    {
        TIP_RGB newRGB = TIP_RGB(
            jsonFromFile["favorites"][i]["r"],
            jsonFromFile["favorites"][i]["g"],
            jsonFromFile["favorites"][i]["b"]);
        initAndAddNewFavSlot(newRGB);
    }

    // Restore Previous State - TODO: Figure out why this doesn't work
    juce::uint8 r = jsonFromFile["base_color_state"]["r"];
    juce::uint8 g = jsonFromFile["base_color_state"]["g"];
    juce::uint8 b = jsonFromFile["base_color_state"]["b"];
    TIP_RGB state = TIP_RGB(r, g, b);
    *_uiRGB_Ref = state.colorCorrect();
    *_ledRGB_Ref = state;
    setSliderValues(state);
}

CoreLEDControlPanel::~CoreLEDControlPanel()
{
    // Save current state
    nlohmann::json currentStateToJSONFile;
    currentStateToJSONFile["r"] = _ledRGB_Ref->r;
    currentStateToJSONFile["g"] = _ledRGB_Ref->g;
    currentStateToJSONFile["b"] = _ledRGB_Ref->b;

    // Save LED Picker state & destroy LED pickers
    nlohmann::json ledPickerStatesToJSONFile;
    for (int i = 0; i < _pHueLEDPickers.size(); i++)
    {
        ledPickerStatesToJSONFile.push_back(_listeningLights[i]);
        delete _pHueLEDPickers[i];
    }

    // Save Faveslot data & destroy slots
    nlohmann::json favoritesToJSONFile;
    for (int i = 0; i < _favSlots.size(); i++) {
        nlohmann::json favSlotJSON;
        TIP_RGB favSlotRGB = _favSlots[i]->getCCRGB();
        favSlotJSON["r"] = favSlotRGB.r;
        favSlotJSON["g"] = favSlotRGB.g;
        favSlotJSON["b"] = favSlotRGB.b;
        favoritesToJSONFile.push_back(favSlotJSON);
    }
    for (int i = 0; i < _favSlots.size(); i++) {
        delete _favSlots[i];
    }
    _persistenceJSONHandler_Ref->addJSONToLocalInstance("base_color_state", currentStateToJSONFile);
    _persistenceJSONHandler_Ref->addJSONToLocalInstance("phue_led_bools", ledPickerStatesToJSONFile);
    _persistenceJSONHandler_Ref->addJSONToLocalInstance("favorites", favoritesToJSONFile);
}

// Runs when slider value is changed
void CoreLEDControlPanel::sliderValueChanged(juce::Slider* slider) {

    _uiRGB_Ref->r = _rSlider.getValue();
    _uiRGB_Ref->g = _gSlider.getValue();
    _uiRGB_Ref->b = _bSlider.getValue();

    _parent->postCommandMessage(tip_rgb_values_updated);
    repaint();
}

// Runs when mouse is lifted from a slider
void CoreLEDControlPanel::sliderDragEnded(juce::Slider* slider) {
    *_ledRGB_Ref = *_uiRGB_Ref;
    _pHuePHueHandler_Ref->pushUpdateToMultipleLights(*_ledRGB_Ref, _listeningLights);
}

void CoreLEDControlPanel::initAndAddNewFavSlot(TIP_RGB rgb)
{
    TIP_RGB hRGB = (rgb).colorCorrect();
    FavoritesSlot* newSlot = new FavoritesSlot(hRGB);
    _favSlots.push_back(newSlot);
    newSlot->getButton(FavoritesSlot::_fav_B_ID).onClick
        = [this, newSlot] { favSlot_B_Clicked(newSlot); };
    newSlot->getButton(FavoritesSlot::_del_B_ID).onClick
        = [this, newSlot] { favSlotDelete_B_Clicked(newSlot); };
}

void CoreLEDControlPanel::toggleAllLEDControl_B_Clicked()
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
}

void CoreLEDControlPanel::newFav_B_Clicked()
{
    TIP_RGB rgb = *_ledRGB_Ref;
    initAndAddNewFavSlot(rgb);
}

void CoreLEDControlPanel::favSlot_B_Clicked(FavoritesSlot* slot)
{
    for (int i = 0; i < _favSlots.size(); i++)
    {
        if (slot == _favSlots[i])
        {
            TIP_RGB rgb = _favSlots[i]->getCCRGB();

            // Update UI
            setSliderValues(rgb);

            // Push colors to respective platforms
            _pHuePHueHandler_Ref->pushUpdateToMultipleLights(rgb, _listeningLights);
            *_ledRGB_Ref = *_uiRGB_Ref;
            break;
        }
    }
}

void CoreLEDControlPanel::favSlotDelete_B_Clicked(FavoritesSlot* slot)
{
    for (int i = 0; i < _favSlots.size(); i++)
    {
        if (slot == _favSlots[i])
        {
            FavoritesSlot* temp = _favSlots[i];
            _favSlots.erase(_favSlots.begin() + i);
            delete temp;
            break;
        }
    }
}

bool CoreLEDControlPanel::checkLEDControlButtons(juce::Button* button)
{
    // Button pressed was global toggle
    if (button == &_toggleAllLEDControl_B)
    {

        return true;
    }
    // Button pressed was an LED toggle
    for (int i = 0; i < _pHuePHueHandler_Ref->getNumLights(); i++)
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
    TIP_RGB correctedRGB = (*_uiRGB_Ref).colorCorrect();
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
    addAndMakeVisible(_newFav_B);
    if (_favSlots.size() > 0) {
        _newFav_B.setBounds(0, 0, keyButtonWidth, getHeight() / 7);
        _newFav_B.setButtonText("+");
    }
    else {
        _newFav_B.setBounds(0, 0, getWidth(), getHeight() / 7);
        _newFav_B.setButtonText("Click Here to Create a Favorite");
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
    relativeWidth = getWidth() / 1.5;
    relativeHeight = 20;
    relativeYPos = getHeight() / 2;
    relativeXPos = getWidth() / 2 - (relativeWidth / 2);
    _rSlider.setBounds(relativeXPos, relativeYPos + 00, relativeWidth, relativeHeight);
    _gSlider.setBounds(relativeXPos, relativeYPos + 20, relativeWidth, relativeHeight);
    _bSlider.setBounds(relativeXPos, relativeYPos + 40, relativeWidth, relativeHeight);
    //* RGBSliders
} // PaintSlider

void CoreLEDControlPanel::paintLEDControlButtons(juce::Graphics& g)
{
    int numSlots = _pHuePHueHandler_Ref->getNumLights();
    float keyButtonWidth = getWidth() / 10;

    float relativeWidth = (getWidth() - keyButtonWidth) / numSlots;
    float relativeHeight = getHeight() / 14;
    float relativeXPos = 0;
    float relativeYPos = getHeight() - relativeHeight;

    // Draw All Button
    addAndMakeVisible(_toggleAllLEDControl_B);
    _toggleAllLEDControl_B.setBounds(0, relativeYPos, keyButtonWidth, relativeHeight);
    relativeXPos += keyButtonWidth;

    // Draw remaining buttons
    for (int i = 0; i < _pHueLEDPickers.size(); i++)
    {
        addAndMakeVisible(_pHueLEDPickers[i]);
        _pHueLEDPickers[i]->setBounds(relativeXPos, relativeYPos, relativeWidth, relativeHeight);
        relativeXPos += relativeWidth;

        _pHueLEDPickers[i]->setButtonText(_pHuePHueHandler_Ref->getLightNameByID(i + 1));
        _pHueLEDPickers[i]->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::black);

        if (!_listeningLights[i])
        {
            _pHueLEDPickers[i]->setColour(
                juce::TextButton::ColourIds::buttonColourId,
                juce::Colours::darkgrey);
        }
        else
        {
            TIP_RGB rgb = (*_uiRGB_Ref).colorCorrect();
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

    *_ledRGB_Ref = rgb;
    *_uiRGB_Ref = rgb.colorCorrect();
}
//* Getters / Setters