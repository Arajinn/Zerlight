//
// Created by tatiana on 27.09.18.
//

#ifndef GUICHAN_ZCHECKBOX_H
#define GUICHAN_ZCHECKBOX_H

#include "guichan/gui/ZWidget.h"
#include "guichan/gui/MouseListener.h"
#include "guichan/gui/KeyListener.h"

namespace gui {
    /**
     * An implementation of a check box where a user can select or deselect
     * the check box and where the status of the check box is displayed to the user.
     * A check box is capable of displaying a caption.
     *
     * If a check box's state changes an action event will be sent to all action
     * listeners of the check box.
     */
    class ZCheckBox : public ZWidget, public MouseListener, public KeyListener{
    public:

        /**
         * Contructor.
         */
        ZCheckBox();

        virtual void init();

        /**
         * Constructor. The check box will be automatically resized
         * to fit the caption.
         *
         * @param caption The caption of the check box.
         * @param marked True if the check box is selected, false otherwise.
         */
        virtual void init(const std::string &caption, bool selected = false);

        /**
         * Destructor.
         */
        virtual ~ZCheckBox();

        /**
         * Checks if the check box is selected.
         *
         * @return True if the check box is selected, false otherwise.
         * @see setSelected
         */
        bool isSelected() const;

        /**
         * Sets the check box to be selected or not.
         *
         * @param selected True if the check box should be set as selected.
         * @see isSelected
         */
        void setSelected(bool selected);

        /**
         * Gets the caption of the check box.
         *
         * @return The caption of the check box.
         * @see setCaption
         */
        const std::string &getCaption() const;

        /**
         * Sets the caption of the check box. It's advisable to call
         * adjustSize after setting of the caption to adjust the
         * check box's size to fit the caption.
         *
         * @param caption The caption of the check box.
         * @see getCaption, adjustSize
         */
        void setCaption(const std::string& caption);

        /**
         * Adjusts the check box's size to fit the caption.
         */
        void adjustSize();


        // Inherited from Widget

        virtual void draw_item(std::shared_ptr<Graphics> graphics);


        // Inherited from KeyListener

        virtual void keyPressed(KeyEvent& keyEvent);


        // Inherited from MouseListener

        virtual void mouseClicked(MouseEvent& mouseEvent);

        virtual void mouseDragged(MouseEvent& mouseEvent);


    protected:
        /**
         * Draws the box of the check box.
         *
         * @param graphics A Graphics object to draw with.
         */
        virtual void drawBox(std::shared_ptr<Graphics> graphics);

        /**
         * Toggles the check box between being selected and
         * not being selected.
         */
        virtual void toggleSelected();

        /**
         * True if the check box is selected, false otherwise.
         */
        bool mSelected;

        /**
         * Holds the caption of the check box.
         */
        std::string mCaption;
    };
}

#endif //GUICHAN_ZCHECKBOX_H
