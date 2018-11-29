//
// Created by tatiana on 26.09.18.
//

#ifndef GUICHAN_EVENT_H
#define GUICHAN_EVENT_H

#include <memory>

namespace gui {
    class ZWidget;
    /**
     * Base class for all events. All events in Guichan should
     * inherit from this class.
     *
     * @author Olof Naess�n
     * @since 0.6.0
     */
    class Event {
    public:

        /**
         * Constructor.
         *
         * @param source The source widget of the event.
         */
        Event(std::shared_ptr<ZWidget> source);

        /**
         * Destructor.
         */
        virtual ~Event();

        /**
         * Gets the source widget of the event. The function
         * is used to tell which widget fired an event.
         *
         * @return The source widget of the event.
         */
        std::shared_ptr<ZWidget> getSource() const;


    protected:

        /**
         * Holds the source widget of the event.
         */
        std::shared_ptr<ZWidget> mSource;
    };
}

#endif //GUICHAN_EVENT_H
