#include "interface/GameActionListener.h"
#include "game/GameContainer.h"
#include "game/core/GameManager.h"
#include "guichan/gui/Exception.h"
#include <iostream>

int main()
{
    try
    {
        std::shared_ptr<interface::GameActionListener> fpsDemo=std::make_shared<interface::GameActionListener>();
        fpsDemo->init();

        std::shared_ptr<game::GameContainer> gameContainer=std::make_shared<game::GameContainer>();
        gameContainer->gameManager->previewSpawn();

        fpsDemo->run();
    }
    catch (gui::Exception e)
    {
        std::cout << e.getMessage() << std::endl;
        return 1;
    }
    catch (std::exception e)
    {
        std::cout << "Std exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cout << "Unknown exception" << std::endl;
        return 1;
    }

    return 0;
}