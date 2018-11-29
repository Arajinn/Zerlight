//
// Created by tatiana on 28.09.18.
//

#ifndef GUICHAN_CONTAINEREVENT_H
#define GUICHAN_CONTAINEREVENT_H

#include "guichan/gui/Event.h"

namespace gui {
    class ZWidget;
    class ZContainer;

    /**
     * Represents a container event. A container event is an event
     * that can be fired by a container whenever a widget is added
     * or removed.
     *
     * Any object can listen for actions from a container by implementing
     * the ContainerListener interface.
     *
     * @see Container::addContainerListener, Container::removeContainerListener,
     *      Container::distributeContainerEvent
     * @author Olof Naess�n
     * @since 0.9.0
     */
    class ContainerEvent : public Event{
    public:

        /**
         * Constructor.
         *
         * @param source The source widget of the event.
         * @param container The container the event concerns.
         */
        ContainerEvent(std::shared_ptr<ZWidget> source, std::shared_ptr<ZContainer> container);

        /**
         * Destructor.
         */
        virtual ~ContainerEvent();

        /**
         * Gets the container the event concerns.
         *
         * @return The container the event concerns.
         */
        std::shared_ptr<ZContainer> getContainer() const;


    protected:
        /**
         * Holds the container the event concerns.
         */
        std::shared_ptr<ZContainer> mContainer;
    };
}

#endif //GUICHAN_CONTAINEREVENT_H
