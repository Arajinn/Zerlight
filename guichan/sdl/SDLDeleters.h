//
// Created by tatiana on 28.09.18.
//

#ifndef GUICHAN_SDLDELETERS_H
#define GUICHAN_SDLDELETERS_H

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_video.h>

class SDL_Window;
class SDL_Surface;
class SDL_Texture;

class Mix_Chunk;

class GLUquadric;

namespace sdl_interface
{
    void DeleteSdlWindow(SDL_Window* window);
    void DeleteSdlSurface(SDL_Surface* surface);
    void DeleteSdlTexture(SDL_Texture* texture);
    void DeleteMixChunk(Mix_Chunk* chunk);
    void DeleteMixMusic(Mix_Music* music);

    void DeleteGLUquadric(GLUquadric* obj);
}


#endif //GUICHAN_SDLDELETERS_H
