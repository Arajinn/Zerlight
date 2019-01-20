//
// Created by tatiana on 23.12.18.
//

#include "Military.h"
#include "Squad.h"
#include "Formation.h"

namespace game {
    Military::Military()
    {

    }

    Military::~Military()
    {

    }

    void Military::addDefendTarget(std::shared_ptr<Character> enemy)
    {
        for (const auto& squad : mSquads)
        {
            const auto formation=squad->formation();
            if ((formation!=nullptr) && (formation->defendGnomes()) && (!squad->dontDefendTarget(enemy)))
                squad->addAttackTarget(enemy);
        }
    }

    void Military::removeAttackTarget(std::shared_ptr<Character> enemy)
    {
        for (const auto& squad : mSquads)
        {
            squad->removeAttackTarget(enemy);
            squad->removeDefendTarget(enemy);
        }
    }
}