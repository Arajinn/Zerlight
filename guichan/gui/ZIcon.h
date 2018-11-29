//
// Created by tatiana on 28.09.18.
//

#ifndef GUICHAN_ICON_H
#define GUICHAN_ICON_H

#include "guichan/gui/ZWidget.h"

namespace gui {
    class ZImage;

    /**
     * Implements an icon capable of displaying an image.
     */
    class ZIcon : public ZWidget{
    public:
        /**
         * Default constructor.
         */
        ZIcon();
        virtual void init();
        virtual void shutdown();

        /**
         * Constructor.
         *
         * @param filename The filename of the image to display.
         */
        virtual void init(const std::string& filename);

        /**
         * Constructor.
         *
         * @param image The image to display.
         */
        virtual void init(std::shared_ptr<const ZImage> image);

        /**
         * Descructor.
         */
        virtual ~ZIcon();

        /**
         * Sets the image to display. Existing image is freed automatically
         * if it was loaded internally.
         *
         * @param image The image to display.
         */
        void setImage(std::shared_ptr<const ZImage> image);

        /**
         * Gets the current image.
         *
         * @return The current image.
         */
        std::shared_ptr<const ZImage> getImage() const;


        // Inherited from Widget

        virtual void draw_item(std::shared_ptr<Graphics> graphics);

        virtual Rectangle getChildrenArea();

    protected:
        /**
         * The image to display.
         */
        std::shared_ptr<const ZImage> mImage;

        /**
         * True if the image has been loaded internally, false otherwise.
         * An image not loaded internally should not be deleted in the
         * destructor.
         */
        bool mInternalImage;
    };
}

#endif //GUICHAN_ICON_H
