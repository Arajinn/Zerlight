//
// Created by tatiana.polozova on 08.06.2018.
//

#include "Action.h"

namespace behavior
{
    Action::Action(task_ptr ptr, float start)
        :Node()
        ,mAction(ptr)
    {
        time=start;
    }

    Action::~Action()
    {

    }

    TaskResult Action::execute(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return (*mAction)(character,dt,node);
    }
}