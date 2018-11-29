//
// Created by tatiana on 28.11.18.
//

#include "MainActionListener.h"
#include "ActionConstants.h"
#include "InterfaceWidgetFactory.h"
#include "OptionButton.h"
#include "OptionCheckBox.h"
#include "ResolutionListModel.h"
#include "guichan/gui/WidgetFactory.h"
#include "guichan/gui/ZContainer.h"
#include "guichan/gui/ImageFont.h"
#include "guichan/gui/ZTextBox.h"
#include "guichan/gui/ZLabel.h"
#include "guichan/gui/ZIcon.h"
#include "guichan/gui/ZListBox.h"
#include "guichan/gui/ZDropDown.h"
#include "guichan/gui/ZSlider.h"
#include "guichan/gui/ActionListener.h"

void MainActionListener::initMain()
{
    mMain = gui::WidgetFactory::create_zcontainer();
    mMain->setOpaque(false);
    mTop->add(mMain);

    mNewGameButton = InterfaceWidgetFactory::create_optionbutton("New Game");
    mNewGameButton->setHighLightFont(mHighLightFont);
    mNewGameButton->adjustSize();
    mNewGameButton->setActionEventId(action::NEW_GAME_ACTION);
    mNewGameButton->addActionListener(ActionListener::downcasted_shared_from_this<MainActionListener>());
    mNewGameButton->setFont(mFont);
    mNewGameButton->adjustSize();
    mMain->add(mNewGameButton);

    mLoadGameButton = InterfaceWidgetFactory::create_optionbutton("Load Game");
    mLoadGameButton->setHighLightFont(mHighLightFont);
    mLoadGameButton->adjustSize();
    mLoadGameButton->setActionEventId(action::LOAD_GAME_ACTION);
    mLoadGameButton->addActionListener(ActionListener::downcasted_shared_from_this<MainActionListener>());
    mLoadGameButton->setFont(mFont);
    mLoadGameButton->adjustSize();
    mMain->add(mLoadGameButton);

    mOptionsButton = InterfaceWidgetFactory::create_optionbutton("Options");
    mOptionsButton->setHighLightFont(mHighLightFont);
    mOptionsButton->adjustSize();
    mOptionsButton->setActionEventId(action::OPTIONS_ACTION);
    mOptionsButton->addActionListener(ActionListener::downcasted_shared_from_this<MainActionListener>());
    mOptionsButton->setFont(mFont);
    mOptionsButton->adjustSize();
    mMain->add(mOptionsButton);

    mQuitButton = InterfaceWidgetFactory::create_optionbutton("Quit");
    mQuitButton->setHighLightFont(mHighLightFont);
    mQuitButton->adjustSize();
    mQuitButton->setActionEventId(action::QUIT_ACTION);
    mQuitButton->addActionListener(ActionListener::downcasted_shared_from_this<MainActionListener>());
    mQuitButton->setFont(mFont);
    mQuitButton->adjustSize();
    mMain->add(mQuitButton);
}

void MainActionListener::cleanMain()
{
    mNewGameButton = nullptr;
    mLoadGameButton = nullptr;
    mOptionsButton = nullptr;
    mQuitButton = nullptr;
    mVersionLabel = nullptr;
    mDemoInfo = nullptr;
    mMain = nullptr;
}

void MainActionListener::resizeMain()
{
    mDemoInfo->setPosition(mWidth - 200, mHeight - 50);
    mVersionLabel->setPosition(mWidth - 100, mHeight - 80);

    mMain->setDimension(gui::Rectangle(0, 0, mWidth, mHeight));

    mNewGameButton->setPosition(mWidth/2 - 100, mHeight/2 - 100);
    mLoadGameButton->setPosition(mWidth/2 - 100, mHeight/2 - 60);
    mOptionsButton->setPosition(mWidth/2 - 100, mHeight/2 - 20);
    mQuitButton->setPosition(mWidth/2 - 100, mHeight/2 +60);
}

