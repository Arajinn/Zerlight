//
// Created by tatiana on 27.09.18.
//

#ifndef GUICHAN_SDLINPUT_H
#define GUICHAN_SDLINPUT_H

#include "guichan/gui/Input.h"

#include <SDL2/SDL_events.h>

#include <queue>

namespace sdl_interface {
    /**
     * SDL implementation of Input.
     */
    class SDLInput : public gui::Input{
    public:

        /**
         * Constructor.
         */
        SDLInput();

        /**
         * Pushes an SDL event. It should be called at least once per frame to
         * update input with user input.
         *
         * @param event an event from SDL.
         */
        virtual void pushInput(SDL_Event event);

        /**
         * Polls all input. It exists for input driver compatibility. If you
         * only use SDL and plan sticking with SDL you can safely ignore this
         * function as it in the SDL case does nothing.
         */
        virtual void pollInput();


        // Inherited from Input

        virtual bool isKeyQueueEmpty();

        virtual gui::KeyInput dequeueKeyInput();

        virtual bool isMouseQueueEmpty();

        virtual gui::MouseInput dequeueMouseInput();

    protected:
        /**
         * Converts a mouse button from SDL to a Guichan mouse button
         * representation.
         *
         * @param button an SDL mouse button.
         * @return a Guichan mouse button.
         */
        int convertMouseButton(int button);

        /**
         * Converts an SDL event to a Guichan key value.
         *
         * @param keysym The SDL event to convert.
         * @return A Guichan key value. -1 if no conversion took place.
         * @see Key
         */
        int convertSDLEventToGuichanKeyValue(SDL_Event event);

        std::queue<gui::KeyInput> mKeyInputQueue;
        std::queue<gui::MouseInput> mMouseInputQueue;

        bool mMouseDown;
        bool mMouseInWindow;
    };
}

#endif //GUICHAN_SDLINPUT_H
