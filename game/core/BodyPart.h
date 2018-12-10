//
// Created by tatiana on 6/14/2018.
//

#ifndef ZERL_BODYPART_H
#define ZERL_BODYPART_H

#include "game/core/enums.h"

#include <vector>
#include <memory>
#include <string>

namespace game
{
    class BodySection;
    class Item;
}

namespace properties
{
    struct BodyPartDef;
}

namespace game {
class BodyPart : public std::enable_shared_from_this<BodyPart>
{
    public:
        BodyPart();
        ~BodyPart();

        static std::shared_ptr<BodyPart> create(std::shared_ptr<BodySection> section,
                std::shared_ptr<const properties::BodyPartDef> aBodyPartDef,
                std::string nameModifier="", std::shared_ptr<BodyPart> containedBy=nullptr,
                std::shared_ptr<Item> prostheticPart=nullptr);

        const BodyFunction& function() const;
        void addFunction();
    private:
        void init(std::shared_ptr<BodySection> section, std::shared_ptr<const properties::BodyPartDef> aBodyPartDef,
                std::string nameModifier="", std::shared_ptr<BodyPart> containedBy=nullptr,
                std::shared_ptr<Item> prostheticPart=nullptr);

        std::shared_ptr<BodySection> parent_section;
        std::shared_ptr<BodyPart> parent_contained;
        std::shared_ptr<Item> mProstheticPart;
        BodyFunction mBodyFunction;

        BodyPartStatus mStatus;

        std::shared_ptr<const properties::BodyPartDef> mBodyPartDef;

        std::vector<std::shared_ptr<BodyPart>> mContainedParts;
    };
}

#endif //ZERL_BODYPART_H
