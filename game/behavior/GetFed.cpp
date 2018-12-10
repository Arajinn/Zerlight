//
// Created by tatiana on 6/15/2018.
//

#include "GetFed.h"
#include "Condition.h"
#include "PathToNeedGoal.h"
#include "game/core/Character.h"

namespace behavior
{
    GetFed::GetFed()
        :Sequence()
    {
        addNode(std::shared_ptr<Condition>(new Condition(&GetFed::noGrip)));
        addNode(std::shared_ptr<PathToNeedGoal>(new PathToNeedGoal));
    }

    GetFed::~GetFed()
    {

    }

    TaskResult GetFed::noGrip(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return (character->abilityLevel(game::BodyFunction::Grip)>0.0f) ? TaskResult::Failure : TaskResult::Success;
    }
}