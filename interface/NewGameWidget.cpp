//
// Created by tatiana on 26.01.19.
//

#include "NewGameWidget.h"
#include "ActionConstants.h"
#include "guichan/gui/WidgetFactory.h"
#include "guichan/gui/ZButton.h"
#include "guichan/gui/ActionEvent.h"

namespace interface {
    NewGameWidget::NewGameWidget()
            :gui::ZContainer()
            ,gui::MouseListener()
            ,gui::KeyListener()
            ,gui::ActionListener()
    {

    }

    NewGameWidget::~NewGameWidget()
    {

    }

    void NewGameWidget::init()
    {
        ZContainer::init();

        addMouseListener(gui::MouseListener::downcasted_shared_from_this<NewGameWidget>());
        addKeyListener(gui::KeyListener::downcasted_shared_from_this<NewGameWidget>());

        initObjects();
        resize();
    }

    void NewGameWidget::shutdown()
    {
        clearObjects();

        gui::ZContainer::shutdown();
    }

    void NewGameWidget::resize()
    {
        const auto width=getWidth();
        const auto height=getHeight();

        const auto widthNew=600;
        const auto heightNew=300;
        const auto indent=20;

        const auto widthShift=(width-widthNew)/2;
        const auto heightShift=(height-heightNew)/2;

        mButtonWidget->setDimension(gui::Rectangle(widthShift, heightShift, width-widthShift*2, height-heightShift*2));

        mBackButton->adjustSize();
        mBackButton->setPosition(indent, heightNew-indent-mBackButton->getHeight());

        mGenerateButton->adjustSize();
        mGenerateButton->setPosition(widthNew-indent-mGenerateButton->getWidth(), heightNew-indent-mGenerateButton->getHeight());

        mAdvancedSetupButton->adjustSize();
        mAdvancedSetupButton->setPosition(widthNew-indent-mGenerateButton->getWidth()-indent-mAdvancedSetupButton->getWidth(), heightNew-indent-mAdvancedSetupButton->getHeight());
    }

    void NewGameWidget::action(const gui::ActionEvent& actionEvent)
    {
        const auto action_id=actionEvent.getId();
        if (action_id==action::GENERATE_NEW_WORLD_ACTION)
            distributeActionEvent(action::GENERATE_NEW_WORLD_ACTION);
        else if (action_id==action::BACK_TO_MAIN_WINDOW_ACTION)
            distributeActionEvent(action::BACK_TO_MAIN_WINDOW_ACTION);
    }

    void NewGameWidget::initObjects()
    {
        mButtonWidget = gui::WidgetFactory::create_zcontainer();
        mButtonWidget->setVisible(true);
        mButtonWidget->setOpaque(true);
        mButtonWidget->setBaseColor(COLOR_DARK_GRAY);
        mButtonWidget->setFrameHighlightOnly(true);
        mButtonWidget->setFrameSize(2);

        mBackButton = gui::WidgetFactory::create_zbutton("Back");
        //mNewGameButton->setFont(mWhiteFont);
        mBackButton->setAlignment(gui::Graphics::Alignment::Center);
        mBackButton->adjustSize();
        mBackButton->setActionEventId(action::BACK_TO_MAIN_WINDOW_ACTION);
        mBackButton->addActionListener(ActionListener::downcasted_shared_from_this<NewGameWidget>());
        mButtonWidget->add(mBackButton);

        mAdvancedSetupButton = gui::WidgetFactory::create_zbutton("Advanced Setup");
        //mLoadGameButton->setFont(mWhiteFont);
        mAdvancedSetupButton->setAlignment(gui::Graphics::Alignment::Center);
        mAdvancedSetupButton->adjustSize();
        mAdvancedSetupButton->setActionEventId(action::ADVANCED_SETUP_ACTION);
        mAdvancedSetupButton->addActionListener(ActionListener::downcasted_shared_from_this<NewGameWidget>());
        mButtonWidget->add(mAdvancedSetupButton);

        mGenerateButton = gui::WidgetFactory::create_zbutton("Generate");
        //mOptionsButton->setFont(mWhiteFont);
        mGenerateButton->setAlignment(gui::Graphics::Alignment::Center);
        mGenerateButton->adjustSize();
        mGenerateButton->setActionEventId(action::GENERATE_NEW_WORLD_ACTION);
        mGenerateButton->addActionListener(ActionListener::downcasted_shared_from_this<NewGameWidget>());
        mButtonWidget->add(mGenerateButton);

        add(mButtonWidget);
    }

    void NewGameWidget::clearObjects()
    {
        mButtonWidget->shutdown();
        mButtonWidget=nullptr;

        mBackButton->shutdown();
        mBackButton=nullptr;

        mAdvancedSetupButton->shutdown();
        mAdvancedSetupButton=nullptr;

        mGenerateButton->shutdown();
        mGenerateButton=nullptr;
    }
}