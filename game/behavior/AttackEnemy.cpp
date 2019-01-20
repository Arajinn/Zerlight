//
// Created by tatiana on 06.01.19.
//

#include "AttackEnemy.h"
#include "HandleTarget.h"

namespace behavior {
    AttackEnemy::AttackEnemy()
        :Sequence()
    {
        addNode(std::make_shared<HandleTarget>());
    }

    AttackEnemy::~AttackEnemy()
    {

    }
}