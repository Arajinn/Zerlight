//
// Created by tatiana on 28.09.18.
//

#include "SDLImageLoader.h"
#include "SDLImage.h"
#include "SDLDeleters.h"
#include "guichan/gui/Exception.h"

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

namespace sdl_interface
{
    std::shared_ptr<gui::ZImage> SDLImageLoader::load(const std::string& filename, bool convertToDisplayFormat)
    {
        auto loadedSurface = loadSDLSurface(filename);

        if (loadedSurface == nullptr)
        {
            throw GCN_EXCEPTION( std::string("Unable to load image file: ") + filename);
        }

        auto surface = convertToStandardFormat(loadedSurface);
        loadedSurface=nullptr;

        if (surface == nullptr)
        {
            throw GCN_EXCEPTION( std::string("Not enough memory to load: ") + filename);
        }

        std::shared_ptr<gui::ZImage> image = std::shared_ptr<SDLImage>(new SDLImage(surface, true));

        if (convertToDisplayFormat)
        {
            image->convertToDisplayFormat();
        }

        return image;
    }

    std::shared_ptr<SDL_Surface> SDLImageLoader::loadSDLSurface(const std::string& filename)
    {
        return std::shared_ptr<SDL_Surface>(IMG_Load(filename.c_str()),DeleteSdlSurface);
    }

    std::shared_ptr<SDL_Surface> SDLImageLoader::convertToStandardFormat(std::shared_ptr<SDL_Surface> surface)
    {
        Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
#else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
#endif

        auto colorSurface = std::shared_ptr<SDL_Surface>(SDL_CreateRGBSurface(SDL_SWSURFACE, 0, 0, 32, rmask, gmask, bmask, amask),
                                                         DeleteSdlSurface);

        std::shared_ptr<SDL_Surface> tmp = nullptr;

        if (colorSurface != nullptr)
        {
            tmp = std::shared_ptr<SDL_Surface>(SDL_ConvertSurface(surface.get(), colorSurface->format, SDL_SWSURFACE),DeleteSdlSurface);
            colorSurface=nullptr;
        }

        return tmp;
    }
}