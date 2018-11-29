//
// Created by tatiana on 26.09.18.
//

#ifndef GUICHAN_ACTIONLISTENER_H
#define GUICHAN_ACTIONLISTENER_H

#include "guichan/gui/MultipleInheritable.h"

namespace gui {
    class ActionEvent;
    /**
     * Interface for listening for action events from widgets.
     *
     * @see Widget::addActionListener, Widget::removeActionListener,
     *      ActionEvent
     * @author Olof Naess�n
     * @author Per Larsson
     */
    class ActionListener  : public inheritable_enable_shared_from_this<ActionListener> {
    public:

        /**
         * Destructor.
         */
        virtual ~ActionListener();

        /**
         * Called when an action is recieved from a widget. It is used
         * to be able to recieve a notification that an action has
         * occured.
         *
         * @param actionEvent The event of the action.
         * @since 0.6.0
         */
        virtual void action(const ActionEvent& actionEvent) = 0;

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of ActionListener,
         * therefore its constructor is protected.
         */
        ActionListener();
    };
}

#endif //GUICHAN_ACTIONLISTENER_H
