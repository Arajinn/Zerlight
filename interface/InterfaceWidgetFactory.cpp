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

void DeleteOptionButton(OptionButton* obj)
{
    obj->shutdown();
    std::cout << "Deleting DemoButton\n";
}

std::shared_ptr<OptionButton> InterfaceWidgetFactory::create_optionbutton(const std::string &caption)
{
    auto ptr=std::shared_ptr<OptionButton>(new OptionButton(),DeleteOptionButton);
    ptr->init(caption);
    return ptr;
}

void DeleteOptionCheckBox(OptionCheckBox* obj)
{
    obj->shutdown();
    std::cout << "Deleting DemoButton\n";
}

std::shared_ptr<OptionCheckBox> InterfaceWidgetFactory::create_optioncheckbox(const std::string& caption)
{
    auto ptr=std::shared_ptr<OptionCheckBox>(new OptionCheckBox(),DeleteOptionCheckBox);
    ptr->init(caption);
    return ptr;
}

void DeleteMapContainer(MapWidget* obj)
{
    obj->shutdown();
    std::cout << "Deleting DemoButton\n";
}

std::shared_ptr<MapWidget> InterfaceWidgetFactory::create_mapwidget()
{
    auto ptr=std::make_shared<MapWidget>();
    ptr->init();
    return ptr;
}

void DeleteMainMenuWidget(interface::MainMenuWidget* obj)
{
    obj->shutdown();
    std::cout << "Deleting StartWidget\n";
}

std::shared_ptr<interface::MainMenuWidget> InterfaceWidgetFactory::create_mainmenuwidget()
{
    auto ptr=std::shared_ptr<interface::MainMenuWidget>(new interface::MainMenuWidget(),DeleteMainMenuWidget);
    ptr->init();
    return ptr;
}

void DeleteNewGameWidget(interface::NewGameWidget* obj)
{
    obj->shutdown();
    std::cout << "Deleting StartWidget\n";
}

std::shared_ptr<interface::NewGameWidget> InterfaceWidgetFactory::create_newgamewidget()
{
    auto ptr=std::shared_ptr<interface::NewGameWidget>(new interface::NewGameWidget(),DeleteNewGameWidget);
    ptr->init();
    return ptr;
}

void DeleteCreateWorldLoadWidget(interface::CreateWorldLoadWidget* obj)
{
    obj->shutdown();
    std::cout << "Deleting StartWidget\n";
}

std::shared_ptr<interface::CreateWorldLoadWidget> InterfaceWidgetFactory::create_createworldloadwidget()
{
    auto ptr=std::shared_ptr<interface::CreateWorldLoadWidget>(new interface::CreateWorldLoadWidget(),DeleteCreateWorldLoadWidget);
    ptr->init();
    return ptr;
}