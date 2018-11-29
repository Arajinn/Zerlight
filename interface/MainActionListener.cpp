//
// Created by tatiana on 28.11.18.
//

#include "MainActionListener.h"
#include "InterfaceWidgetFactory.h"
#include "MapWidget.h"
#include "guichan/sdl/SDLDeleters.h"
#include "guichan/sdl/OpenGLSDLImageLoader.h"
#include "guichan/sdl/OpenGLGraphics.h"
#include "guichan/sdl/OpenGLImage.h"
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

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#include <GL/gl.h>
#include <GL/glu.h>

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
    mWidth=800;
    mHeight=600;
    mTime=-1.0f;
    mDeltaTime=0.0f;
    mInit=true;
    mResolutionChange=false;
    mHaveFullscreen=false;

    // Init SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    //mScreen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_OPENGL | SDL_HWACCEL);

    mWindow = std::shared_ptr<SDL_Window>(SDL_CreateWindow("FPS demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE),sdl_interface::DeleteSdlWindow);

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

    mFont = std::shared_ptr<gui::ImageFont>(new gui::ImageFont(          "../images/techyfontbig2.png",        " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\""));
    mHighLightFont = std::shared_ptr<gui::ImageFont>(new gui::ImageFont( "../images/techyfontbighighlight.png"," abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\""));
    mSmallBlackFont = std::shared_ptr<gui::ImageFont>(new gui::ImageFont("../images/techyfontblack.png",       " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\""));
    mWhiteFont = std::shared_ptr<gui::ImageFont>(new gui::ImageFont(     "../images/techyfontwhite.png",       " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\""));
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

    mBoxImage = gui::ZImage::load("../images/box.png");

    mSplashImage = gui::ZImage::load("../images/splash.png");
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
    mOpenGLImageLoader = nullptr;
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

    mCloudImage = gui::ZImage::load("../images/cloudsblackwhite.png");
    image = std::dynamic_pointer_cast<sdl_interface::OpenGLImage>(mCloudImage);
    mCloudTexture = image->getTextureHandle();

    mPlanetImage = gui::ZImage::load("../images/planet.png");
    image = std::dynamic_pointer_cast<sdl_interface::OpenGLImage>(mPlanetImage);
    mPlanetTexture = image->getTextureHandle();

    mStarsImage = gui::ZImage::load("../images/background.png");
    image = std::dynamic_pointer_cast<sdl_interface::OpenGLImage>(mStarsImage);
    mStarsTexture = image->getTextureHandle();

    mMoonImage = gui::ZImage::load("../images/moon.png");
    image = std::dynamic_pointer_cast<sdl_interface::OpenGLImage>(mMoonImage);
    mMoonTexture = image->getTextureHandle();

    mMoonRedImage = gui::ZImage::load("../images/moonred.png");
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