void MainActionListener::initNewGame()
{
    mNewGame = gui::WidgetFactory::create_zcontainer();
    mNewGame->setVisible(false);
    mNewGame->setOpaque(false);
    mTop->add(mNewGame);

    mNewGameBoxIcon = gui::WidgetFactory::create_zicon(mBoxImage);
    mNewGame->add(mNewGameBoxIcon);

    mNewGameLabel = gui::WidgetFactory::create_zlabel("New Game");
    mNewGameLabel->setFont(mWhiteFont);
    mNewGameLabel->adjustSize();
    mNewGameBoxIcon->add_child(mNewGameLabel);

    mNewGameText = gui::WidgetFactory::create_ztextbox("I'm verry sorry but this is not an actual game.\n"
                                                       "It's a demonstration of the GUI library.\n");
    mNewGameText->setFont(mWhiteFont);
    mNewGameText->setOpaque(false);
    mNewGameText->setEditable(false);
    mNewGameText->setFrameSize(0);
    mNewGameBoxIcon->add_child(mNewGameText);

    mNewGameGenerateButton = gui::WidgetFactory::create_zbutton("Generate");
    mNewGameGenerateButton->setFont(mWhiteFont);
    mNewGameGenerateButton->adjustSize();
    mNewGameGenerateButton->setActionEventId(action::GENERATE_NEW_WORLD_ACTION);
    mNewGameGenerateButton->addActionListener(ActionListener::downcasted_shared_from_this<MainActionListener>());
    mNewGameBoxIcon->add_child(mNewGameGenerateButton);

    mNewGameBackButton = InterfaceWidgetFactory::create_optionbutton("Back");
    mNewGameBackButton->setHighLightFont(mHighLightFont);
    mNewGameBackButton->adjustSize();
    mNewGameBackButton->setActionEventId(action::BACK_TO_MAIN_WINDOW_ACTION);
    mNewGameBackButton->addActionListener(ActionListener::downcasted_shared_from_this<MainActionListener>());
    mNewGameBackButton->setFont(mFont);
    mNewGameBackButton->adjustSize();

    mNewGame->add(mNewGameBackButton);
}

void MainActionListener::cleanNewGame()
{
    mNewGameBoxIcon = nullptr;
    mNewGameText = nullptr;
    mNewGameLabel = nullptr;
    mNewGameGenerateButton = nullptr;
    mNewGame = nullptr;
}

void MainActionListener::resizeNewGame()
{
    mNewGameBackButton->setPosition(mWidth/2 - 290, mHeight/2 + 180);

    mNewGame->setDimension(gui::Rectangle(0, 0, mWidth, mHeight));
    mNewGameBoxIcon->setPosition(mWidth/2 - 300, mHeight/2 - 150);
    mNewGameText->setPosition(15, 30);
    mNewGameLabel->setPosition(450, 5);
    mNewGameGenerateButton->setPosition(250,250);
}

void MainActionListener::initLoadGame()
{
    mLoadGame = gui::WidgetFactory::create_zcontainer();
    mLoadGame->setVisible(false);
    mLoadGame->setOpaque(false);
    mTop->add(mLoadGame);

    mLoadGameBoxIcon = gui::WidgetFactory::create_zicon(mBoxImage);
    mLoadGame->add(mLoadGameBoxIcon);

    mLoadGameLabel = gui::WidgetFactory::create_zlabel("Load Game");
    mLoadGameLabel->setFont(mWhiteFont);
    mLoadGameLabel->adjustSize();
    mLoadGame->add(mLoadGameLabel);

    mLoadGameText = gui::WidgetFactory::create_ztextbox("I'm verry sorry but this is not an actuall game.\n"
                                                        "It's a demonstration of the GUI library.\n");
    mLoadGameText->setFont(mWhiteFont);
    mLoadGameText->setOpaque(false);
    mLoadGameText->setEditable(false);
    mLoadGameText->setFrameSize(0);
    mLoadGame->add(mLoadGameText);

    mLoadGameBackButton = InterfaceWidgetFactory::create_optionbutton("Back");
    mLoadGameBackButton->setHighLightFont(mHighLightFont);
    mLoadGameBackButton->adjustSize();
    mLoadGameBackButton->setActionEventId(action::BACK_TO_MAIN_WINDOW_ACTION);
    mLoadGameBackButton->addActionListener(ActionListener::downcasted_shared_from_this<MainActionListener>());
    mLoadGameBackButton->setFont(mFont);

    mLoadGameBackButton->adjustSize();

    mLoadGame->add(mLoadGameBackButton);
}

void MainActionListener::cleanLoadGame()
{
    mLoadGameBoxIcon = nullptr;
    mLoadGameBackButton = nullptr;
    mLoadGameText = nullptr;
    mLoadGameLabel = nullptr;
    mLoadGame = nullptr;
}

void MainActionListener::resizeLoadGame()
{
    mLoadGameBackButton->setPosition(mWidth/2 - 290, mHeight/2 + 180);

    mLoadGame->setDimension(gui::Rectangle(0, 0, mWidth, mHeight));
    mLoadGameBoxIcon->setPosition(mWidth/2 - 300, mHeight/2 - 150);
    mLoadGameText->setPosition(mWidth/2 - 285, mHeight/2 - 120);
    mLoadGameLabel->setPosition(mWidth/2 + 150, mHeight/2 - 145);
}

