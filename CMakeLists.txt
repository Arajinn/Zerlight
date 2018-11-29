cmake_minimum_required(VERSION 3.12)
project(Zerlight)

set(CMAKE_CXX_STANDARD 14)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer)

FIND_PACKAGE(OpenGL)
FIND_PACKAGE(Threads)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS})

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
        interface/OptionButton.cpp interface/OptionButton.h interface/MainActionListener.cpp interface/MainActionListener.h interface/MainActionOptions.cpp interface/InterfaceWidgetFactory.cpp interface/InterfaceWidgetFactory.h interface/MapWidget.cpp interface/MapWidget.h)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2IMAGE_LIBRARIES}
        ${SDL2MIXER_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        ${OPENGL_glu_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT})