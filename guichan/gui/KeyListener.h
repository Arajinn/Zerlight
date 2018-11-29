//
// Created by tatiana on 27.09.18.
//

#ifndef GUICHAN_KEYLISTENER_H
#define GUICHAN_KEYLISTENER_H

#include "guichan/gui/MultipleInheritable.h"

namespace gui {
    class KeyEvent;

    /**
     * Interface for listening for key events from widgets.
     *
     * @see Widget::addKeyListener, Widget::removeKeyListener
     */
    class KeyListener : public inheritable_enable_shared_from_this<KeyListener>{
    public:

        /**
         * Destructor.
         */
        virtual ~KeyListener();

        /**
         * Called if a key is pressed when the widget has keyboard focus.
         * If a key is held down the widget will generate multiple key
         * presses.
         *
         * @param keyEvent Discribes the event.
         */
        virtual void keyPressed(KeyEvent& keyEvent);

        /**
         * Called if a key is released when the widget has keyboard focus.
         *
         * @param keyEvent Discribes the event.
         */
        virtual void keyReleased(KeyEvent& keyEvent);

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of KeyListener,
         * therefore its constructor is protected.
         */
        KeyListener();
    };
}

#endif //GUICHAN_KEYLISTENER_H
