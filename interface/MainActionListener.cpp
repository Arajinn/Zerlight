//
// Created by tatiana on 28.11.18.
//

#include "MainActionListener.h"
#include "InterfaceWidgetFactory.h"
#include "MapWidget.h"
#include "ActionConstants.h"
#include "OptionCheckBox.h"
#include "guichan/sdl/SDLDeleters.h"
#include "guichan/sdl/OpenGLGraphics.h"
#include "guichan/sdl/OpenGLImage.h"
#include "guichan/sdl/OpenGLSDLImageLoader.h"
#include "guichan/sdl/SDLInput.h"
#include "guichan/gui/WidgetFactory.h"
#include "guichan/gui/ZImage.h"
#include "guichan/gui/ZContainer.h"
#include "guichan/gui/Gui.h"
#include "guichan/gui/ImageFont.h"
#include "guichan/gui/ZTextBox.h"
#include "guichan/gui/ZLabel.h"
#include "guichan/gui/ZIcon.h"
#include "guichan/gui/ZProgressBar.h"
#include "guichan/gui/ZDropDown.h"
#include "guichan/gui/ZSlider.h"
#include "guichan/gui/ActionEvent.h"
#include "game/core/defines.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <sstream>
#include <future>

MainActionListener::MainActionListener()
        :gui::ActionListener()
{

}

MainActionListener::~MainActionListener()
{
    cleanGui();
    clearGame();

    SDL_Quit();
}

void MainActionListener::init()
{
    mRotation=0;

    mRunning=true;
    mDisplayMainWindow=true;
    mDisplayGenerateNewWordProgress=false;
    mDisplayWorldMap=false;

    mGlow=0.0;
    mWidth=1024;
    mHeight=768;
    mTime=-1.0f;
    mDeltaTime=0.0f;
    mInit=true;
    mResolutionChange=false;
    mHaveFullscreen=false;

    // Init SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    mWindow = std::shared_ptr<SDL_Window>(SDL_CreateWindow("FPS demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    mWidth, mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE),sdl_interface::DeleteSdlWindow);

    mGLContext=SDL_GL_CreateContext(mWindow.get());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_MASK);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    SDL_GL_SetSwapInterval(1);

    // Create some GLU quadrics
    mQuad1 = std::shared_ptr<GLUquadric>(gluNewQuadric(),sdl_interface::DeleteGLUquadric);
    mQuad2 = std::shared_ptr<GLUquadric>(gluNewQuadric(),sdl_interface::DeleteGLUquadric);
    mMoon = std::shared_ptr<GLUquadric>(gluNewQuadric(),sdl_interface::DeleteGLUquadric);
    mMoonRed = std::shared_ptr<GLUquadric>(gluNewQuadric(),sdl_interface::DeleteGLUquadric);
    mClouds2 = std::shared_ptr<GLUquadric>(gluNewQuadric(),sdl_interface::DeleteGLUquadric);
    gluQuadricNormals(mQuad1.get(), GLU_SMOOTH);
    gluQuadricTexture(mQuad1.get(),GL_TRUE);
    gluQuadricNormals(mQuad2.get(), GLU_SMOOTH);
    gluQuadricTexture(mQuad2.get(),GL_TRUE);
    gluQuadricNormals(mMoon.get(), GLU_SMOOTH);
    gluQuadricTexture(mMoon.get(),GL_TRUE);
    gluQuadricNormals(mMoonRed.get(), GLU_SMOOTH);
    gluQuadricTexture(mMoonRed.get(),GL_TRUE);
    gluQuadricNormals(mClouds2.get(), GLU_SMOOTH);
    gluQuadricTexture(mClouds2.get(),GL_TRUE);

    initOpenGL();
    initGui();
    resize();

    initGame();

    mInit = false;
}

void MainActionListener::initGui()
{
    mOpenGLImageLoader = std::shared_ptr<sdl_interface::OpenGLSDLImageLoader>(new sdl_interface::OpenGLSDLImageLoader());
    gui::ZImage::setImageLoader(mOpenGLImageLoader);
    mOpenGLGraphics = std::shared_ptr<sdl_interface::OpenGLGraphics>(new sdl_interface::OpenGLGraphics());
    mOpenGLGraphics->setTargetPlane(mWidth, mHeight);
    mSDLInput = std::shared_ptr<sdl_interface::SDLInput>(new sdl_interface::SDLInput());

    mTop = gui::WidgetFactory::create_zcontainer();
    mTop->setOpaque(false);
    mGui = std::shared_ptr<gui::Gui>(new gui::Gui());
    mGui->setTabbingEnabled(false);
    mGui->setGraphics(mOpenGLGraphics);
    mGui->setInput(mSDLInput);

    mFont = std::shared_ptr<gui::ImageFont>(new gui::ImageFont(          "../images/interface/techyfontbig2.png",        " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\""));
    mHighLightFont = std::shared_ptr<gui::ImageFont>(new gui::ImageFont( "../images/interface/techyfontbighighlight.png"," abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\""));
    mSmallBlackFont = std::shared_ptr<gui::ImageFont>(new gui::ImageFont("../images/interface/techyfontblack.png",       " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\""));
    mWhiteFont = std::shared_ptr<gui::ImageFont>(new gui::ImageFont(     "../images/interface/techyfontwhite.png",       " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\""));
    gui::ZWidget::setGlobalFont(mWhiteFont);

    mTitleLabel = gui::WidgetFactory::create_ztextbox("Gnomoria");
    mTitleLabel->setFont(mHighLightFont);
    mTitleLabel->setOpaque(false);
    mTitleLabel->setEditable(false);
    mTitleLabel->setFocusable(false);
    mTop->add(mTitleLabel);

    mDemoInfo = gui::WidgetFactory::create_ztextbox("Copyright 2018 (c) Aprilis.");
    mDemoInfo->setFont(mSmallBlackFont);
    mDemoInfo->setOpaque(false);
    mDemoInfo->setEditable(false);
    mDemoInfo->setFocusable(false);
    mDemoInfo->setFrameSize(0);
    mTop->add(mDemoInfo);

    mVersionLabel = gui::WidgetFactory::create_zlabel("Version 0.01");
    mVersionLabel->setFont(mSmallBlackFont);
    mTop->add(mVersionLabel);

    mBoxImage = gui::ZImage::load("../images/interface/box.png");

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

    loadTextures();
    initMain();
    initNewGame();
    initLoadGame();
    initOptions();
    initGenerateNewWorld();
    initWorldMap();
}

void MainActionListener::cleanGui()
{
    cleanTextures();
    cleanMain();
    cleanNewGame();
    cleanLoadGame();
    cleanOptions();
    cleanGenerateNewWorld();
    cleanWorldMap();

    mBoxImage = nullptr;
    mFont = nullptr;
    mHighLightFont = nullptr;
    mSmallBlackFont = nullptr;
    mWhiteFont = nullptr;
    mTop = nullptr;
    mGui = nullptr;
    mSplashIcon = nullptr;
    mSplashImage = nullptr;
    mSDLInput = nullptr;
    mOpenGLGraphics = nullptr;
    mTitleLabel = nullptr;
}

void MainActionListener::resizeGui()
{
    mTitleLabel->setPosition(mWidth/2 - 100, mHeight/2 - 230);

    mTop->setDimension(gui::Rectangle(0, 0, mWidth, mHeight));
}

void MainActionListener::loadTextures()
{
    std::shared_ptr<sdl_interface::OpenGLImage> image;

    mCloudImage = gui::ZImage::load("../images/interface/cloudsblackwhite.png");
    image = std::dynamic_pointer_cast<sdl_interface::OpenGLImage>(mCloudImage);
    mCloudTexture = image->getTextureHandle();

    mPlanetImage = gui::ZImage::load("../images/interface/planet.png");
    image = std::dynamic_pointer_cast<sdl_interface::OpenGLImage>(mPlanetImage);
    mPlanetTexture = image->getTextureHandle();

    mStarsImage = gui::ZImage::load("../images/interface/background.png");
    image = std::dynamic_pointer_cast<sdl_interface::OpenGLImage>(mStarsImage);
    mStarsTexture = image->getTextureHandle();

    mMoonImage = gui::ZImage::load("../images/interface/moon.png");
    image = std::dynamic_pointer_cast<sdl_interface::OpenGLImage>(mMoonImage);
    mMoonTexture = image->getTextureHandle();

    mMoonRedImage = gui::ZImage::load("../images/interface/moonred.png");
    image = std::dynamic_pointer_cast<sdl_interface::OpenGLImage>(mMoonRedImage);
    mMoonRedTexture = image->getTextureHandle();
}