void MainActionListener::initOptions()
{
    mOptions = gui::WidgetFactory::create_zcontainer();
    mOptions->setVisible(false);
    mOptions->setOpaque(false);
    mTop->add(mOptions);

    mOptionsBoxIcon = gui::WidgetFactory::create_zicon(mBoxImage);
    mOptions->add(mOptionsBoxIcon);

    mOptionsLabel = gui::WidgetFactory::create_zlabel("Options");
    mOptionsLabel->setFont(mWhiteFont);
    mOptionsLabel->adjustSize();
    mOptionsBoxIcon->add_child(mOptionsLabel);

    mFullScreen = InterfaceWidgetFactory::create_optioncheckbox("Fullscreen");
    mFullScreen->setFont(mWhiteFont);
    mFullScreen->adjustSize();
    mFullScreen->setBackgroundColor(0x102020);
    mFullScreen->setForegroundColor(0xffffff);
    mFullScreen->setBaseColor(0x204545);
    mFullScreen->setActionEventId(action::FULLSCREEN_ACTION);
    mFullScreen->addActionListener(ActionListener::downcasted_shared_from_this<MainActionListener>());
    mFullScreen->setSelected(mHaveFullscreen);
    mOptionsBoxIcon->add_child(mFullScreen);

    mResolutionScrollArea = gui::WidgetFactory::create_zscrollarea();
    mResolutionScrollArea->setBackgroundColor(0x102020);
    mResolutionScrollArea->setForegroundColor(0x102020);
    mResolutionScrollArea->setBaseColor(0x204545);

    mResolutionListBox = gui::WidgetFactory::create_zlistbox();
    mResolutionListBox->setBackgroundColor(0x102020);
    mResolutionListBox->setForegroundColor(0x102020);
    mResolutionListBox->setBaseColor(0x105555);
    mResolutionListBox->setSelectionColor(0x204545);

    mResolutionListModel = std::shared_ptr<ResolutionListModel>(new ResolutionListModel());

    mResolution = gui::WidgetFactory::create_zdropdown(mResolutionListModel,mResolutionScrollArea,mResolutionListBox);
    mResolution->setWidth(200);
    mResolution->setBackgroundColor(0x102020);
    mResolution->setForegroundColor(0x102020);
    mResolution->setBaseColor(0x105555);
    mResolution->setSelectionColor(0x204545);

    if (mWidth == 800)
    {
        mResolution->setSelected(1);
    }
    else
    {
        mResolution->setSelected(0);
    }
    mResolution->setActionEventId(action::RESOLUTION_ACTION);
    mResolution->addActionListener(ActionListener::downcasted_shared_from_this<MainActionListener>());
    //mOptions->add(mResolution);
    mOptionsBoxIcon->add_child(mResolution);

    mVolume = gui::WidgetFactory::create_zslider(0.0, 1.0);
    mVolume->setWidth(200);
    mVolume->setHeight(20);
    mVolume->setValue(0.7);
    mVolume->setBackgroundColor(0x102020);
    mVolume->setForegroundColor(0x102020);
    mVolume->setBaseColor(0x105555);
    mVolume->setActionEventId(action::VOLUME_ACTION);
    mVolume->addActionListener(ActionListener::downcasted_shared_from_this<MainActionListener>());
    mVolume->setMarkerLength(20);
    mOptionsBoxIcon->add_child(mVolume);

    mVolumePercent = gui::WidgetFactory::create_zlabel("70%");
    mOptionsBoxIcon->add_child(mVolumePercent);

    mVolumeLabel = gui::WidgetFactory::create_zlabel("Volume");
    mOptionsBoxIcon->add_child(mVolumeLabel);

    mResolutionLabel = gui::WidgetFactory::create_zlabel("Resolution");
    mOptionsBoxIcon->add_child(mResolutionLabel);

    mOptionsBackButton = InterfaceWidgetFactory::create_optionbutton("Back");
    mOptionsBackButton->setHighLightFont(mHighLightFont);
    mOptionsBackButton->adjustSize();
    mOptionsBackButton->setActionEventId(action::BACK_TO_MAIN_WINDOW_ACTION);
    mOptionsBackButton->addActionListener(ActionListener::downcasted_shared_from_this<MainActionListener>());
    mOptionsBackButton->setFont(mFont);
    mOptionsBackButton->adjustSize();
    mOptions->add(mOptionsBackButton);
}

void MainActionListener::cleanOptions()
{
    mOptionsBoxIcon = nullptr;
    mOptionsBackButton = nullptr;
    mResolutionLabel = nullptr;
    mVolumeLabel = nullptr;
    mVolumePercent = nullptr;
    mVolume = nullptr;
    mResolutionListBox = nullptr;
    mResolutionScrollArea = nullptr;
    mResolutionListModel = nullptr;
    mResolution = nullptr;
    mFullScreen = nullptr;
    mOptionsLabel = nullptr;
    mOptions = nullptr;
}

void MainActionListener::resizeOptions()
{
    mOptionsBackButton->setPosition(mWidth/2 - 290, mHeight/2 + 180);

    mOptions->setDimension(gui::Rectangle(0, 0, mWidth, mHeight));
    mOptionsBoxIcon->setPosition(mWidth/2 - 300, mHeight/2 - 150);
    mOptionsLabel->setPosition(450, 5);
    mFullScreen->setPosition(100, 50);
    mResolution->setPosition(210, 80);
    mResolutionLabel->setPosition(100, 80);
    mVolume->setPosition(210, 110);
    mVolumePercent->setPosition(420, 110);
    mVolumeLabel->setPosition(100, 110);
}