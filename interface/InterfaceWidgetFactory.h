//
// Created by tatiana on 28.11.18.
//

#ifndef ZERLIGHT_INTERFACEWIDGETFACTORY_H
#define ZERLIGHT_INTERFACEWIDGETFACTORY_H

#include <memory>

class OptionButton;
class OptionCheckBox;
class MapWidget;

class InterfaceWidgetFactory
{
public:
    static std::shared_ptr<OptionButton> create_optionbutton(const std::string& caption);
    static std::shared_ptr<OptionCheckBox> create_optioncheckbox(const std::string& caption);
    static std::shared_ptr<MapWidget> create_mapwidget();
};


#endif //ZERLIGHT_INTERFACEWIDGETFACTORY_H
