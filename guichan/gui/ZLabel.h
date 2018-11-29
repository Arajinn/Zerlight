//
// Created by tatiana on 28.09.18.
//

#ifndef GUICHAN_ZLABEL_H
#define GUICHAN_ZLABEL_H

#include "guichan/gui/ZWidget.h"
#include "guichan/gui/Graphics.h"

namespace gui {
    /**
     * Implementation of a label capable of displaying a caption.
     */
    class ZLabel : public ZWidget{
    public:
        /**
         * Constructor.
         */
        ZLabel();
        virtual void init();

        /**
         * Constructor. The label will be automatically resized
         * to fit the caption.
         *
         * @param caption The caption of the label.
         */
        virtual void init(const std::string& caption);

        /**
         * Gets the caption of the label.
         *
         * @return The caption of the label.
         * @see setCaption
         */
        const std::string &getCaption() const;

        /**
         * Sets the caption of the label. It's advisable to call
         * adjustSize after setting of the caption to adjust the
         * label's size to fit the caption.
         *
         * @param caption The caption of the label.
         * @see getCaption, adjustSize
         */
        void setCaption(const std::string& caption);

        /**
         * Sets the alignment of the caption. The alignment is relative
         * to the center of the label.
         *
         * @param alignemnt The alignment of the caption of the label.
         * @see getAlignment, Graphics
         */
        void setAlignment(Graphics::Alignment alignment);

        /**
         * Gets the alignment of the caption. The alignment is relative to
         * the center of the label.
         *
         * @return The alignment of caption of the label.
         * @see setAlignmentm Graphics
         */
        Graphics::Alignment getAlignment() const;

        /**
         * Adjusts the label's size to fit the caption.
         */
        void adjustSize();


        // Inherited from Widget

        virtual void draw_item(std::shared_ptr<Graphics> graphics);

    protected:
        /**
         * Holds the caption of the label.
         */
        std::string mCaption;

        /**
         * Holds the alignment of the caption.
         */
        Graphics::Alignment mAlignment;
    };
}

#endif //GUICHAN_ZLABEL_H
