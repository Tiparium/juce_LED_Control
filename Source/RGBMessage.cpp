/*
  ==============================================================================

    RGBMessage.cpp
    Created: 16 Oct 2022 4:28:04pm
    Author:  Tiparium

  ==============================================================================
*/

#include "RGBMessage.h"

RGBMessage::RGBMessage(RGB rgb) {
    _rgb = rgb;
}
RGBMessage::~RGBMessage() {

}
RGB RGBMessage::getRGB() {
    return _rgb;
}