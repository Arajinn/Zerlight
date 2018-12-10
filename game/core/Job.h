//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_JOB_H
#define ZERL_JOB_H

#include "game/map/vector3.h"
#include <memory>
#include <vector>

namespace game
{
    class Character;
    class Item;
}

namespace game {
    class Job {
    public:
        Job(const map::vector3& position);
        ~Job();

        const std::shared_ptr<Character>& requiredCharacter();

        bool isClaimed(std::shared_ptr<Item> item) const;

        virtual std::shared_ptr<Item> nextComponent() const;
    protected:
        virtual bool componentInPosition(std::shared_ptr<Item> item) const;
    private:
        std::shared_ptr<Character> mRequiredCharacter;
        std::shared_ptr<Character> mClaimedBy;

        std::vector<std::shared_ptr<Item>> mClaimedComponents;

        map::vector3 mPosition;
        bool mAdjacentComponents;
    };
}

#endif //ZERL_JOB_H
