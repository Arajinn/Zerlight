//
// Created by tatiana on 07.12.18.
//

#ifndef ZERLIGHT_SPRITEDEF_H
#define ZERLIGHT_SPRITEDEF_H

#include <string>
#include <memory>

namespace gui
{
    class ZImage;
}

namespace properties
{
    struct SpriteDef
    {
        std::string SpriteID;
        std::string SpritePath;
        std::shared_ptr<gui::ZImage> Image;
    };
}

#endif //ZERLIGHT_SPRITEDEF_H
