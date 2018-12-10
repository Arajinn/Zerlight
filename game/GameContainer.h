//
// Created by tatiana on 06.12.18.
//

#ifndef ZERLIGHT_GAMECONTAINER_H
#define ZERLIGHT_GAMECONTAINER_H

#include <memory>

namespace properties
{
    struct GameDefinition;
}

namespace game {
    class GameManager;

    class GameContainer {
    public:
        GameContainer();
        ~GameContainer();

        static std::shared_ptr<GameManager> gameManager;
        static std::shared_ptr<properties::GameDefinition> gameDefinition;
    };
}

#endif //ZERLIGHT_GAMECONTAINER_H
