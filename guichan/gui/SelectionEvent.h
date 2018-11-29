//
// Created by tatiana on 28.09.18.
//

#ifndef GUICHAN_SELECTIONEVENT_H
#define GUICHAN_SELECTIONEVENT_H

#include "guichan/gui/Event.h"

namespace gui {
    class ZWidget;
    /**
     * Represents a selection event.
     *
     * @author Olof Naess�n
     * @since 0.8.0
     */
    class SelectionEvent : public Event{
    public:

        /**
         * Constructor.
         *
         * @param source source The widget of the selection event.
         */
        SelectionEvent(std::shared_ptr<ZWidget> source);

        /**
         * Destructor.
         */
        virtual ~SelectionEvent();
    };
}

#endif //GUICHAN_SELECTIONEVENT_H
