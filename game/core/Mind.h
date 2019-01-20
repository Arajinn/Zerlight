//
// Created by tatiana on 6/15/2018.
//

#ifndef ZERL_MIND_H
#define ZERL_MIND_H

#include <memory>

namespace game
{
    class Character;
}

namespace game {
    class Mind : public std::enable_shared_from_this<Mind>{
    public:
        Mind();
        ~Mind();

        static std::shared_ptr<Mind> create(std::shared_ptr<Character> parent);

        void adjustHappiness(float delta);

        float combatValue() const;

        bool isUpset() const;
    private:
        void init(std::shared_ptr<Character> parent);

        std::shared_ptr<Character> owner;
        float mHappinessLevel;
    };
}

#endif //ZERL_MIND_H
