//
// Created by tatiana on 08.06.2018.
//

#include "Condition.h"

namespace behavior
{
    Condition::Condition(task_ptr ptr, float start)
        :Node()
        ,mCondition(ptr)
    {
        time=start;
    }

    Condition::~Condition()
    {

    }

    TaskResult Condition::execute(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return (*mCondition)(character,dt,node);
    }
}