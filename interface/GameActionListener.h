//
// Created by tatiana on 23.01.19.
//

#ifndef ZERLIGHT_GAMEACTIONLISTENER_H
#define ZERLIGHT_GAMEACTIONLISTENER_H

#include "guichan/gui/ActionListener.h"

#include <SDL2/SDL_events.h>

#include <GL/gl.h>

#include <thread>

class GLUquadric;

namespace sdl_interface
{
    class OpenGLGraphics;
    class SDLInput;
    class OpenGLSDLImageLoader;
}

namespace gui
{
    class Gui;
    class ZImage;
    class ZIcon;
}

class MapWidget;

namespace interface {
    class GameActionListener : public gui::ActionListener {
    public:
        GameActionListener();
        ~GameActionListener();

        void init();
        void run();

        void action(const gui::ActionEvent& actionEvent);
    private:
        bool mRunning, mInit;
        bool mResolutionChange, mHaveFullscreen;

        std::shared_ptr<SDL_Window> mWindow;
        SDL_GLContext mGLContext;
        SDL_Event mEvent;

        std::shared_ptr<sdl_interface::OpenGLGraphics> mOpenGLGraphics;
        std::shared_ptr<sdl_interface::SDLInput> mSDLInput;
        std::shared_ptr<sdl_interface::OpenGLSDLImageLoader> mOpenGLImageLoader;

        std::shared_ptr<gui::Gui> mGui;

        std::shared_ptr<gui::ZImage> mSplashImage;
        std::shared_ptr<gui::ZIcon> mSplashIcon;

        std::shared_ptr<MapWidget> mTop;

        int mWidth,mHeight;

        float mTime, mDeltaTime;

        void initOpenGL();

        void initGui();
        void cleanGui();

        void resize();

        void input();

        void display();
    };
}

#endif //ZERLIGHT_GAMEACTIONLISTENER_H