void MainActionListener::cleanTextures()
{
    mPlanetImage = nullptr;
    mCloudImage = nullptr;
    mStarsImage = nullptr;
    mMoonImage = nullptr;
    mMoonRedImage = nullptr;
}

void MainActionListener::initOpenGL()
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

    // Init Light
    Light2Ambient[0] =  0.5f;
    Light2Ambient[1]	= 0.4f;
    Light2Ambient[2]	= 0.7f;
    Light2Ambient[3]	= 1.0f;

    Light2Diffuse[0] = 1.0f;
    Light2Diffuse[1] = 1.0f;
    Light2Diffuse[2] = 1.0f;
    Light2Diffuse[3] =	1.0f;

    Light2Position[0] =  10.0f;
    Light2Position[1] =	 .5f;
    Light2Position[2] =  0.0f;
    Light2Position[3] =  1.0f;

    Light2SpotDirection[0] = -1.0;
    Light2SpotDirection[1] = 0.0;
    Light2SpotDirection[2] = 0.0;

    glLightfv(GL_LIGHT2, GL_POSITION, Light2Position);
    glLightfv(GL_LIGHT2, GL_AMBIENT, Light2Ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, Light2Diffuse);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, Light2SpotDirection);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0);

    glEnable(GL_LIGHT2);
}

void MainActionListener::resize()
{
    resizeGui();
    resizeMain();
    resizeNewGame();
    resizeLoadGame();
    resizeOptions();

    resizeGenerateNewWorld();

    resizeWorldMap();

    //const float ar = (float)mWidth / (float)mHeight; //The width-to-height ratio
    //glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
}

void MainActionListener::initGenerateNewWorld()
{
    mGenerateNewWorld = gui::WidgetFactory::create_zcontainer();
    mGenerateNewWorld->setVisible(false);
    mGenerateNewWorld->setOpaque(true);
    mGenerateNewWorld->setBaseColor(gui::ZColor::COLOR_DARK_GRAY);

    mGenerateNewWorldLabel = gui::WidgetFactory::create_zlabel("Generate New World");
    mGenerateNewWorldLabel->setFont(mWhiteFont);
    mGenerateNewWorldLabel->adjustSize();
    mGenerateNewWorld->add(mGenerateNewWorldLabel);

    mGenerateNewWorldProgress = gui::WidgetFactory::create_zprogressbar();
    mGenerateNewWorldProgress->setFont(mWhiteFont);
    mGenerateNewWorldProgress->setBaseColor(gui::ZColor::COLOR_LIGHT_GRAY);
    mGenerateNewWorldProgress->setWidth(400);
    mGenerateNewWorldProgress->setHeight(40);
    mGenerateNewWorld->add(mGenerateNewWorldProgress);
}

void MainActionListener::cleanGenerateNewWorld()
{
    mGenerateNewWorld=nullptr;
    mGenerateNewWorldLabel=nullptr;
    mGenerateNewWorldProgress=nullptr;
}

void MainActionListener::resizeGenerateNewWorld()
{
    mGenerateNewWorld->setDimension(gui::Rectangle(0, 0, mWidth, mHeight));
    mGenerateNewWorldLabel->setPosition(mWidth/2 - 100, mHeight/2 - 50);
    mGenerateNewWorldProgress->setPosition(mWidth/2 - 200, mHeight/2 - 20);
}

void MainActionListener::initWorldMap()
{
    mWorldMapContainer = InterfaceWidgetFactory::create_mapwidget();
    mWorldMapContainer->setVisible(false);
    mWorldMapContainer->setOpaque(true);
    mWorldMapContainer->setFocusable(true);
    mWorldMapContainer->setBaseColor(gui::ZColor::COLOR_DARK_GRAY);
}

void MainActionListener::cleanWorldMap()
{
    mWorldMapContainer = nullptr;
}

void MainActionListener::resizeWorldMap()
{
    mWorldMapContainer->setDimension(gui::Rectangle(0, 0, mWidth, mHeight));
}

void MainActionListener::initVideo()
{
    if (mResolution->getSelected() == 0)
    {
        mWidth = 1024;
        mHeight = 768;
        mResolutionChange = true;
    }
    else if (mResolution->getSelected() == 1)
    {
        mWidth = 800;
        mHeight = 600;
        mResolutionChange = true;
    }
    if (mFullScreen->isSelected())
    {
        mHaveFullscreen = true;
        SDL_SetWindowSize(mWindow.get(),mWidth, mHeight);
    }
    else
    {
        mHaveFullscreen = false;
        SDL_SetWindowSize(mWindow.get(),mWidth, mHeight);
    }
    mOpenGLGraphics->setTargetPlane(mWidth, mHeight);
    initOpenGL();
}

void MainActionListener::action(const gui::ActionEvent& actionEvent)
{
    const auto action_id=actionEvent.getId();

    if (action_id == action::QUIT_ACTION)
    {
        mRunning = false;
    }
    else if (action_id == action::NEW_GAME_ACTION)
    {
        mMain->setVisible(false);
        mNewGame->setVisible(true);
    }
    else if (action_id == action::LOAD_GAME_ACTION)
    {
        mMain->setVisible(false);
        mLoadGame->setVisible(true);
    }
    else if (action_id == action::OPTIONS_ACTION)
    {
        mMain->setVisible(false);
        mOptions->setVisible(true);
    }
    else if (action_id == action::BACK_TO_MAIN_WINDOW_ACTION)
    {
        mMain->setVisible(true);
        mNewGame->setVisible(false);
        mLoadGame->setVisible(false);
        mOptions->setVisible(false);
        mDisplayMainWindow=true;
        mDisplayGenerateNewWordProgress=false;
        mDisplayWorldMap=false;

        if (mGui->getTop()!=mTop)
            mGui->setTop(mTop);
    }
    else if (action_id == action::FULLSCREEN_ACTION)
    {
        initVideo();
    }
    else if (action_id == action::RESOLUTION_ACTION)
    {
        initVideo();
    }
    else if (action_id == action::VOLUME_ACTION)
    {
        std::string str;
        std::ostringstream os(str);
        os << (int)(mVolume->getValue() * 100) << "%";
        mVolumePercent->setCaption(os.str());
        mVolumePercent->adjustSize();
        double m = MIX_MAX_VOLUME;
        double p = mVolume->getValue();
        Mix_Volume(-1,(int)(m*p));
    }
    else if (action_id == action::GENERATE_NEW_WORLD_ACTION)
    {
        std::cout << "generate new world start" << std::endl;
        mDisplayMainWindow=false;
        mDisplayGenerateNewWordProgress=true;

        mTop->setVisible(false);
        mGenerateNewWorld->setVisible(true);
        mProgress=0.0f;

        if (mGui->getTop()!=mGenerateNewWorld)
            mGui->setTop(mGenerateNewWorld);

        std::packaged_task<bool()> task(std::bind(&game::GameManager::generateNewWorld,GMINSTANCE.get()));
        std::future<bool> result = task.get_future();

        mGameThread = std::thread(std::move(task));
        //bool ret=result.get();
        mGameThread.detach();
    }
}

void MainActionListener::run()
{
    runIntro();
    runMain();
}

void MainActionListener::runIntro()
{
    while(SDL_GetTicks() < 3000)
    {
        if (mTime < 0)
        {
            mTime = SDL_GetTicks();
        }
        mDeltaTime = SDL_GetTicks() - mTime;
        mTime = SDL_GetTicks();

        input();

        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mGui->draw();
        SDL_GL_SwapWindow(mWindow.get());
        SDL_Delay(10);
    }

    mGui->setTop(mTop);
}

void MainActionListener::input()
{
    while(SDL_PollEvent(&mEvent))
    {
        if (mEvent.type == SDL_KEYDOWN)
        {
            if (mEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                mMain->setVisible(true);
                mNewGame->setVisible(false);
                mLoadGame->setVisible(false);
                mOptions->setVisible(false);
            }

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

void MainActionListener::runMain()
{
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

        input();
        mGui->logic();

        if (mDisplayMainWindow)
        {
            displayMainWindow();
        }
        else if (mDisplayGenerateNewWordProgress)
        {
            displayGenerateNewWordProgress();
        }
        else if (mDisplayWorldMap)
        {
            //displayWorldMap();
            displayWorldMapContainer();
        }
    }
}

void MainActionListener::displayMainWindow()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    drawSpace();
    gluLookAt(0.0, -1.8, -2.9,
              0.0, -1.2, 1.0,
              0.0, -1.0, 0.0);

    drawBackground();
    glPushMatrix();
    mGui->draw();
    glPopMatrix();

    SDL_GL_SwapWindow(mWindow.get());
    SDL_Delay(10);
}

void MainActionListener::drawSpace()
{
    int y = -200;
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, mWidth, mHeight, 0.0, 1, 0.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mStarsTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glScalef(2,1.5,1);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3i(0,y,0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3i(mStarsImage->getWidth(),y,0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3i(mStarsImage->getWidth(),mStarsImage->getHeight()+y,0);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3i(0,mStarsImage->getHeight()+y,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void MainActionListener::drawBackground()
{
    glEnable(GL_LIGHTING);
    mRotation+= mDeltaTime / 2000;

    glPushMatrix();
    glTranslatef(0.0,0.0,1.0);
    glRotatef(80,0.0, 0.0, 1.0);
    glRotatef(mRotation*7,0.0, -1.0, 0.0);
    glTranslatef(0,0.0,2.1);
    glRotatef(mRotation*50,1.0, 0.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mMoonTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluSphere(mMoon.get(), .07, 10, 10);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0,0.0,1.0);
    glRotatef(110,0.0, 1.0, 0.0);
    glRotatef(mRotation*5,0.0, 0.0, 1.0);
    glTranslatef(2.2,0.0,0.0);
    glRotatef(mRotation*30,1.0, 0.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mMoonRedTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluSphere(mMoonRed.get(), 0.15, 10, 10);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0,0.0,1.0);
    glRotatef(mRotation*3,1.0, 0.0, 0.0);
    glRotatef(77,0.0, 1.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mPlanetTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluSphere(mQuad1.get(), 1.93, 60, 60);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE,GL_ONE);
    glTranslatef(0.0,0.0,1.0);
    glRotatef(mRotation*6,1.0, 0.0, 0.0);
    glRotatef(90,0.0, 1.0, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mCloudTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluSphere(mQuad2.get(), 2.0, 60, 60);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE,GL_ONE);
    glTranslatef(0.0,0.0,1.0);
    glRotatef(mRotation*5,1.0, 0.0, 0.0);
    glRotatef(90,0.0, 1.0, 0.0);
    glRotatef(90,0.0, 0.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mCloudTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gluSphere(mQuad2.get(), 1.98, 60, 60);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glDisable(GL_LIGHTING);
}

void MainActionListener::displayGenerateNewWordProgress()
{
    //std::cout << "generate new world " << mDeltaTime << std::endl;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mProgress = GMINSTANCE->getInitProgress();
    mGenerateNewWorldProgress->setProgress(mProgress);

    mGui->draw();

    SDL_GL_SwapWindow(mWindow.get());

    if (GMINSTANCE->getIsGenerateFinish())
    {
        std::cout << "generate new world finish" << std::endl;

        mDisplayMainWindow=false;
        mDisplayGenerateNewWordProgress=false;
        mDisplayWorldMap=true;

        mGenerateNewWorld->setVisible(false);
        mWorldMapContainer->setVisible(true);

        if (mGui->getTop()!=mWorldMapContainer)
            mGui->setTop(mWorldMapContainer);
    }

    SDL_Delay(10);
}

void MainActionListener::displayWorldMapContainer()
{
    std::cout << "display new world " << mDeltaTime << std::endl;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mGui->draw();

    SDL_GL_SwapWindow(mWindow.get());

    if (mDeltaTime==0)
        SDL_Delay(100);
}