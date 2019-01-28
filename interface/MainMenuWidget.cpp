//
// Created by tatiana on 24.01.19.
//

#include "MainMenuWidget.h"
#include "ActionConstants.h"
#include "guichan/gui/WidgetFactory.h"
#include "guichan/gui/ZButton.h"
#include "guichan/gui/ActionEvent.h"

namespace interface {
    MainMenuWidget::MainMenuWidget()
            :gui::ZContainer()
            ,gui::MouseListener()
            ,gui::KeyListener()
            ,gui::ActionListener()
    {

    }

    MainMenuWidget::~MainMenuWidget()
    {

    }

    void MainMenuWidget::init()
    {
        ZContainer::init();

        addMouseListener(gui::MouseListener::downcasted_shared_from_this<MainMenuWidget>());
        addKeyListener(gui::KeyListener::downcasted_shared_from_this<MainMenuWidget>());

        initObjects();
        resize();
    }

    void MainMenuWidget::shutdown()
    {
        clearObjects();

        gui::ZContainer::shutdown();
    }

    void MainMenuWidget::resize()
    {
        const auto width=getWidth();
        const auto height=getHeight();

        const auto widthNew=300;
        const auto heightNew=300;
        const auto widthForButton=150;

        const auto widthShift=(width-widthNew)/2;
        const auto heightShift=(height-heightNew)/2;

        mButtonWidget->setDimension(gui::Rectangle(widthShift, heightShift, width-widthShift*2, height-heightShift*2));

        mNewGameButton->setPosition(widthNew/2 - widthForButton/2, heightNew/2 - 100);
        mNewGameButton->adjustHeight();
        mNewGameButton->setWidth(widthForButton);

        mLoadGameButton->setPosition(widthNew/2 - widthForButton/2, heightNew/2 - 60);
        mLoadGameButton->adjustHeight();
        mLoadGameButton->setWidth(widthForButton);

        mOptionsButton->setPosition(widthNew/2 - widthForButton/2, heightNew/2 - 20);
        mOptionsButton->adjustHeight();
        mOptionsButton->setWidth(widthForButton);

        mQuitButton->setPosition(widthNew/2 - widthForButton/2, heightNew/2 +60);
        mQuitButton->adjustHeight();
        mQuitButton->setWidth(widthForButton);
    }

    void MainMenuWidget::action(const gui::ActionEvent& actionEvent)
    {
        const auto action_id=actionEvent.getId();
        if (action_id==action::NEW_GAME_ACTION)
            distributeActionEvent(action::NEW_GAME_ACTION);
    }

    void MainMenuWidget::initObjects()
    {
        mButtonWidget = gui::WidgetFactory::create_zcontainer();
        mButtonWidget->setVisible(true);
        mButtonWidget->setOpaque(true);
        mButtonWidget->setBaseColor(COLOR_DARK_GRAY);
        mButtonWidget->setFrameHighlightOnly(true);
        mButtonWidget->setFrameSize(2);

        mNewGameButton = gui::WidgetFactory::create_zbutton("New Game");
        //mNewGameButton->setFont(mWhiteFont);
        mNewGameButton->setAlignment(gui::Graphics::Alignment::Center);
        mNewGameButton->adjustSize();
        mNewGameButton->setActionEventId(action::NEW_GAME_ACTION);
        mNewGameButton->addActionListener(ActionListener::downcasted_shared_from_this<MainMenuWidget>());
        mButtonWidget->add(mNewGameButton);

        mLoadGameButton = gui::WidgetFactory::create_zbutton("Load Game");
        //mLoadGameButton->setFont(mWhiteFont);
        mLoadGameButton->setAlignment(gui::Graphics::Alignment::Center);
        mLoadGameButton->adjustSize();
        mLoadGameButton->setActionEventId(action::LOAD_GAME_ACTION);
        mLoadGameButton->addActionListener(ActionListener::downcasted_shared_from_this<MainMenuWidget>());
        mButtonWidget->add(mLoadGameButton);

        mOptionsButton = gui::WidgetFactory::create_zbutton("Options");
        //mOptionsButton->setFont(mWhiteFont);
        mOptionsButton->setAlignment(gui::Graphics::Alignment::Center);
        mOptionsButton->adjustSize();
        mOptionsButton->setActionEventId(action::OPTIONS_ACTION);
        mOptionsButton->addActionListener(ActionListener::downcasted_shared_from_this<MainMenuWidget>());
        mButtonWidget->add(mOptionsButton);

        mQuitButton = gui::WidgetFactory::create_zbutton("Quit");
        //mQuitButton->setFont(mWhiteFont);
        mQuitButton->setAlignment(gui::Graphics::Alignment::Center);
        mQuitButton->adjustSize();
        mQuitButton->setActionEventId(action::QUIT_ACTION);
        mQuitButton->addActionListener(ActionListener::downcasted_shared_from_this<MainMenuWidget>());
        mButtonWidget->add(mQuitButton);

        add(mButtonWidget);
    }

    void MainMenuWidget::clearObjects()
    {
        mButtonWidget->shutdown();
        mButtonWidget=nullptr;

        mNewGameButton->shutdown();
        mNewGameButton=nullptr;

        mLoadGameButton->shutdown();
        mLoadGameButton=nullptr;

        mOptionsButton->shutdown();
        mOptionsButton=nullptr;

        mQuitButton->shutdown();
        mQuitButton=nullptr;
    }
}