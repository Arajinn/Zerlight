//
// Created by tatiana on 28.09.18.
//

#ifndef GUICHAN_GUI_H
#define GUICHAN_GUI_H

#include "guichan/gui/KeyEvent.h"

#include <memory>
#include <set>
#include <list>

namespace gui {
    class ZWidget;
    class Graphics;
    class Input;
    class KeyListener;
    class MouseInput;
    class FocusHandler;

    /**
     * @mainpage
     * @section Introduction
     * This documentation is mostly intended as a reference to the API. If you want to get started with Guichan, we suggest you check out the programs in the examples directory of the Guichan release.
     * @n
     * @n
     * This documentation is, and will always be, work in progress. If you find any errors, typos or inconsistencies, or if you feel something needs to be explained in more detail - don't hesitate to tell us.
     */

    /**
     * Contains a Guichan GUI. This is the core class of Guichan to which
     * implementations of back ends are passed, to make Guichan work with
     * a specific library, and to where the top widget (root widget of GUI)
     * is added. If you want to be able to have more then one widget in your
     * GUI, the top widget should be a container.
     *
     * A Gui object cannot work properly without passing back end
     * implementations to it. A Gui object must have an implementation of a
     * Graphics and an implementation of Input.
     *
     * NOTE: A complete GUI also must have the ability to load images.
     *       Images are loaded with the Image class, so to make Guichan
     *       able to load images an implementation of ImageLoader must be
     *       passed to Image.
     *
     * @see Graphics, Input, Image
     */
    class Gui {
    public:
        /**
         * Constructor.
         */
        Gui();

        /**
         * Destructor.
         */
        virtual ~Gui();

        /**
         * Sets the top widget. The top widget is the root widget
         * of the GUI. If you want a GUI to be able to contain more
         * than one widget the top widget should be a container.
         *
         * @param top The top widget.
         * @see Container
         * @since 0.1.0
         */
        virtual void setTop(std::shared_ptr<ZWidget> top);

        /**
         * Gets the top widget. The top widget is the root widget
         * of the GUI.
         *
         * @return The top widget. NULL if no top widget has been set.
         * @since 0.1.0
         */
        virtual std::shared_ptr<ZWidget> getTop() const;

        /**
         * Sets the graphics object to use for drawing.
         *
         * @param graphics The graphics object to use for drawing.
         * @see getGraphics, AllegroGraphics, HGEGraphics,
         *      OpenLayerGraphics, OpenGLGraphics, SDLGraphics
         * @since 0.1.0
         */
        virtual void setGraphics(std::shared_ptr<Graphics> graphics);

        /**
         * Gets the graphics object used for drawing.
         *
         *  @return The graphics object used for drawing. NULL if no
         *          graphics object has been set.
         * @see setGraphics, AllegroGraphics, HGEGraphics,
         *      OpenLayerGraphics, OpenGLGraphics, SDLGraphics
         * @since 0.1.0
         */
        virtual std::shared_ptr<Graphics> getGraphics() const;

        /**
         * Sets the input object to use for input handling.
         *
         * @param input The input object to use for input handling.
         * @see getInput, AllegroInput, HGEInput, OpenLayerInput,
         *      SDLInput
         * @since 0.1.0
         */
        virtual void setInput(std::shared_ptr<Input> input);

        /**
         * Gets the input object being used for input handling.
         *
         *  @return The input object used for handling input. NULL if no
         *          input object has been set.
         * @see setInput, AllegroInput, HGEInput, OpenLayerInput,
         *      SDLInput
         * @since 0.1.0
         */
        virtual std::shared_ptr<Input> getInput() const;

        /**
         * Performs logic of the GUI. By calling this function all logic
         * functions down in the GUI heirarchy will be called. When logic
         * is called for Gui, user input will be handled.
         *
         * @see Widget::logic
         * @since 0.1.0
         */
        virtual void logic();

