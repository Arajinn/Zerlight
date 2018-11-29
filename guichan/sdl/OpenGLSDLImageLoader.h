//
// Created by tatiana on 28.09.18.
//

#ifndef GUICHAN_OPENGLSDLIMAGELOADER_H
#define GUICHAN_OPENGLSDLIMAGELOADER_H

#include "guichan/sdl/SDLImageLoader.h"

namespace sdl_interface {
    /**
     * OpenGL ImageLoader that loads images with SDL.
     */
    class OpenGLSDLImageLoader : public SDLImageLoader{
    public:

        // Inherited from ImageLoader

        virtual std::shared_ptr<gui::ZImage> load(const std::string& filename, bool convertToDisplayFormat = true);
    };
}

#endif //GUICHAN_OPENGLSDLIMAGELOADER_H
