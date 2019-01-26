//
// Created by tatiana on 26.01.19.
//

#ifndef ZERLIGHT_CREATEWORLDLOADWIDGET_H
#define ZERLIGHT_CREATEWORLDLOADWIDGET_H

#include "guichan/gui/ZContainer.h"
#include "guichan/gui/MouseListener.h"
#include "guichan/gui/KeyListener.h"

namespace gui
{
    class ZLabel;
    class ZProgressBar;
}

namespace interface {
    class CreateWorldLoadWidget : public gui::ZContainer, public gui::MouseListener, public gui::KeyListener{
    public:
        CreateWorldLoadWidget();
        ~CreateWorldLoadWidget();

        void init();
        void shutdown();

        void resize();

        void draw_item(std::shared_ptr<gui::Graphics> graphics) override;
    private:
        void initObjects();
        void clearObjects();

        std::shared_ptr<gui::ZContainer> mButtonWidget;
        std::shared_ptr<gui::ZLabel> mGenerateLabel;
        std::shared_ptr<gui::ZProgressBar> mGenerateProgress;
    };
}

#endif //ZERLIGHT_CREATEWORLDLOADWIDGET_H
