#include "interface/MainActionListener.h"
#include "guichan/gui/Exception.h"
#include <iostream>

int main()
{
    try
    {
        std::shared_ptr<MainActionListener> fpsDemo=std::shared_ptr<MainActionListener>(new MainActionListener());
        fpsDemo->init();
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