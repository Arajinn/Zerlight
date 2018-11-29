//
// Created by tatiana on 28.09.18.
//

#ifndef GUICHAN_SELECTIONLISTENER_H
#define GUICHAN_SELECTIONLISTENER_H

#include "guichan/gui/MultipleInheritable.h"

namespace gui {
    class SelectionEvent;

    /**
     * Interface for listening for selection events from widgets.
     *
     * @see ListBox::addSelectionListener,
     *      ListBox::removeSelectionListener,
     *      DropDown::addSelectionListener,
     *      DropDown::removeSelectionListener
     * @author Olof Naess�n
     * @since 0.8.0
     */
    class SelectionListener : public inheritable_enable_shared_from_this<SelectionListener> {
    public:
        /**
         * Destructor.
         */
        virtual ~SelectionListener();

        /**
         * Called when the value of a selection has been changed in a Widget.
         * It is used to be able to recieve a notification that a value has
         * been changed.
         *
         * @param event The event of the value change.
         * @since 0.8.0
         */
        virtual void valueChanged(const SelectionEvent& event);

    protected:
        /**
         * Constructor.
         *
         * You should not be able to make an instance of SelectionListener,
         * therefore its constructor is protected.
         */
        SelectionListener();
    };
}

#endif //GUICHAN_SELECTIONLISTENER_H
