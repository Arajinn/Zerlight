//
// Created by tatiana on 27.01.19.
//

#ifndef ZERLIGHT_HUDTOPPANEL_H
#define ZERLIGHT_HUDTOPPANEL_H

#include "guichan/gui/ZContainer.h"
#include "guichan/gui/MouseListener.h"
#include "guichan/gui/KeyListener.h"
#include "guichan/gui/ActionListener.h"

namespace gui
{
    class ZButton;
}

namespace interface {
    class HUDTopPanel : public gui::ZContainer, public gui::MouseListener, public gui::KeyListener, public gui::ActionListener{
    public:
        HUDTopPanel();
        ~HUDTopPanel();

        void init();
        void shutdown();

        void action(const gui::ActionEvent& actionEvent);

        void resize();
    private:
        void initObjects();
        void clearObjects();

        std::shared_ptr<gui::ZContainer> mButtonWidget;
        std::shared_ptr<gui::ZButton> mKingdomButton;
        std::shared_ptr<gui::ZButton> mStocksButton;
        std::shared_ptr<gui::ZButton> mPopulationButton;
        std::shared_ptr<gui::ZButton> mMilitaryButton;
        std::shared_ptr<gui::ZButton> mEventsButton;
        std::shared_ptr<gui::ZButton> mHelpButton;
    };
}

#endif //ZERLIGHT_HUDTOPPANEL_H
