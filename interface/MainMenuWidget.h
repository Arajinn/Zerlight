//
// Created by tatiana on 24.01.19.
//

#ifndef ZERLIGHT_MAINMENUWIDGET_H
#define ZERLIGHT_MAINMENUWIDGET_H

#include "guichan/gui/ZContainer.h"
#include "guichan/gui/MouseListener.h"
#include "guichan/gui/KeyListener.h"
#include "guichan/gui/ActionListener.h"

namespace gui
{
    class ZButton;
}

namespace interface {
    class MainMenuWidget : public gui::ZContainer, public gui::MouseListener, public gui::KeyListener, public gui::ActionListener{
    public:
        MainMenuWidget();
        ~MainMenuWidget();

        void init();
        void shutdown();

        void action(const gui::ActionEvent& actionEvent);

        void resize();
    private:
        void initObjects();
        void clearObjects();

        std::shared_ptr<gui::ZContainer> mButtonWidget;
        std::shared_ptr<gui::ZButton> mNewGameButton;
        std::shared_ptr<gui::ZButton> mLoadGameButton;
        std::shared_ptr<gui::ZButton> mOptionsButton;
        std::shared_ptr<gui::ZButton> mQuitButton;
    };
}

#endif //ZERLIGHT_STARTWIDGET_H
