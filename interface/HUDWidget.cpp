//
// Created by tatiana on 27.01.19.
//

#include "HUDWidget.h"
#include "ActionConstants.h"
#include "interface/InterfaceWidgetFactory.h"
#include "interface/HUDTopPanel.h"
#include "guichan/gui/ActionEvent.h"
#include "guichan/gui/WidgetFactory.h"

namespace interface {
    HUDWidget::HUDWidget()
            :gui::ZContainer()
            ,gui::MouseListener()
            ,gui::KeyListener()
            ,gui::ActionListener()
    {

    }

    HUDWidget::~HUDWidget()
    {

    }

    void HUDWidget::init()
    {
        ZContainer::init();

        addMouseListener(gui::MouseListener::downcasted_shared_from_this<HUDWidget>());
        addKeyListener(gui::KeyListener::downcasted_shared_from_this<HUDWidget>());

        initObjects();
        resize();
    }

    void HUDWidget::shutdown()
    {
        clearObjects();

        gui::ZContainer::shutdown();
    }

    void HUDWidget::resize()
    {
        const auto width=getWidth();
        const auto height=getHeight();

        mHUDTopPanel->setDimension(gui::Rectangle(0, 0, width, height));
        mHUDTopPanel->resize();
    }

    void HUDWidget::action(const gui::ActionEvent& actionEvent)
    {
        const auto action_id=actionEvent.getId();
    }

    void HUDWidget::initObjects()
    {
        mHUDTopPanel = InterfaceWidgetFactory::create_hudtoppanel();
        mHUDTopPanel->setVisible(true);
        mHUDTopPanel->setOpaque(false);
        mHUDTopPanel->setBaseColor(COLOR_DARK_GRAY);
        mHUDTopPanel->setFrameSize(0);

        add(mHUDTopPanel);
    }

    void HUDWidget::clearObjects()
    {
        mHUDTopPanel->shutdown();
        mHUDTopPanel=nullptr;
    }
}