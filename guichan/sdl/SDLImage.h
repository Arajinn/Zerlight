//
// Created by tatiana on 27.09.18.
//

#ifndef GUICHAN_SDLIMAGE_H
#define GUICHAN_SDLIMAGE_H

#include "guichan/gui/ZImage.h"

class SDL_Surface;

namespace sdl_interface {
    /**
     * SDL implementation of Image.
     */
    class SDLImage : public gui::ZImage{
    public:
        /**
         * Constructor. Load an image from an SDL surface.
         *
         * NOTE: The functions getPixel and putPixel are only guaranteed to work
         *       before an image has been converted to display format.
         *
         * @param surface the surface from which to load.
         * @param autoFree true if the surface should automatically be deleted.
         */
        SDLImage(std::shared_ptr<SDL_Surface> surface, bool autoFree);

        /**
         * Destructor.
         */
        virtual ~SDLImage();

        /**
         * Gets the SDL surface for the image.
         *
         * @return the SDL surface for the image.
         */
        virtual std::shared_ptr<SDL_Surface> getSurface() const;


        // Inherited from Image

        virtual void free();

        virtual int getWidth() const;

        virtual int getHeight() const;

        virtual gui::ZColor getPixel(int x, int y);

        virtual void putPixel(int x, int y, const gui::ZColor& color);

        virtual void convertToDisplayFormat();

    protected:
        std::shared_ptr<SDL_Surface> mSurface;
        bool mAutoFree;
    };
}

#endif //GUICHAN_SDLIMAGE_H
