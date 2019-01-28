//
// Created by tatiana on 27.01.19.
//

#ifndef ZERLIGHT_HUDWIDGET_H
#define ZERLIGHT_HUDWIDGET_H

#include "guichan/gui/ZContainer.h"
#include "guichan/gui/MouseListener.h"
#include "guichan/gui/KeyListener.h"
#include "guichan/gui/ActionListener.h"

namespace interface
{
    class HUDTopPanel;
}

namespace interface {
    class HUDWidget : public gui::ZContainer, public gui::MouseListener, public gui::KeyListener, public gui::ActionListener{
    public:
        HUDWidget();
        ~HUDWidget();

        void init();
        void shutdown();

        void action(const gui::ActionEvent& actionEvent);

        void resize();
    private:
        void initObjects();
        void clearObjects();

        std::shared_ptr<HUDTopPanel> mHUDTopPanel;
    };
}

#endif //ZERLIGHT_HUDWIDGET_H
