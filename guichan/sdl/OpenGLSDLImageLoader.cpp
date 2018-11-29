//
// Created by tatiana on 28.09.18.
//

#include "OpenGLSDLImageLoader.h"
#include "OpenGLImage.h"
#include "guichan/gui/Exception.h"

#include <SDL2/SDL_surface.h>

namespace sdl_interface
{
    std::shared_ptr<gui::ZImage> OpenGLSDLImageLoader::load(const std::string& filename, bool convertToDisplayFormat)
    {
        auto loadedSurface = loadSDLSurface(filename);

        if (loadedSurface == nullptr)
        {
            throw GCN_EXCEPTION(std::string("Unable to load image file: ") + filename);
        }

        auto surface = convertToStandardFormat(loadedSurface);
        loadedSurface=nullptr;

        if (surface == nullptr)
        {
            throw GCN_EXCEPTION(std::string("Not enough memory to load: ") + filename);
        }

        auto image = std::shared_ptr<OpenGLImage>(new OpenGLImage((unsigned int*)surface->pixels, surface->w, surface->h, convertToDisplayFormat));
        surface=nullptr;

        return image;
    }
}