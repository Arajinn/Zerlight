//
// Created by tatiana on 28.11.18.
//

#include "InterfaceWidgetFactory.h"
#include "OptionButton.h"
#include "OptionCheckBox.h"
#include "MapWidget.h"

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
    auto ptr=std::shared_ptr<MapWidget>(new MapWidget());
    ptr->init();
    return ptr;
}