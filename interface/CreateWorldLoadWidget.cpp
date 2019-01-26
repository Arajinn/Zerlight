//
// Created by tatiana on 26.01.19.
//

#include "CreateWorldLoadWidget.h"
#include "ActionConstants.h"
#include "guichan/gui/WidgetFactory.h"
#include "guichan/gui/ZLabel.h"
#include "guichan/gui/ZProgressBar.h"
#include "game/core/defines.h"

namespace interface {
    CreateWorldLoadWidget::CreateWorldLoadWidget()
            :gui::ZContainer()
            ,gui::MouseListener()
            ,gui::KeyListener()
    {

    }

    CreateWorldLoadWidget::~CreateWorldLoadWidget()
    {

    }

    void CreateWorldLoadWidget::init()
    {
        ZContainer::init();

        addMouseListener(gui::MouseListener::downcasted_shared_from_this<CreateWorldLoadWidget>());
        addKeyListener(gui::KeyListener::downcasted_shared_from_this<CreateWorldLoadWidget>());

        initObjects();
        resize();
    }

    void CreateWorldLoadWidget::shutdown()
    {
        clearObjects();

        gui::ZContainer::shutdown();
    }

    void CreateWorldLoadWidget::resize()
    {
        const auto width=getWidth();
        const auto height=getHeight();

        const auto indent=20;

        mButtonWidget->setDimension(gui::Rectangle(0, 0, width, height));

        mGenerateLabel->adjustSize();
        mGenerateLabel->setPosition(width/2-mGenerateLabel->getWidth()/2, height/2-indent);

        mGenerateProgress->setPosition(width/2-mGenerateProgress->getWidth()/2, height/2+indent);
    }

    void CreateWorldLoadWidget::initObjects()
    {
        mButtonWidget = gui::WidgetFactory::create_zcontainer();
        mButtonWidget->setVisible(true);
        mButtonWidget->setOpaque(true);
        mButtonWidget->setBaseColor(gui::ZColor::COLOR_DARK_GRAY);
        mButtonWidget->setFrameHighlightOnly(true);
        //mButtonWidget->setFrameSize(2);

        mGenerateLabel = gui::WidgetFactory::create_zlabel("Generating Map");
        mGenerateLabel->adjustSize();
        mButtonWidget->add(mGenerateLabel);

        mGenerateProgress = gui::WidgetFactory::create_zprogressbar();
        mGenerateProgress->setBaseColor(gui::ZColor::COLOR_LIGHT_GRAY);
        mGenerateProgress->setWidth(400);
        mGenerateProgress->setHeight(40);
        mButtonWidget->add(mGenerateProgress);

        add(mButtonWidget);
    }

    void CreateWorldLoadWidget::clearObjects()
    {
        mButtonWidget=nullptr;

        mGenerateLabel=nullptr;
        mGenerateProgress=nullptr;
    }

    void CreateWorldLoadWidget::draw_item(std::shared_ptr<gui::Graphics> graphics)
    {
        const auto progress = GMINSTANCE->getInitProgress();
        mGenerateProgress->setProgress(progress);

        ZContainer::draw_item(graphics);
    }
}