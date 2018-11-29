//
// Created by tatiana on 28.09.18.
//

#include "SDLDeleters.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_mixer.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>

namespace sdl_interface
{
    void DeleteSdlWindow(SDL_Window* window)
    {
        std::cout << "Deleting SDL window\n";
        SDL_DestroyWindow(window);
    }

    void DeleteSdlSurface(SDL_Surface* surface)
    {
        std::cout << "Deleting SDL surface\n";
        SDL_FreeSurface(surface);
    }

    void DeleteSdlTexture(SDL_Texture* texture)
    {
        std::cout << "Deleting SDL texture\n";
        SDL_DestroyTexture(texture);
    }

    void DeleteMixChunk(Mix_Chunk* chunk)
    {
        std::cout << "Deleting SDL chunk\n";
        Mix_FreeChunk(chunk);
    }

    void DeleteMixMusic(Mix_Music* music)
    {
        std::cout << "Deleting SDL music\n";
        Mix_FreeMusic(music);
    }

    void DeleteGLUquadric(GLUquadric* obj)
    {
        std::cout << "Deleting GLU quadric\n";
        gluDeleteQuadric(obj);
    }
}