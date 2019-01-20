//
// Created by tatiana on 30.11.18.
//

#ifndef ZERLIGHT_DEFINES_H
#define ZERLIGHT_DEFINES_H

#include "game/core/GameManager.h"
#include "game/map/Region.h"
#include "game/map/Map.h"
#include "game/properties/GameDefinition.h"
#include "game/GameContainer.h"

#define GMINSTANCE game::GameContainer::gameManager
#define REGION GMINSTANCE->region()
#define WORLD_MAP REGION->map()
#define FORTRESS REGION->fortress()
#define AIDIRECTOR GMINSTANCE->aiDirector()

#define GAME_DEFINITIONS game::GameContainer::gameDefinition
#define GAME_SETTINGS GAME_DEFINITIONS->gameSettings()
#define TERRAIN_SETTINGS GAME_DEFINITIONS->terrainSettings()
#define ITEM_SETTINGS GAME_DEFINITIONS->itemSettings()
#define UNIFORM_SETTINGS GAME_DEFINITIONS->uniformSettings()
#define CHARACTER_SETTINGS GAME_DEFINITIONS->characterSettings()

#endif //ZERLIGHT_DEFINES_H
