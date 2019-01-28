//
// Created by tatiana on 28.11.18.
//

#include "InterfaceWidgetFactory.h"
#include "OptionButton.h"
#include "OptionCheckBox.h"
#include "MapWidget.h"
#include "MainMenuWidget.h"
#include "NewGameWidget.h"
#include "CreateWorldLoadWidget.h"
#include "HUDWidget.h"
#include "HUDTopPanel.h"

std::shared_ptr<OptionButton> InterfaceWidgetFactory::create_optionbutton(const std::string &caption)
{
    auto ptr=std::shared_ptr<OptionButton>(new OptionButton());
    ptr->init(caption);
    return ptr;
}

std::shared_ptr<OptionCheckBox> InterfaceWidgetFactory::create_optioncheckbox(const std::string& caption)
{
    auto ptr=std::shared_ptr<OptionCheckBox>(new OptionCheckBox());
    ptr->init(caption);
    return ptr;
}

std::shared_ptr<MapWidget> InterfaceWidgetFactory::create_mapwidget()
{
    auto ptr=std::make_shared<MapWidget>();
    ptr->init();
    return ptr;
}

std::shared_ptr<interface::MainMenuWidget> InterfaceWidgetFactory::create_mainmenuwidget()
{
    auto ptr=std::make_shared<interface::MainMenuWidget>();
    ptr->init();
    return ptr;
}

std::shared_ptr<interface::NewGameWidget> InterfaceWidgetFactory::create_newgamewidget()
{
    auto ptr=std::make_shared<interface::NewGameWidget>();
    ptr->init();
    return ptr;
}

std::shared_ptr<interface::CreateWorldLoadWidget> InterfaceWidgetFactory::create_createworldloadwidget()
{
    auto ptr=std::make_shared<interface::CreateWorldLoadWidget>();
    ptr->init();
    return ptr;
}

std::shared_ptr<interface::HUDWidget> InterfaceWidgetFactory::create_hudwidget()
{
    auto ptr=std::make_shared<interface::HUDWidget>();
    ptr->init();
    return ptr;
}

std::shared_ptr<interface::HUDTopPanel> InterfaceWidgetFactory::create_hudtoppanel()
{
    auto ptr=std::make_shared<interface::HUDTopPanel>();
    ptr->init();
    return ptr;
}