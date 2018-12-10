//
// Created by tatiana on 6/14/2018.
//

#ifndef ZERL_BODYDEF_H
#define ZERL_BODYDEF_H

#include <string>
#include <memory>

namespace properties
{
    struct BodySectionDef;
}

namespace properties {
    struct BodyDef {
    public:
        std::string ID;
        std::shared_ptr<BodySectionDef> MainBody;
    };
}

#endif //ZERL_BODYDEF_H
