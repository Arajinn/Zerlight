//
// Created by tatiana on 27.09.18.
//

#ifndef GUICHAN_KEYEVENT_H
#define GUICHAN_KEYEVENT_H

#include "guichan/gui/InputEvent.h"
#include "guichan/gui/Key.h"

namespace gui {
    /**
     * Represents a key event.
     */
    class KeyEvent : public InputEvent{
    public:
        /**
         * Key event types.
         */
        enum
        {
            Pressed = 0,
            Released
        };

        /**
         * Constructor.
         *
         * @param source The widget the event concerns..
         * @param distributor The distributor of the event.
         * @param isShiftPressed True if shift is pressed, false otherwise.
         * @param isControlPressed True if control is pressed, false otherwise.
         * @param isAltPressed True if alt is pressed, false otherwise.
         * @param isMetaPressed True if meta is pressed, false otherwise.
         * @param type The type of the event. A value from KeyEventType.
         * @param isNumericPad True if the event occured on the numeric pad,
         *                     false otherwise.
         * @param key The key of the event.
         */
        KeyEvent(std::shared_ptr<ZWidget> source,
                 std::shared_ptr<ZWidget> distributor,
                 bool isShiftPressed,
                 bool isControlPressed,
                 bool isAltPressed,
                 bool isMetaPressed,
                 unsigned int type,
                 bool isNumericPad,
                 const Key& key);

        /**
         * Destructor.
         */
        virtual ~KeyEvent();

        /**
         * Gets the type of the event.
         *
         * @return The type of the event.
         */
        unsigned int getType() const;

        /**
         * Checks if the key event occured on the numeric pad.
         *
         * @return True if key event occured on the numeric pad,
         *         false otherwise.
         *
         */
        bool isNumericPad() const;

        /**
         * Gets the key of the event.
         *
         * @return The key of the event.
         */
        const Key& getKey() const;

    protected:
        /**
         * Holds the type of the key event.
         */
        unsigned int mType;

        /**
         * True if the numeric pad was used, false otherwise.
         */
        bool mIsNumericPad;

        /**
         * Holds the key of the key event.
         */
        Key mKey;
    };
}

#endif //GUICHAN_KEYEVENT_H
