cmake_minimum_required(VERSION 3.12)
project(Zerlight)

set(CMAKE_CXX_STANDARD 14)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer)

FIND_PACKAGE(OpenGL)
FIND_PACKAGE(Threads)
FIND_PACKAGE(Boost)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

add_executable(Zerlight main.cpp
        guichan/gui/ZWidget.cpp guichan/gui/ZWidget.h
        guichan/gui/ZColor.cpp guichan/gui/ZColor.h
        guichan/gui/Rectangle.cpp guichan/gui/Rectangle.h
        guichan/gui/Font.cpp guichan/gui/Font.h
        guichan/gui/DefaultFont.cpp guichan/gui/DefaultFont.h
        guichan/gui/Graphics.cpp guichan/gui/Graphics.h
        guichan/gui/Exception.cpp guichan/gui/Exception.h
        guichan/gui/ClipRectangle.cpp guichan/gui/ClipRectangle.h
        guichan/gui/ZImage.cpp guichan/gui/ZImage.h
        guichan/gui/ImageLoader.cpp guichan/gui/ImageLoader.h
        guichan/gui/Event.cpp guichan/gui/Event.h
        guichan/gui/DeathListener.cpp guichan/gui/DeathListener.h
        guichan/gui/FocusListener.cpp guichan/gui/FocusListener.h
        guichan/gui/FocusHandler.cpp guichan/gui/FocusHandler.h
        guichan/gui/WidgetListener.cpp guichan/gui/WidgetListener.h
        guichan/gui/ActionEvent.cpp guichan/gui/ActionEvent.h
        guichan/gui/ActionListener.cpp guichan/gui/ActionListener.h
        guichan/gui/MouseListener.cpp guichan/gui/MouseListener.h
        guichan/gui/InputEvent.cpp guichan/gui/InputEvent.h
        guichan/gui/MouseEvent.cpp guichan/gui/MouseEvent.h
        guichan/gui/KeyListener.cpp guichan/gui/KeyListener.h
        guichan/gui/KeyEvent.cpp guichan/gui/KeyEvent.h
        guichan/gui/Key.cpp guichan/gui/Key.h
        guichan/gui/ZButton.cpp guichan/gui/ZButton.h
        guichan/gui/MultipleInheritable.h
        guichan/gui/WidgetFactory.cpp guichan/gui/WidgetFactory.h
        guichan/gui/ZCheckBox.cpp guichan/gui/ZCheckBox.h
        guichan/gui/Input.cpp guichan/gui/Input.h
        guichan/gui/KeyInput.cpp guichan/gui/KeyInput.h
        guichan/gui/MouseInput.cpp guichan/gui/MouseInput.h
        guichan/gui/Gui.cpp guichan/gui/Gui.h
        guichan/gui/ZContainer.cpp guichan/gui/ZContainer.h
        guichan/gui/ContainerEvent.cpp guichan/gui/ContainerEvent.h
        guichan/gui/ContainerListener.cpp guichan/gui/ContainerListener.h
        guichan/gui/ImageFont.cpp guichan/gui/ImageFont.h
        guichan/gui/ZIcon.cpp guichan/gui/ZIcon.h
        guichan/gui/ZLabel.cpp guichan/gui/ZLabel.h
        guichan/gui/ZTextBox.cpp guichan/gui/ZTextBox.h
        guichan/gui/ZText.cpp guichan/gui/ZText.h
        guichan/gui/ListModel.cpp guichan/gui/ListModel.h
        guichan/gui/SelectionEvent.cpp guichan/gui/SelectionEvent.h
        guichan/gui/SelectionListener.cpp guichan/gui/SelectionListener.h
        guichan/gui/ZScrollArea.cpp guichan/gui/ZScrollArea.h
        guichan/gui/ZListBox.cpp guichan/gui/ZListBox.h
        guichan/gui/ZDropDown.cpp guichan/gui/ZDropDown.h
        guichan/gui/ZSlider.cpp guichan/gui/ZSlider.h
        guichan/gui/ZProgressBar.cpp guichan/gui/ZProgressBar.h
        guichan/sdl/SDLGraphics.cpp guichan/sdl/SDLGraphics.h
        guichan/sdl/SDLPixel.cpp guichan/sdl/SDLPixel.h
        guichan/sdl/SDLImage.cpp guichan/sdl/SDLImage.h
        guichan/sdl/SDLInput.cpp guichan/sdl/SDLInput.h
        guichan/sdl/SDLDeleters.cpp guichan/sdl/SDLDeleters.h
        guichan/sdl/SDLImageLoader.cpp guichan/sdl/SDLImageLoader.h
        guichan/sdl/OpenGLSDLImageLoader.cpp guichan/sdl/OpenGLSDLImageLoader.h
        guichan/sdl/OpenGLImage.cpp guichan/sdl/OpenGLImage.h
        guichan/sdl/OpenGLGraphics.cpp guichan/sdl/OpenGLGraphics.h
        interface/ActionConstants.h
        interface/OptionCheckBox.cpp interface/OptionCheckBox.h
        interface/ResolutionListModel.cpp interface/ResolutionListModel.h
        interface/OptionButton.cpp interface/OptionButton.h
        interface/MainActionListener.cpp interface/MainActionListener.h
        interface/MainActionOptions.cpp interface/MainActionGame.cpp
        interface/InterfaceWidgetFactory.cpp interface/InterfaceWidgetFactory.h
        interface/MapWidget.cpp interface/MapWidget.h
        game/core/enums.h
        game/core/GameManager.cpp game/core/GameManager.h
        game/core/GameEntity.cpp game/core/GameEntity.h
        game/core/Liquid.cpp game/core/Liquid.h
        game/core/Grass.cpp game/core/Grass.h
        game/core/Ramp.cpp game/core/Ramp.h
        game/core/Item.cpp game/core/Item.h
        game/core/ItemHistory.cpp game/core/ItemHistory.h
        game/core/Character.cpp game/core/Character.h
        game/core/CharacterHistory.cpp game/core/CharacterHistory.h
        game/core/CharacterAttribute.cpp game/core/CharacterAttribute.h
        game/core/Body.cpp game/core/Body.h
        game/core/BodySection.cpp game/core/BodySection.h
        game/core/BodyPart.cpp game/core/BodyPart.h
        game/core/HealthStatusEffect.cpp game/core/HealthStatusEffect.h
        game/core/Mind.cpp game/core/Mind.h
        game/core/Squad.cpp game/core/Squad.h
        game/core/SquadPosition.cpp game/core/SquadPosition.h
        game/core/Construction.cpp game/core/Construction.h
        game/core/Stairs.cpp game/core/Stairs.h
        game/core/Scaffolding.cpp game/core/Scaffolding.h
        game/core/ConstructedTerrain.cpp game/core/ConstructedTerrain.h
        game/core/StorageContainer.cpp game/core/StorageContainer.h
        game/core/ResourcePile.cpp game/core/ResourcePile.h
        game/core/AIDirector.cpp game/core/AIDirector.h
        game/core/Fortress.cpp game/core/Fortress.h
        game/core/Pathfinder.cpp game/core/Pathfinder.h
        game/core/PriorityQueue.cpp game/core/PriorityQueue.h
        game/core/Job.cpp game/core/Job.h
        game/core/StockManager.cpp game/core/StockManager.h
        game/core/Formation.cpp game/core/Formation.h
        game/behavior/TaskResult.h
        game/behavior/Node.cpp game/behavior/Node.h
        game/behavior/Action.cpp game/behavior/Action.h
        game/behavior/CompositeNode.cpp game/behavior/CompositeNode.h
        game/behavior/Selector.cpp game/behavior/Selector.h
        game/behavior/Sequence.cpp game/behavior/Sequence.h
        game/behavior/Condition.cpp game/behavior/Condition.h
        game/behavior/SatisfyDyingOfThirst.cpp game/behavior/SatisfyDyingOfThirst.h
        game/behavior/GetDrinkOrDrinkSelf.cpp game/behavior/GetDrinkOrDrinkSelf.h
        game/behavior/GetFed.cpp game/behavior/GetFed.h
        game/behavior/PathToNeedGoal.cpp game/behavior/PathToNeedGoal.h
        game/behavior/SatisfyStarvation.cpp game/behavior/SatisfyStarvation.h
        game/behavior/SatisfyThirst.cpp game/behavior/SatisfyThirst.h
        game/behavior/StopStarvationOrEatAtTable.cpp game/behavior/StopStarvationOrEatAtTable.h
        game/behavior/LookingForFood.cpp game/behavior/LookingForFood.h
        game/behavior/GetFedOrFeedSelfAtTable.cpp game/behavior/GetFedOrFeedSelfAtTable.h
        game/behavior/GetFedOrFeedSelf.cpp game/behavior/GetFedOrFeedSelf.h
        game/behavior/FindFoodAndEatAtTable.cpp game/behavior/FindFoodAndEatAtTable.h
        game/behavior/FindFood.cpp game/behavior/FindFood.h
        game/behavior/LookForFood.cpp game/behavior/LookForFood.h
        game/behavior/GetFood.cpp game/behavior/GetFood.h
        game/behavior/LookForChairThenEatAtTable.cpp game/behavior/LookForChairThenEatAtTable.h
        game/behavior/PickupFood.cpp game/behavior/PickupFood.h
        game/behavior/LookForChair.cpp game/behavior/LookForChair.h
        game/behavior/EatAtTable.cpp game/behavior/EatAtTable.h
        game/behavior/PathToFood.cpp game/behavior/PathToFood.h
        game/behavior/FreeHandsForItem.cpp game/behavior/FreeHandsForItem.h
        game/behavior/PathToChair.cpp game/behavior/PathToChair.h
        game/behavior/SatisfyHunger.cpp game/behavior/SatisfyHunger.h
        game/behavior/WanderInGeneral.cpp game/behavior/WanderInGeneral.h
        game/behavior/GeneralWander.cpp game/behavior/GeneralWander.h
        game/behavior/FindDrink.cpp game/behavior/FindDrink.h
        game/behavior/PathToNeedGoal.cpp game/behavior/PathToNeedGoal.h
        game/behavior/LookForDrink.cpp game/behavior/LookForDrink.h
        game/behavior/DrinkItem.cpp game/behavior/DrinkItem.h
        game/behavior/PathToDrink.cpp game/behavior/PathToDrink.h
        game/behavior/PlayerCharacterBehavior.cpp game/behavior/PlayerCharacterBehavior.h
        game/map/vector3.cpp game/map/vector3.h
        game/map/rectangle2.cpp game/map/rectangle2.h
        game/map/MapCell.cpp game/map/MapCell.h
        game/map/Map.cpp game/map/Map.h
        game/map/Region.cpp game/map/Region.h
        game/map/NavGraph.cpp game/map/NavGraph.h
        game/map/NavGraphNode.cpp game/map/NavGraphNode.h
        game/properties/GameDefinition.cpp game/properties/GameDefinition.h
        game/properties/GameSettings.cpp game/properties/GameSettings.h
        game/properties/TerrainSettings.cpp game/properties/TerrainSettings.h
        game/properties/SettlerSettings.cpp game/properties/SettlerSettings.h
        game/properties/RaceDefinition.cpp game/properties/RaceDefinition.h
        game/properties/AttributeDef.cpp game/properties/AttributeDef.h
        game/properties/ItemDefinition.cpp game/properties/ItemDefinition.h
        game/properties/ItemSettings.cpp game/properties/ItemSettings.h
        game/properties/MaterialDef.cpp game/properties/MaterialDef.h
        game/properties/BodyDef.cpp game/properties/BodyDef.h
        game/properties/BodySectionDef.cpp game/properties/BodySectionDef.h
        game/properties/BodyPartDef.cpp game/properties/BodyPartDef.h
        game/properties/ConstructionDef.cpp game/properties/ConstructionDef.h
        game/utils/Randomizer.cpp game/utils/Randomizer.h
        game/utils/math_utils.h game/core/defines.h
        game/GameContainer.cpp game/GameContainer.h
        game/core/String2Enums.cpp
        game/core/String2Enums.h
        game/properties/TerrainSprites.cpp game/properties/TerrainSprites.h
        game/properties/SpriteDef.cpp game/properties/SpriteDef.h
        game/properties/TileDef.cpp game/properties/TileDef.h
        game/behavior/SatisfyBreathing.cpp game/behavior/SatisfyBreathing.h
        game/behavior/PathToDryLand.cpp game/behavior/PathToDryLand.h
        game/behavior/PickupWeapon.cpp game/behavior/PickupWeapon.h
        game/core/Military.cpp game/core/Military.h
        game/properties/FactionDef.cpp game/properties/FactionDef.h
        game/properties/RaceClassDef.cpp game/properties/RaceClassDef.h
        game/core/Weapon.cpp game/core/Weapon.h
        game/properties/WeaponDef.cpp game/properties/WeaponDef.h
        game/core/CharacterSkill.cpp game/core/CharacterSkill.h
        game/properties/AttackDef.cpp game/properties/AttackDef.h
        game/properties/UniformSettings.cpp game/properties/UniformSettings.h
        game/behavior/RunFromTarget.cpp game/behavior/RunFromTarget.h
        game/behavior/PathToEnemy.cpp game/behavior/PathToEnemy.h
        game/behavior/GetInPosition.cpp game/behavior/GetInPosition.h
        game/properties/TargetedAttackDef.cpp game/properties/TargetedAttackDef.h
        game/core/Enums2Str.cpp game/core/Enums2Str.h
        game/core/Attack.cpp game/core/Attack.h
        game/properties/AmmoDef.cpp game/properties/AmmoDef.h
        game/core/StringConstants.h
        game/core/Designation.cpp game/core/Designation.h
        game/core/Stockpile.cpp game/core/Stockpile.h game/core/StockItemJob.cpp game/core/StockItemJob.h game/core/TrainOrder.cpp game/core/TrainOrder.h game/properties/GenderDef.cpp game/properties/GenderDef.h game/properties/CharacterSettings.cpp game/properties/CharacterSettings.h game/properties/DefendDef.cpp game/properties/DefendDef.h game/properties/DamageDef.cpp game/properties/DamageDef.h game/properties/NaturalWeaponDef.cpp game/properties/NaturalWeaponDef.h game/core/Automaton.cpp game/core/Automaton.h game/core/BodyPartItem.cpp game/core/BodyPartItem.h game/behavior/EngageTarget.cpp game/behavior/EngageTarget.h game/behavior/HandleTarget.cpp game/behavior/HandleTarget.h game/behavior/AttackEnemy.cpp game/behavior/AttackEnemy.h game/behavior/PathToLastEnemyLocation.cpp game/behavior/PathToLastEnemyLocation.h game/behavior/TrackEnemy.cpp game/behavior/TrackEnemy.h game/behavior/SenseThreats.cpp game/behavior/SenseThreats.h game/properties/SquadDef.cpp game/properties/SquadDef.h game/behavior/WildAnimalBehavior.cpp game/behavior/WildAnimalBehavior.h game/core/Faction.cpp game/core/Faction.h game/core/CreateWorldOptions.cpp game/core/CreateWorldOptions.h game/core/MilitaryStation.cpp game/core/MilitaryStation.h game/core/TrainingStation.cpp game/core/TrainingStation.h game/core/Ambassador.cpp game/core/Ambassador.h game/core/Envoy.cpp game/core/Envoy.h)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2IMAGE_LIBRARIES}
        ${SDL2MIXER_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        ${OPENGL_glu_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT})