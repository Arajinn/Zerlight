//
// Created by tatiana on 28.11.18.
//

#ifndef ZERLIGHT_MAINACTIONLISTENER_H
#define ZERLIGHT_MAINACTIONLISTENER_H

#include "guichan/gui/ActionListener.h"

#include <SDL2/SDL_events.h>

#include <GL/gl.h>

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
    class ZContainer;
    class ZImage;
    class ImageFont;
    class ZIcon;
    class ZLabel;
    class ZTextBox;
    class ZDropDown;
    class ZScrollArea;
    class ZListBox;
    class ZSlider;
    class ZButton;
    class ZProgressBar;
    class ZColor;
}

class OptionButton;
class OptionCheckBox;
class ResolutionListModel;
class MapWidget;

class MainActionListener : public gui::ActionListener {
public:
    MainActionListener();
    ~MainActionListener();

    void init();

    void run();
    void action(const gui::ActionEvent& actionEvent);

private:
    float mRotation;

    bool mRunning;
    bool mDisplayMainWindow;
    bool mDisplayGenerateNewWordProgress;
    bool mDisplayWorldMap;

    bool mInit;
    bool mResolutionChange;
    bool mHaveFullscreen;

    double mGlow;

    int mWidth,mHeight;

    float mTime, mDeltaTime;

    std::shared_ptr<SDL_Window> mWindow;
    SDL_GLContext mGLContext;
    SDL_Event mEvent;

    std::shared_ptr<GLUquadric> mQuad1;
    std::shared_ptr<GLUquadric> mQuad2;
    std::shared_ptr<GLUquadric> mClouds2;
    std::shared_ptr<GLUquadric> mMoon;
    std::shared_ptr<GLUquadric> mMoonRed;

    void initGui();
    void cleanGui();
    void resizeGui();

    std::shared_ptr<sdl_interface::OpenGLGraphics> mOpenGLGraphics;
    std::shared_ptr<sdl_interface::SDLInput> mSDLInput;
    std::shared_ptr<sdl_interface::OpenGLSDLImageLoader> mOpenGLImageLoader;

    std::shared_ptr<gui::Gui> mGui;

    std::shared_ptr<gui::ZContainer> mTop;

    std::shared_ptr<gui::ImageFont> mFont;
    std::shared_ptr<gui::ImageFont> mSmallBlackFont;
    std::shared_ptr<gui::ImageFont> mWhiteFont;
    std::shared_ptr<gui::ImageFont> mHighLightFont;

    std::shared_ptr<gui::ZTextBox> mTitleLabel;
    std::shared_ptr<gui::ZTextBox> mDemoInfo;
    std::shared_ptr<gui::ZLabel> mVersionLabel;

    std::shared_ptr<gui::ZImage> mBoxImage;

    std::shared_ptr<gui::ZImage> mSplashImage;
    std::shared_ptr<gui::ZIcon> mSplashIcon;

    void loadTextures();
    void cleanTextures();

    std::shared_ptr<gui::ZImage> mCloudImage;
    std::shared_ptr<gui::ZImage> mPlanetImage;
    std::shared_ptr<gui::ZImage> mStarsImage;
    std::shared_ptr<gui::ZImage> mMoonImage;
    std::shared_ptr<gui::ZImage> mMoonRedImage;

    GLuint mPlanetTexture;
    GLuint mCloudTexture;
    GLuint mStarsTexture;
    GLuint mMoonTexture;
    GLuint mMoonRedTexture;

    void initOpenGL();

    GLfloat Light2Ambient[4];
    GLfloat Light2Diffuse[4];
    GLfloat Light2Position[4];
    GLfloat Light2SpotDirection[3];

    void resize();

    void initMain();
    void cleanMain();
    void resizeMain();

    std::shared_ptr<gui::ZContainer> mMain;

    std::shared_ptr<OptionButton> mNewGameButton;
    std::shared_ptr<OptionButton> mLoadGameButton;
    std::shared_ptr<OptionButton> mOptionsButton;
    std::shared_ptr<OptionButton> mQuitButton;

    void initNewGame();
    void cleanNewGame();
    void resizeNewGame();

    // new game container
    std::shared_ptr<gui::ZContainer> mNewGame;
    std::shared_ptr<gui::ZIcon> mNewGameBoxIcon;
    std::shared_ptr<gui::ZLabel> mNewGameLabel;
    std::shared_ptr<gui::ZTextBox> mNewGameText;
    std::shared_ptr<gui::ZButton> mNewGameGenerateButton;
    std::shared_ptr<OptionButton> mNewGameBackButton;

    void initLoadGame();
    void cleanLoadGame();
    void resizeLoadGame();

    std::shared_ptr<gui::ZContainer> mLoadGame;
    std::shared_ptr<gui::ZIcon> mLoadGameBoxIcon;
    std::shared_ptr<gui::ZLabel> mLoadGameLabel;
    std::shared_ptr<gui::ZTextBox> mLoadGameText;
    std::shared_ptr<OptionButton> mLoadGameBackButton;

    void initOptions();
    void cleanOptions();
    void resizeOptions();

    std::shared_ptr<gui::ZContainer> mOptions;
    std::shared_ptr<gui::ZIcon> mOptionsBoxIcon;
    std::shared_ptr<gui::ZLabel> mOptionsLabel;
    std::shared_ptr<OptionCheckBox> mFullScreen;
    std::shared_ptr<gui::ZScrollArea> mResolutionScrollArea;
    std::shared_ptr<gui::ZListBox> mResolutionListBox;
    std::shared_ptr<ResolutionListModel> mResolutionListModel;
    std::shared_ptr<gui::ZDropDown> mResolution;
    std::shared_ptr<gui::ZSlider> mVolume;
    std::shared_ptr<gui::ZLabel> mVolumePercent;
    std::shared_ptr<gui::ZLabel> mVolumeLabel;
    std::shared_ptr<gui::ZLabel> mResolutionLabel;
    std::shared_ptr<OptionButton> mOptionsBackButton;

    void initGenerateNewWorld();
    void cleanGenerateNewWorld();
    void resizeGenerateNewWorld();

    std::shared_ptr<gui::ZContainer> mGenerateNewWorld;
    std::shared_ptr<gui::ZLabel> mGenerateNewWorldLabel;
    std::shared_ptr<gui::ZProgressBar> mGenerateNewWorldProgress;

    void initWorldMap();
    void cleanWorldMap();
    void resizeWorldMap();

    std::shared_ptr<MapWidget> mWorldMapContainer;

    void initGame();
    void clearGame();
};


#endif //ZERLIGHT_MAINACTIONLISTENER_H
