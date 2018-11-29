//
// Created by tatiana on 28.11.18.
//

#ifndef ZERLIGHT_OPTIONCHECKBOX_H
#define ZERLIGHT_OPTIONCHECKBOX_H

#include "guichan/gui/ZCheckBox.h"

namespace gui
{
    class Graphics;
}

class OptionCheckBox : public gui::ZCheckBox{
public:
    OptionCheckBox();
    void init(const std::string& caption);

    ~OptionCheckBox();

    void draw_item(std::shared_ptr<gui::Graphics> graphics);
};


#endif //ZERLIGHT_OPTIONCHECKBOX_H
