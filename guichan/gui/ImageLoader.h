//
// Created by tatiana on 26.09.18.
//

#ifndef GUICHAN_IMAGELOADER_H
#define GUICHAN_IMAGELOADER_H

#include <memory>

namespace gui {
    class ZImage;

    /**
     * Abstract class for providing functions for loading images.
     *
     * Guichan contains implementations of ImageLoader for common
     * libraries like the Allegro library, the HGE library,
     * he OpenLayer library, and the SDL library.
     * To make Guichan usable with other libraries, an ImageLoader
     * class must be implemented.
     *
     * To make Guichan use an image loader, the image loader needs
     * to be passed to the Image class using the static method
     * Image::setImageLoader.
     *
     * @see Image::setImageLoader, Image::getImageLoader,
     *      AllegroImageLoader, HGEImageLoader, OpenLayerImageLoader,
     *      OpenGLAllegroImageLoader, OpenGLSDLImageLoader,
     *      SDLImageLoader
     * @since 0.1.0
     */
    class ImageLoader {
    public:

        /**
         * Destructor.
         */
        virtual ~ImageLoader();

        /**
         * Loads an image.
         *
         * NOTE: The functions Image::getPixel and Image::putPixel
         *       are only guaranteed to work before an image has
         *       been converted to display format.
         *
         * @param filename The filename of the image to load.
         * @param convertToDisplayFormat True if the image should be converted
         *                               to display, false otherwise.
         */
        virtual std::shared_ptr<ZImage> load(const std::string& filename, bool convertToDisplayFormat = true) = 0;
    };
}

#endif //GUICHAN_IMAGELOADER_H
