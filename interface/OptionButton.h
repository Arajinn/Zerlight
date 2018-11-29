//
// Created by tatiana on 28.11.18.
//

#ifndef ZERLIGHT_OPTIONBUTTON_H
#define ZERLIGHT_OPTIONBUTTON_H

#include "guichan/gui/ZButton.h"

namespace gui
{
    class Font;
    class Graphics;
    class MouseEvent;
}

class OptionButton : public gui::ZButton{
public:
    OptionButton();
    void init(const std::string& caption);

    ~OptionButton();

    void setHighLightFont(std::shared_ptr<gui::Font> font);


    void draw_item(std::shared_ptr<gui::Graphics> graphics);
    void mouseEntered(gui::MouseEvent& mouseEvent);
    void mouseExited(gui::MouseEvent& mouseEvent);

private:
    std::shared_ptr<gui::Font> mHighLightFont;
    bool mHasMouse;
};


#endif //ZERLIGHT_OPTIONBUTTON_H
