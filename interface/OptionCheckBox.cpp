//
// Created by tatiana on 28.11.18.
//

#include "OptionCheckBox.h"
#include "guichan/gui/Graphics.h"

OptionCheckBox::OptionCheckBox()
        :gui::ZCheckBox()
{

}

void OptionCheckBox::init(const std::string& caption)
{
    gui::ZCheckBox::init(caption);
}

OptionCheckBox::~OptionCheckBox()
{

}

void OptionCheckBox::draw_item(std::shared_ptr<gui::Graphics> graphics)
{
    graphics->setFont(getFont());
    graphics->setColor(getForegroundColor());

    // First draw the caption
    graphics->drawText(mCaption, 0, 0);

    // Calculate the x coordinate for the box
    int x = getFont()->getWidth(mCaption) + getHeight() / 2;

    // Push a clip area where the box should draw itself
    graphics->pushClipArea(gui::Rectangle(x, 0, getWidth() - x, getHeight()));
    drawBox(graphics);
    // Pop the cliparea. VERY IMPORTANT!
    graphics->popClipArea();
}