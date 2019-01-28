//
// Created by tatiana on 27.01.19.
//

#include "HUDTopPanel.h"
#include "ActionConstants.h"
#include "guichan/gui/ActionEvent.h"
#include "guichan/gui/WidgetFactory.h"
#include "guichan/gui/ZButton.h"

namespace interface {
    HUDTopPanel::HUDTopPanel()
            :gui::ZContainer()
            ,gui::MouseListener()
            ,gui::KeyListener()
            ,gui::ActionListener()
    {

    }

    HUDTopPanel::~HUDTopPanel()
    {

    }

    void HUDTopPanel::init()
    {
        ZContainer::init();

        addMouseListener(gui::MouseListener::downcasted_shared_from_this<HUDTopPanel>());
        addKeyListener(gui::KeyListener::downcasted_shared_from_this<HUDTopPanel>());

        initObjects();
        resize();
    }

    void HUDTopPanel::shutdown()
    {
        clearObjects();

        gui::ZContainer::shutdown();
    }

    void HUDTopPanel::resize()
    {
        const auto indent=10;

        mKingdomButton->adjustSize();
        mStocksButton->adjustSize();
        mPopulationButton->adjustSize();
        mMilitaryButton->adjustSize();
        mEventsButton->adjustSize();
        mHelpButton->adjustSize();

        const auto widthNew=indent+mKingdomButton->getWidth()+indent+mStocksButton->getWidth()+indent+mPopulationButton->getWidth()
                +indent+mMilitaryButton->getWidth()+indent+mEventsButton->getWidth()+indent+mHelpButton->getWidth()+indent;
        const auto heightNew=indent+mKingdomButton->getHeight()+indent;

        const auto width=getWidth();
        const auto widthShift=(width-widthNew)/2;

        mButtonWidget->setDimension(gui::Rectangle(widthShift, 0, width-widthShift*2, heightNew));

        auto pos=indent;
        mKingdomButton->setPosition(pos, indent);

        pos+=mKingdomButton->getWidth() + indent;
        mStocksButton->setPosition(pos, indent);

        pos+=mStocksButton->getWidth() + indent;
        mPopulationButton->setPosition(pos, indent);

        pos+=mPopulationButton->getWidth() + indent;
        mMilitaryButton->setPosition(pos, indent);

        pos+=mMilitaryButton->getWidth() + indent;
        mEventsButton->setPosition(pos, indent);

        pos+=mEventsButton->getWidth() + indent;
        mHelpButton->setPosition(pos, indent);
    }

    void HUDTopPanel::action(const gui::ActionEvent& actionEvent)
    {
        const auto action_id=actionEvent.getId();
    }

    void HUDTopPanel::initObjects()
    {
        mButtonWidget = gui::WidgetFactory::create_zcontainer();
        mButtonWidget->setVisible(true);
        mButtonWidget->setOpaque(false);
        mButtonWidget->setBaseColor(COLOR_DARK_GRAY);
        mButtonWidget->setFrameSize(0);

        mKingdomButton = gui::WidgetFactory::create_zbutton("Kingdom");
        mKingdomButton->setUseFont(ZWidget::UseFont::Small);
        mKingdomButton->setAlignment(gui::Graphics::Alignment::Center);
        mKingdomButton->adjustSize();
        //mKingdomButton->setActionEventId(action::NEW_GAME_ACTION);
        mKingdomButton->addActionListener(ActionListener::downcasted_shared_from_this<HUDTopPanel>());
        mButtonWidget->add(mKingdomButton);

        mStocksButton = gui::WidgetFactory::create_zbutton("Stocks");
        mStocksButton->setUseFont(ZWidget::UseFont::Small);
        mStocksButton->setAlignment(gui::Graphics::Alignment::Center);
        mStocksButton->adjustSize();
        //mStocksButton->setActionEventId(action::LOAD_GAME_ACTION);
        mStocksButton->addActionListener(ActionListener::downcasted_shared_from_this<HUDTopPanel>());
        mButtonWidget->add(mStocksButton);

        mPopulationButton = gui::WidgetFactory::create_zbutton("Population");
        mPopulationButton->setUseFont(ZWidget::UseFont::Small);
        mPopulationButton->setAlignment(gui::Graphics::Alignment::Center);
        mPopulationButton->adjustSize();
        //mPopulationButton->setActionEventId(action::OPTIONS_ACTION);
        mPopulationButton->addActionListener(ActionListener::downcasted_shared_from_this<HUDTopPanel>());
        mButtonWidget->add(mPopulationButton);

        mMilitaryButton = gui::WidgetFactory::create_zbutton("Military");
        mMilitaryButton->setUseFont(ZWidget::UseFont::Small);
        mMilitaryButton->setAlignment(gui::Graphics::Alignment::Center);
        mMilitaryButton->adjustSize();
        //mMilitaryButton->setActionEventId(action::QUIT_ACTION);
        mMilitaryButton->addActionListener(ActionListener::downcasted_shared_from_this<HUDTopPanel>());
        mButtonWidget->add(mMilitaryButton);

        mEventsButton = gui::WidgetFactory::create_zbutton("Events");
        mEventsButton->setUseFont(ZWidget::UseFont::Small);
        mEventsButton->setAlignment(gui::Graphics::Alignment::Center);
        mEventsButton->adjustSize();
        //mEventsButton->setActionEventId(action::QUIT_ACTION);
        mEventsButton->addActionListener(ActionListener::downcasted_shared_from_this<HUDTopPanel>());
        mButtonWidget->add(mEventsButton);

        mHelpButton = gui::WidgetFactory::create_zbutton("Help");
        mHelpButton->setUseFont(ZWidget::UseFont::Small);
        mHelpButton->setAlignment(gui::Graphics::Alignment::Center);
        mHelpButton->adjustSize();
        //mHelpButton->setActionEventId(action::QUIT_ACTION);
        mHelpButton->addActionListener(ActionListener::downcasted_shared_from_this<HUDTopPanel>());
        mButtonWidget->add(mHelpButton);

        add(mButtonWidget);
    }

    void HUDTopPanel::clearObjects()
    {
        mButtonWidget->shutdown();
        mButtonWidget=nullptr;

        mKingdomButton->shutdown();
        mKingdomButton=nullptr;

        mStocksButton->shutdown();
        mStocksButton=nullptr;

        mPopulationButton->shutdown();
        mPopulationButton=nullptr;

        mMilitaryButton->shutdown();
        mMilitaryButton=nullptr;

        mEventsButton->shutdown();
        mEventsButton=nullptr;

        mHelpButton->shutdown();
        mHelpButton=nullptr;
    }
}