        /**
         * Draws the GUI. By calling this funcion all draw functions
         * down in the GUI hierarchy will be called. When draw is called
         * the used Graphics object will be initialised and drawing of
         * the top widget will commence.
         *
         * @see Widget::draw
         * @since 0.1.0
         */
        virtual void draw();

        /**
         * Focuses none of the widgets in the Gui.
         *
         * @since 0.1.0
         */
        virtual void focusNone();

        /**
         * Sets tabbing enabled, or not. Tabbing is the usage of
         * changing focus by utilising the tab key.
         *
         * @param tabbing True if tabbing should be enabled, false
         *                otherwise.
         * @see isTabbingEnabled
         * @since 0.1.0
         */
        virtual void setTabbingEnabled(bool tabbing);

        /**
         * Checks if tabbing is enabled.
         *
         * @return True if tabbing is enabled, false otherwise.
         * @see setTabbingEnabled
         * @since 0.1.0
         */
        virtual bool isTabbingEnabled();

        /**
         * Adds a global key listener to the Gui. A global key listener
         * will receive all key events generated from the GUI and global
         * key listeners will receive the events before key listeners
         * of widgets.
         *
         * @param keyListener The key listener to add.
         * @see removeGlobalKeyListener
         * @since 0.5.0
         */
        virtual void addGlobalKeyListener(std::shared_ptr<KeyListener> keyListener);

        /**
         * Removes global key listener from the Gui.
         *
         * @param keyListener The key listener to remove.
         * @throws Exception if the key listener hasn't been added.
         * @see addGlobalKeyListener
         * @since 0.5.0
         */
        virtual void removeGlobalKeyListener(std::shared_ptr<KeyListener> keyListener);

    protected:
        /**
         * Handles all mouse input.
         *
         * @since 0.6.0
         */
        virtual void handleMouseInput();

        /**
         * Handles key input.
         *
         * @since 0.6.0
         */
        virtual void handleKeyInput();

        /**
         * Handles mouse moved input.
         *
         * @param mouseInput The mouse input to handle.
         * @since 0.6.0
         */
        virtual void handleMouseMoved(const MouseInput& mouseInput);

        /**
         * Handles mouse pressed input.
         *
         * @param mouseInput The mouse input to handle.
         * @since 0.6.0
         */
        virtual void handleMousePressed(const MouseInput& mouseInput);

        /**
         *
         * Handles mouse wheel moved down input.
         *
         * @param mouseInput The mouse input to handle.
         * @since 0.6.0
         */
        virtual void handleMouseWheelMovedDown(const MouseInput& mouseInput);

        /**
         * Handles mouse wheel moved up input.
         *
         * @param mouseInput The mouse input to handle.
         * @since 0.6.0
         */
        virtual void handleMouseWheelMovedUp(const MouseInput& mouseInput);

        /**
         * Handles mouse released input.
         *
         * @param mouseInput The mouse input to handle.
         * @since 0.6.0
         */
        virtual void handleMouseReleased(const MouseInput& mouseInput);

        /**
         * Handles modal focus. Modal focus needs to be checked at
         * each logic iteration as it might be necessary to distribute
         * mouse entered or mouse exited events.
         *
         * @since 0.8.0
         */
        virtual void handleModalFocus();

        /**
         * Handles modal mouse input focus. Modal mouse input focus needs
         * to be checked at each logic iteration as it might be necessary to
         * distribute mouse entered or mouse exited events.
         *
         * @since 0.8.0
         */
        virtual void handleModalMouseInputFocus();

        /**
         * Handles modal focus gained. If modal focus has been gained it might
         * be necessary to distribute mouse entered or mouse exited events.
         *
         * @since 0.8.0
         */
        virtual void handleModalFocusGained();

        /**
         * Handles modal mouse input focus gained. If modal focus has been
         * gained it might be necessary to distribute mouse entered or mouse
         * exited events.
         *
         * @since 0.8.0
         */
        virtual void handleModalFocusReleased();

