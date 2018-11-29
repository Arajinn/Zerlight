//
// Created by tatiana on 28.11.18.
//

#include "OptionButton.h"

OptionButton::OptionButton()
        :gui::ZButton()
{

}

void OptionButton::init(const std::string& caption)
{
    gui::ZButton::init(caption);

    mHasMouse=false;

    setFrameSize(0);
}

OptionButton::~OptionButton()
{
}

void OptionButton::draw_item(std::shared_ptr<gui::Graphics> graphics)
{
    if (mHasMouse)
    {
        graphics->setFont(mHighLightFont);
        graphics->drawText(getCaption(),0,0);
    }
    else
    {
        graphics->setFont(getFont());
        graphics->drawText(getCaption(),0,0);
    }
}

void OptionButton::setHighLightFont(std::shared_ptr<gui::Font> font)
{
    mHighLightFont = font;
}

void OptionButton::mouseEntered(gui::MouseEvent& mouseEvent)
{
    ZButton::mouseEntered(mouseEvent);
    mHasMouse = true;
}

void OptionButton::mouseExited(gui::MouseEvent& mouseEvent)
{
    ZButton::mouseExited(mouseEvent);
    mHasMouse = false;
}