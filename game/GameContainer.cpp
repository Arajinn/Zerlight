//
// Created by tatiana on 06.12.18.
//

#include "GameContainer.h"
#include "core/GameManager.h"
#include "properties/GameDefinition.h"

namespace game
{
    std::shared_ptr<GameManager> GameContainer::gameManager=nullptr;
    std::shared_ptr<properties::GameDefinition> GameContainer::gameDefinition=nullptr;

    GameContainer::GameContainer()
    {
        gameManager=GameManager::create();
        gameDefinition=std::make_shared<properties::GameDefinition>();
    }

    GameContainer::~GameContainer()
    {
        gameManager=nullptr;
    }
}