//
// Created by tatiana on 26.01.19.
//

#ifndef ZERLIGHT_NEWGAMEWIDGET_H
#define ZERLIGHT_NEWGAMEWIDGET_H

#include "guichan/gui/ZContainer.h"
#include "guichan/gui/MouseListener.h"
#include "guichan/gui/KeyListener.h"
#include "guichan/gui/ActionListener.h"

namespace gui
{
    class ZButton;
}

namespace interface {
    class NewGameWidget : public gui::ZContainer, public gui::MouseListener, public gui::KeyListener, public gui::ActionListener{
    public:
        NewGameWidget();
        ~NewGameWidget();

        void init();
        void shutdown();

        void action(const gui::ActionEvent& actionEvent);

        void resize();
    private:
        void initObjects();
        void clearObjects();

        std::shared_ptr<gui::ZContainer> mButtonWidget;
        std::shared_ptr<gui::ZButton> mBackButton;
        std::shared_ptr<gui::ZButton> mAdvancedSetupButton;
        std::shared_ptr<gui::ZButton> mGenerateButton;
    };
}

#endif //ZERLIGHT_NEWGAMEWIDGET_H
