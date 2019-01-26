//
// Created by tatiana on 23.01.19.
//

#include "GameActionListener.h"
#include "ActionConstants.h"
#include "InterfaceWidgetFactory.h"
#include "MapWidget.h"
#include "guichan/sdl/SDLDeleters.h"
#include "guichan/sdl/OpenGLGraphics.h"
#include "guichan/sdl/OpenGLSDLImageLoader.h"
#include "guichan/sdl/SDLInput.h"
#include "guichan/gui/WidgetFactory.h"
#include "guichan/gui/Gui.h"
#include "guichan/gui/ZImage.h"
#include "guichan/gui/ImageFont.h"
#include "guichan/gui/ZIcon.h"
#include "guichan/gui/ActionEvent.h"
#include "game/core/defines.h"

#include <SDL.h>
#include <GL/glu.h>

namespace interface
{
    GameActionListener::GameActionListener()
    {
        init();
    }

    GameActionListener::~GameActionListener()
    {
        cleanGui();

        SDL_Quit();
    }

    void GameActionListener::init()
    {
        mRunning=true;
        mInit=true;
        mResolutionChange=false;
        mHaveFullscreen=false;

        mTime=-1.0f;
        mDeltaTime=0.0f;

        mWidth=1024;
        mHeight=768;

        // Init SDL
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        mWindow = std::shared_ptr<SDL_Window>(SDL_CreateWindow("FPS demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                mWidth, mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE),sdl_interface::DeleteSdlWindow);

        mGLContext=SDL_GL_CreateContext(mWindow.get());

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_MASK);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

        SDL_GL_SetSwapInterval(1);

        initOpenGL();

        initGui();

        resize();

        mInit = false;
    }

    void GameActionListener::initOpenGL()
    {
        // Init OpenGL
        glViewport(0, 0, mWidth, mHeight);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        glShadeModel(GL_SMOOTH);
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(50.0, mWidth/mHeight, 1.0, 10.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_LIGHT2);
    }

    void GameActionListener::initGui()
    {
        mOpenGLImageLoader = std::make_shared<sdl_interface::OpenGLSDLImageLoader>();
        gui::ZImage::setImageLoader(mOpenGLImageLoader);
        mOpenGLGraphics = std::make_shared<sdl_interface::OpenGLGraphics>();
        mOpenGLGraphics->setTargetPlane(mWidth, mHeight);
        mSDLInput = std::make_shared<sdl_interface::SDLInput>();

        mTop = InterfaceWidgetFactory::create_mapwidget();
        mTop->setOpaque(true);
        mGui = std::make_shared<gui::Gui>();
        mGui->setTabbingEnabled(false);
        mGui->setGraphics(mOpenGLGraphics);
        mGui->setInput(mSDLInput);

        mSplashImage = gui::ZImage::load("../images/interface/splash.png");
        mSplashIcon = gui::WidgetFactory::create_zicon(mSplashImage);

        if (mInit)
        {
            mGui->setTop(mSplashIcon);
            mSplashIcon->setPosition(mWidth / 2 -mSplashImage->getWidth() /2, mHeight / 2 -mSplashImage->getHeight() /2);
        }
        else
        {
            mGui->setTop(mTop);
        }

        //initMain();
    }

    void GameActionListener::cleanGui()
    {
        //cleanMain();

        mTop = nullptr;
        mGui = nullptr;
        mSplashImage = nullptr;
        mSplashIcon->shutdown();
        mSplashIcon = nullptr;
        mSDLInput = nullptr;
        mOpenGLGraphics = nullptr;
    }

    void GameActionListener::resize()
    {
        mTop->setDimension(gui::Rectangle(0, 0, mWidth, mHeight));
        mTop->resize();
    }

    void GameActionListener::input()
    {
        while(SDL_PollEvent(&mEvent))
        {
            if (mEvent.type == SDL_KEYDOWN)
            {
                mSDLInput->pushInput(mEvent);
            }
            else if (mEvent.type == SDL_KEYUP)
            {
                mSDLInput->pushInput(mEvent);
            }
            else if (mEvent.type == SDL_QUIT)
            {
                mRunning = false;
            }
                // We ignore keyboard input and just sends mouse input to Guichan
            else if (mEvent.type == SDL_MOUSEMOTION
                     || mEvent.type == SDL_MOUSEBUTTONDOWN
                     || mEvent.type == SDL_MOUSEBUTTONUP)
            {
                mSDLInput->pushInput(mEvent);
            }
            else if (mEvent.type == SDL_MOUSEWHEEL)
            {
                mSDLInput->pushInput(mEvent);
            }
        }
    }

    void GameActionListener::run()
    {
        mGui->setTop(mTop);

        while (mRunning)
        {
            if (mResolutionChange)
            {
                // Clear the screen before remaking the Gui
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                SDL_GL_SwapWindow(mWindow.get());

                cleanGui();
                initGui();
                resize();
                mResolutionChange = false;
            }

            if (mTime < 0)
            {
                mTime = SDL_GetTicks();
            }

            mDeltaTime = SDL_GetTicks() - mTime;
            mTime = SDL_GetTicks();

            if (GMINSTANCE->currentMode()==game::GameManager::CurrentMode::StandartWorld)
                GMINSTANCE->update(0.1f);

            input();
            mGui->logic();

            display();
        }
    }

    void GameActionListener::display()
    {
        std::cout << "display world " << mDeltaTime << std::endl;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mGui->draw();

        SDL_GL_SwapWindow(mWindow.get());

        const int delta=200-int(mDeltaTime);
        if (delta>50)
            SDL_Delay(delta);
    }

    void GameActionListener::action(const gui::ActionEvent& actionEvent)
    {
        const auto action_id=actionEvent.getId();

        if (action_id == action::QUIT_ACTION)
        {
            mRunning = false;
        }
//        else if (action_id == action::NEW_GAME_ACTION)
//        {
//            mMain->setVisible(false);
//            mNewGame->setVisible(true);
//        }
//        else if (action_id == action::LOAD_GAME_ACTION)
//        {
//            mMain->setVisible(false);
//            mLoadGame->setVisible(true);
//        }
//        else if (action_id == action::OPTIONS_ACTION)
//        {
//            mMain->setVisible(false);
//            mOptions->setVisible(true);
//        }
//        else if (action_id == action::BACK_TO_MAIN_WINDOW_ACTION)
//        {
//            mMain->setVisible(true);
//            mNewGame->setVisible(false);
//            mLoadGame->setVisible(false);
//            mOptions->setVisible(false);
//            mDisplayMainWindow=true;
//            mDisplayGenerateNewWordProgress=false;
//            mDisplayWorldMap=false;
//
//            if (mGui->getTop()!=mTop)
//                mGui->setTop(mTop);
//        }
//        else if (action_id == action::FULLSCREEN_ACTION)
//        {
//            initVideo();
//        }
//        else if (action_id == action::RESOLUTION_ACTION)
//        {
//            initVideo();
//        }
//        else if (action_id == action::VOLUME_ACTION)
//        {
//            std::string str;
//            std::ostringstream os(str);
//            os << (int)(mVolume->getValue() * 100) << "%";
//            mVolumePercent->setCaption(os.str());
//            mVolumePercent->adjustSize();
//            double m = MIX_MAX_VOLUME;
//            double p = mVolume->getValue();
//            Mix_Volume(-1,(int)(m*p));
//        }
//        else if (action_id == action::GENERATE_NEW_WORLD_ACTION)
//        {
//            std::cout << "generate new world start" << std::endl;
//            mDisplayMainWindow=false;
//            mDisplayGenerateNewWordProgress=true;
//
//            mTop->setVisible(false);
//            mGenerateNewWorld->setVisible(true);
//            mProgress=0.0f;
//
//            if (mGui->getTop()!=mGenerateNewWorld)
//                mGui->setTop(mGenerateNewWorld);
//
//            std::packaged_task<bool()> task(std::bind(&game::GameManager::generateNewWorld,GMINSTANCE.get()));
//            std::future<bool> result = task.get_future();
//
//            mGameThread = std::thread(std::move(task));
//            //bool ret=result.get();
//            mGameThread.detach();
//        }
    }
}