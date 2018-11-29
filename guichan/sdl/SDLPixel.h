//
// Created by tatiana on 27.09.18.
//

#ifndef GUICHAN_SDLPIXEL_H
#define GUICHAN_SDLPIXEL_H

#include "guichan/gui/ZColor.h"

#include <memory>

class SDL_Surface;
class SDL_PixelFormat;

namespace sdl_interface
{
    /**
    * Checks a pixels color of an SDL_Surface.
    *
    * @param surface an SDL_Surface where to check for a pixel color.
    * @param x the x coordinate on the surface.
    * @param y the y coordinate on the surface.
    * @return a color of a pixel.
    */
    const gui::ZColor SDLgetPixel(std::shared_ptr<SDL_Surface> surface, int x, int y);

    /**
     * Puts a pixel on an SDL_Surface.
     *
     * @param x the x coordinate on the surface.
     * @param y the y coordinate on the surface.
     * @param color the color the pixel should be in.
     */
    void SDLputPixel(std::shared_ptr<SDL_Surface> surface, int x, int y, const gui::ZColor& color);

    /**
     * Puts a pixel on an SDL_Surface with alpha
     *
     * @param x the x coordinate on the surface.
     * @param y the y coordinate on the surface.
     * @param color the color the pixel should be in.
     */
    void SDLputPixelAlpha(std::shared_ptr<SDL_Surface> surface, int x, int y, const gui::ZColor& color);

    /**
     * Blends two 32 bit colors together.
     *
     * @param src the source color.
     * @param dst the destination color.
     * @param a alpha.
     */
    unsigned int SDLAlpha32(unsigned int src, unsigned int dst, unsigned char a);

    /**
     * Blends two 16 bit colors together.
     *
     * @param src the source color.
     * @param dst the destination color.
     * @param a alpha.
     */
    unsigned short SDLAlpha16(unsigned short src, unsigned short dst, unsigned char a, const SDL_PixelFormat *f);
}


#endif //GUICHAN_SDLPIXEL_H
