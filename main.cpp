#include "interface/MainActionListener.h"
#include "guichan/gui/Exception.h"
#include "game/GameContainer.h"
#include "guichan/sdl/OpenGLSDLImageLoader.h"
#include "guichan/gui/ZImage.h"
#include <iostream>

int main()
{
    try
    {
        std::shared_ptr<MainActionListener> fpsDemo=std::make_shared<MainActionListener>();
        fpsDemo->init();

        std::shared_ptr<game::GameContainer> gameContainer=std::make_shared<game::GameContainer>();

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