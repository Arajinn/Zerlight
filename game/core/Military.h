//
// Created by tatiana on 23.12.18.
//

#ifndef ZERLIGHT_MILITARY_H
#define ZERLIGHT_MILITARY_H

#include <vector>
#include <memory>

namespace game
{
    class Character;
    class Squad;
}

namespace game {
    class Military {
    public:
        Military();
        ~Military();

        void addDefendTarget(std::shared_ptr<Character> enemy);
        void removeAttackTarget(std::shared_ptr<Character> enemy);
    private:
        std::vector<std::shared_ptr<Squad>> mSquads;
    };
}

#endif //ZERLIGHT_MILITARY_H
