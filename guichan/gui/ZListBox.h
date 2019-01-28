//
// Created by tatiana on 28.09.18.
//

#ifndef GUICHAN_ZLISTBOX_H
#define GUICHAN_ZLISTBOX_H

#include "guichan/gui/ZWidget.h"
#include "guichan/gui/MouseListener.h"
#include "guichan/gui/KeyListener.h"
#include "guichan/gui/FocusListener.h"

namespace gui {
    class ListModel;
    class SelectionListener;

    /**
     * An implementation of a list box where an item can be selected.
     *
     * To be able display a list the list box uses a user provided list model.
     * A list model can be any class that implements the ListModel interface.
     *
     * If an item is selected in the list box a select event will be sent to
     * all selection listeners of the list box. If an item is selected by using
     * a mouse click or by using the enter or space key an action event will be
     * sent to all action listeners of the list box.
     */
    class ZListBox : public ZWidget, public MouseListener, public KeyListener, public FocusListener{
    public:
        /**
         * Constructor.
         */
        ZListBox();

        virtual void init();
        virtual void shutdown();

        /**
         * Constructor.
         *
         * @param listModel the list model to use.
         */
        virtual void init(std::shared_ptr<ListModel> listModel);

        /**
         * Destructor.
         */
        virtual ~ZListBox();

        /**
         * Gets the selected item as an index in the list model.
         *
         * @return the selected item as an index in the list model.
         * @see setSelected
         */
        int getSelected() const;

        /**
        * Sets the selected item. The selected item is represented by
        * an index from the list model.
        *
        * @param selected the selected item as an index from the list model.
        * @see getSelected
        */
        void setSelected(int selected);

        /**
         * Sets the list model to use.
         *
         * @param listModel the list model to use.
         * @see getListModel
         */
        void setListModel(std::shared_ptr<ListModel> listModel);

        /**
         * Gets the list model used.
         *
         * @return the list model used.
         * @see setListModel
         */
        std::shared_ptr<ListModel> getListModel() const;

        /**
         * Adjusts the size of the list box to fit it's list model.
         */
        void adjustSize();

        /**
         * Checks whether the list box wraps when selecting items with a
         * keyboard.
         *
         * Wrapping means that the selection of items will be wrapped. That is,
         * if the first item is selected and up is pressed, the last item will
         * get selected. If the last item is selected and down is pressed, the
         * first item will get selected.
         *
         * @return true if wrapping is enabled, fasle otherwise.
         * @see setWrappingEnabled
         */
        bool isWrappingEnabled() const;

        /**
         * Sets the list box to wrap or not when selecting items with a
         * keyboard.
         *
         * Wrapping means that the selection of items will be wrapped. That is,
         * if the first item is selected and up is pressed, the last item will
         * get selected. If the last item is selected and down is pressed, the
         * first item will get selected.
         *
         * @see isWrappingEnabled
         */
        void setWrappingEnabled(bool wrappingEnabled);

        /**
         * Adds a selection listener to the list box. When the selection
         * changes an event will be sent to all selection listeners of the
         * list box.
         *
         * If you delete your selection listener, be sure to also remove it
         * using removeSelectionListener().
         *
         * @param selectionListener The selection listener to add.
         * @since 0.8.0
         */
        void addSelectionListener(std::shared_ptr<SelectionListener> selectionListener);

        /**
         * Removes a selection listener from the list box.
         *
         * @param selectionListener The selection listener to remove.
         * @since 0.8.0
         */
        void removeSelectionListener(std::shared_ptr<SelectionListener> selectionListener);

        /**
         * Gets the height of a row. Should be overridden if another row
         * height than the font height is preferred.
         *
         * @return The height of a row.
         * @since 0.8.0
         */
        virtual unsigned int getRowHeight() const;


        // Inherited from Widget

        virtual void draw_item(std::shared_ptr<Graphics> graphics);

        virtual void logic_item();


        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent);


        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent);

        virtual void mouseWheelMovedUp(MouseEvent& mouseEvent);

        virtual void mouseWheelMovedDown(MouseEvent& mouseEvent);

        virtual void mouseDragged(MouseEvent& mouseEvent);


    protected:
        /**
         * Distributes a value changed event to all selection listeners
         * of the list box.
         *
         * @since 0.8.0
         */
        void distributeValueChangedEvent();

        /**
         * The selected item as an index in the list model.
         */
        int mSelected;

        /**
         * The list model to use.
         */
        std::shared_ptr<ListModel> mListModel;

        /**
         * True if wrapping is enabled, false otherwise.
         */
        bool mWrappingEnabled;

        /**
         * The selection listeners of the list box.
         */
        std::list<std::shared_ptr<SelectionListener>>  mSelectionListeners;
    };
}

#endif //GUICHAN_ZLISTBOX_H
