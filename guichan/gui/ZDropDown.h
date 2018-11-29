//
// Created by tatiana on 28.09.18.
//

#ifndef GUICHAN_ZDROPDOWN_H
#define GUICHAN_ZDROPDOWN_H

#include "guichan/gui/ActionListener.h"
#include "guichan/gui/MouseListener.h"
#include "guichan/gui/KeyListener.h"
#include "guichan/gui/FocusListener.h"
#include "guichan/gui/SelectionListener.h"
#include "guichan/gui/ZWidget.h"

namespace gui {
    class ListModel;
    class ZScrollArea;
    class ZListBox;
    /**
     * An implementation of a drop downable list from which an item can be
     * selected. The drop down consists of an internal ScrollArea and an
     * internal ListBox. The drop down also uses an internal FocusHandler to
     * handle the focus of the internal ScollArea and the internal ListBox. The
     * scroll area and the list box can be passed to the drop down if a custom
     * scroll area and or a custom list box is preferable.
     *
     * To be able display a list the drop down uses a user provided list model.
     * A list model can be any class that implements the ListModel interface.
     *
     * If an item is selected in the drop down a select event will be sent to
     * all selection listeners of the drop down. If an item is selected by
     * using a mouse click or by using the enter or space key an action event
     * will be sent to all action listeners of the drop down.
     */
    class ZDropDown : public ZWidget, public ActionListener, public MouseListener,
            public KeyListener, public FocusListener, public SelectionListener{
    public:
        /**
         * Contructor.
         *
         * @param listModel the ListModel to use.
         * @param scrollArea the ScrollArea to use.
         * @param listBox the listBox to use.
         * @see ListModel, ScrollArea, ListBox.
         */
        ZDropDown();
        void init(std::shared_ptr<ListModel> listModel = nullptr,
                  std::shared_ptr<ZScrollArea> scrollArea = nullptr,
                  std::shared_ptr<ZListBox> listBox = nullptr);

        /**
         * Destructor.
         */
        virtual ~ZDropDown();

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
         * Sets the list model to use when displaying the list.
         *
         * @param listModel the list model to use.
         * @see getListModel
         */
        void setListModel(std::shared_ptr<ListModel> listModel);

        /**
         * Gets the list model used.
         *
         * @return the ListModel used.
         * @see setListModel
         */
        std::shared_ptr<ListModel> getListModel() const;

        /**
         * Adjusts the height of the drop down to fit the height of the
         * drop down's parent's height. It's used to not make the drop down
         * draw itself outside of it's parent if folded down.
         */
        void adjustHeight();

        /**
         * Adds a selection listener to the drop down. When the selection
         * changes an event will be sent to all selection listeners of the
         * drop down.
         *
         * If you delete your selection listener, be sure to also remove it
         * using removeSelectionListener().
         *
         * @param selectionListener the selection listener to add.
         * @since 0.8.0
         */
        void addSelectionListener(std::shared_ptr<SelectionListener> selectionListener);

        /**
         * Removes a selection listener from the drop down.
         *
         * @param selectionListener the selection listener to remove.
         * @since 0.8.0
         */
        void removeSelectionListener(std::shared_ptr<SelectionListener> selectionListener);


        // Inherited from Widget

        virtual void draw_item(std::shared_ptr<Graphics> graphics);

        void setBaseColor(const ZColor& color);

        void setBackgroundColor(const ZColor& color);

        void setForegroundColor(const ZColor& color);

        void setFont(std::shared_ptr<Font> font);

        void setSelectionColor(const ZColor& color);


        // Inherited from BasicContainer

        virtual Rectangle getChildrenArea();


        // Inherited from FocusListener

        virtual void focusLost(const Event& event);


        // Inherited from ActionListener

        virtual void action(const ActionEvent& actionEvent);


        // Inherited from DeathListener

        virtual void death(const Event& event);


        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent);


        // Inherited from MouseListener

        virtual void mousePressed(MouseEvent& mouseEvent);

        virtual void mouseReleased(MouseEvent& mouseEvent);

        virtual void mouseWheelMovedUp(MouseEvent& mouseEvent);

        virtual void mouseWheelMovedDown(MouseEvent& mouseEvent);

        virtual void mouseDragged(MouseEvent& mouseEvent);


        // Inherited from SelectionListener

        virtual void valueChanged(const SelectionEvent& event);

    protected:
        /**
         * Draws the button of the drop down.
         *
         * @param graphics a Graphics object to draw with.
         */
        virtual void drawButton(std::shared_ptr<Graphics> graphics);

        /**
         * Sets the drop down to be dropped down.
         */
        virtual void dropDown();

        /**
         * Sets the drop down to be folded up.
         */
        virtual void foldUp();

        /**
         * Distributes a value changed event to all selection listeners
         * of the drop down.
         *
         * @since 0.8.0
         */
        void distributeValueChangedEvent();

        /**
         * True if the drop down is dropped down, false otherwise.
         */
        bool mDroppedDown;

        /**
         * True if the drop down has been pushed with the mouse, false
         * otherwise.
         */
        bool mPushed;

        /**
         * Holds what the height is if the drop down is folded up. Used when
         * checking if the list of the drop down was clicked or if the upper
         * part of the drop down was clicked on a mouse click.
         */
        int mFoldedUpHeight;

        /**
         * The scroll area used.
         */
        std::shared_ptr<ZScrollArea> mScrollArea;

        /**
         * The list box used.
         */
        std::shared_ptr<ZListBox> mListBox;

        /**
         * The internal focus handler used to keep track of focus for the
         * internal list box.
         */
        std::shared_ptr<FocusHandler> mInternalFocusHandler;

        /**
         * True if an internal scroll area is used, false if a scroll area
         * has been passed to the drop down which the drop down should not
         * deleted in it's destructor.
         */
        bool mInternalScrollArea;

        /**
         * True if an internal list box is used, false if a list box
         * has been passed to the drop down which the drop down should not
         * deleted in it's destructor.
         */
        bool mInternalListBox;

        /**
         * True if the drop down is dragged.
         */
        bool mIsDragged;

        /**
         * The selection listener's of the drop down.
         */
        std::list<std::shared_ptr<SelectionListener>>  mSelectionListeners;
    };
}

#endif //GUICHAN_ZDROPDOWN_H