        /**
         * Distributes a mouse event.
         *
         * @param type The type of the event to distribute,
         * @param button The button of the event (if any used) to distribute.
         * @param x The x coordinate of the event.
         * @param y The y coordinate of the event.
         * @param fource indicates whether the distribution should be forced or not.
         *               A forced distribution distributes the event even if a widget
         *               is not enabled, not visible, another widget has modal
         *               focus or another widget has modal mouse input focus.
         *               Default value is false.
         * @param toSourceOnly indicates whether the distribution should be to the
         *                     source widget only or to it's parent's mouse listeners
         *                     as well.
         *
         * @since 0.6.0
         */
        virtual void distributeMouseEvent(std::shared_ptr<ZWidget> source,
                                          int type,
                                          int button,
                                          int x,
                                          int y,
                                          bool force = false,
                                          bool toSourceOnly = false);

        /**
         * Distributes a key event.
         *
         * @param keyEvent The key event to distribute.

         * @since 0.6.0
         */
        virtual void distributeKeyEvent(KeyEvent& keyEvent);

        /**
         * Distributes a key event to the global key listeners.
         *
         * @param keyEvent The key event to distribute.
         *
         * @since 0.6.0
         */
        virtual void distributeKeyEventToGlobalKeyListeners(KeyEvent& keyEvent);

        /**
         * Gets the widget at a certain position.
         *
         * @return The widget at a certain position.
         * @since 0.6.0
         */
        virtual std::shared_ptr<ZWidget> getWidgetAt(int x, int y);

        /**
         * Gets the source of the mouse event.
         *
         * @return The source widget of the mouse event.
         * @since 0.6.0
         */
        virtual std::shared_ptr<ZWidget> getMouseEventSource(int x, int y);

        /**
         * Gets the source of the key event.
         *
         * @return The source widget of the key event.
         * @since 0.6.0
         */
        virtual std::shared_ptr<ZWidget> getKeyEventSource();

        /**
         * Gets all widgets a certain coordinate in the Gui.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         * @return A set of all widgets at the specified coordinate.
         * @since 0.9.0
         */
        virtual std::set<std::shared_ptr<ZWidget>> getWidgetsAt(int x, int y);

        /**
         * Holds the top widget.
         */
        std::shared_ptr<ZWidget> mTop;

        /**
         * Holds the graphics implementation used.
         */
        std::shared_ptr<Graphics> mGraphics;

        /**
         * Holds the input implementation used.
         */
        std::shared_ptr<Input> mInput;

        /**
         * Holds the focus handler for the Gui.
         */
        std::shared_ptr<FocusHandler> mFocusHandler;

        /**
         * True if tabbing is enabled, false otherwise.
         */
        bool mTabbing;

        /**
         * Holds the global key listeners of the Gui.
         */
        std::list<std::shared_ptr<KeyListener>> mKeyListeners;

        /**
         * True if shift is pressed, false otherwise.
         */
        bool mShiftPressed;

        /**
         * True if meta is pressed, false otherwise.
         */
        bool mMetaPressed;

        /**
         * True if control is pressed, false otherwise.
         */
        bool mControlPressed;

        /**
         * True if alt is pressed, false otherwise.
         */
        bool mAltPressed;

        /**
         * Holds the last mouse button pressed.
         */
        unsigned int mLastMousePressButton;

        /**
         * Holds the last mouse press time stamp.
         */
        int mLastMousePressTimeStamp;

        /**
         * Holds the last mouse x coordinate.
         */
        int mLastMouseX;

        /**
         * Holds the last mouse y coordinate.
         */
        int mLastMouseY;

        /**
         * Holds the current click count. Used to keep track
         * of clicks for a the last pressed button.
         */
        int mClickCount;

        /**
         * Holds the last button used when a drag of a widget
         * was initiated. Used to be able to release a drag
         * when the same button is released.
         */
        int mLastMouseDragButton;
    };
}

#endif //GUICHAN_GUI_H
