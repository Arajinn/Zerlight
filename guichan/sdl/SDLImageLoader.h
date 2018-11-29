//
// Created by tatiana on 28.09.18.
//

#ifndef GUICHAN_SDLIMAGELOADER_H
#define GUICHAN_SDLIMAGELOADER_H

#include "guichan/gui/ImageLoader.h"

class SDL_Surface;

namespace gui
{
    class ZImage;
}

namespace sdl_interface {
    /**
     * SDL implementation of ImageLoader.
     */
    class SDLImageLoader : public gui::ImageLoader{
    public:

        // Inherited from ImageLoader

        virtual std::shared_ptr<gui::ZImage> load(const std::string& filename, bool convertToDisplayFormat = true);

    protected:
        virtual std::shared_ptr<SDL_Surface> loadSDLSurface(const std::string& filename);
        virtual std::shared_ptr<SDL_Surface> convertToStandardFormat(std::shared_ptr<SDL_Surface> surface);
    };
}

#endif //GUICHAN_SDLIMAGELOADER_H
