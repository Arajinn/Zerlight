//
// Created by tatiana on 28.11.18.
//

#ifndef ZERLIGHT_INTERFACEWIDGETFACTORY_H
#define ZERLIGHT_INTERFACEWIDGETFACTORY_H

#include <memory>

class OptionButton;
class OptionCheckBox;
class MapWidget;

namespace interface
{
    class MainMenuWidget;
    class NewGameWidget;
    class CreateWorldLoadWidget;
}

class InterfaceWidgetFactory
{
public:
    static std::shared_ptr<OptionButton> create_optionbutton(const std::string& caption);
    static std::shared_ptr<OptionCheckBox> create_optioncheckbox(const std::string& caption);
    static std::shared_ptr<MapWidget> create_mapwidget();
    static std::shared_ptr<interface::MainMenuWidget> create_mainmenuwidget();
    static std::shared_ptr<interface::NewGameWidget> create_newgamewidget();
    static std::shared_ptr<interface::CreateWorldLoadWidget> create_createworldloadwidget();
};


#endif //ZERLIGHT_INTERFACEWIDGETFACTORY_H
