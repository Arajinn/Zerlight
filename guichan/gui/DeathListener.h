//
// Created by tatiana on 26.09.18.
//

#ifndef GUICHAN_DEATHLISTENER_H
#define GUICHAN_DEATHLISTENER_H

namespace gui {
    class Event;
    /**
     * Interface for listening for death events from widgets.
     *
     * @see Widget::addDeathListener, Widget::removeDeathListener
     * @author Olof Naess�n
     * @since 0.6.0
     */
    class DeathListener {
    public:

        /**
         * Destructor.
         */
        virtual ~DeathListener();

        /**
         * Called when a widget dies. It is used to be able to recieve
         * a notification when a death of a widget occurs.
         *
         * @param event The event of the death.
         */
        virtual void death(const Event& event) = 0;

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of DeathListener,
         * therefore its constructor is protected.
         */
        DeathListener();
    };
}

#endif //GUICHAN_DEATHLISTENER_H
