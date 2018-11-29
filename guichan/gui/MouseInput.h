//
// Created by tatiana on 27.09.18.
//

#ifndef GUICHAN_MOUSEINPUT_H
#define GUICHAN_MOUSEINPUT_H

namespace gui {
    /**
     * Internal class that represents mouse input. Generally you won't have to
     * bother using this class unless you implement an Input class for
     * a back end.
     *
     * @author Olof Naess�n
     * @author Per Larsson
     * @since 0.1.0
     */
    class MouseInput {
    public:

        /**
         * Constructor.
         */
        MouseInput();

        /**
         * Constructor.
         *
         * @param button The button pressed.
         * @param type The type of mouse input.
         * @param x The mouse x coordinate.
         * @param y The mouse y coordinate.
         * @param timeStamp The timestamp of the mouse input. Used to
         *                  check for double clicks.
         */
        MouseInput(unsigned int button,
                   unsigned int type,
                   int x,
                   int y,
                   int timeStamp);

        /**
         * Sets the type of the mouse input.
         *
         * @param type The type of the mouse input. Should be a value from the
         *             mouse event type enum
         * @see getType
         * @since 0.1.0
         */
        void setType(unsigned int type);

        /**
         * Gets the type of the mouse input.
         *
         * @return The type of the mouse input. A value from the mouse event
         *         type enum.
         * @see setType
         * @since 0.1.0
         */
        unsigned int getType() const;

        /**
         * Sets the button pressed.
         *
         * @param button The button pressed. Should be one of the values
         *               in the mouse event button enum.
         * @see getButton.
         * @since 0.1.0
         */
        void setButton(unsigned int button);

        /**
         * Gets the button pressed.
         *
         * @return The button pressed. A value from the mouse event
         *         button enum.
         * @see setButton
         * @since 0.1.0
         */
        unsigned int getButton() const;

        /**
         * Sets the timestamp for the mouse input.
         * Used to check for double clicks.
         *
         * @param timeStamp The timestamp of the mouse input.
         * @see getTimeStamp
         * @since 0.1.0
         */
        void setTimeStamp(int timeStamp);

        /**
         * Gets the time stamp of the input.
         * Used to check for double clicks.
         *
         * @return The time stamp of the mouse input.
         * @see setTimeStamp
         * @since 0.1.0
         */
        int getTimeStamp() const;

        /**
         * Sets the x coordinate of the mouse input.
         *
         * @param x The x coordinate of the mouse input.
         * @see getX
         * @since 0.6.0
         */
        void setX(int x);

        /**
         * Gets the x coordinate of the mouse input.
         *
         * @return The x coordinate of the mouse input.
         * @see setX
         * @since 0.6.0
         */
        int getX() const;

        /**
         * Sets the y coordinate of the mouse input.
         *
         * @param y The y coordinate of the mouse input.
         * @see getY
         * @since 0.6.0
         */
        void setY(int y);

        /**
         * Gets the y coordinate of the mouse input.
         *
         * @return The y coordinate of the mouse input.
         * @see setY
         * @since 0.6.0
         */
        int getY() const;

        /**
         * Mouse input event types. This enum partially corresponds
         * to the enum with event types in MouseEvent for easy mapping.
         */
        enum
        {
            Moved = 0,
            Pressed,
            Released,
            WheelMovedDown,
            WheelMovedUp
        };

        /**
         * Mouse button types.
         */
        enum
        {
            Empty = 0,
            Left,
            Right,
            Middle
        };

    protected:
        /**
         * Holds the type of the mouse input.
         */
        unsigned int mType;

        /**
         * Holds the button of the mouse input.
         */
        unsigned int mButton;

        /**
         * Holds the timestamp of the mouse input. Used to
         * check for double clicks.
         */
        int mTimeStamp;

        /**
         * Holds the x coordinate of the mouse input.
         */
        int mX;

        /**
         * Holds the y coordinate of the mouse input.
         */
        int mY;
    };
}

#endif //GUICHAN_MOUSEINPUT_H
