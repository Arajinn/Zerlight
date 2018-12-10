//
// Created by tatiana on 08.06.2018.
//

#include "GeneralWander.h"
#include "Condition.h"
#include "Action.h"
#include "game/core/Character.h"
#include "game/core/Pathfinder.h"
#include "game/map/MapCell.h"
#include "game/map/NavGraphNode.h"

namespace behavior
{
    GeneralWander::GeneralWander()
        :Selector()
    {
        addNode(std::shared_ptr<Condition>(new Condition(&GeneralWander::pathing)));
        addNode(std::shared_ptr<Condition>(new Condition(&GeneralWander::wait,1.0f)));
        addNode(std::shared_ptr<Action>(new Action(&GeneralWander::pathToNeighbourNode)));

        time=1.0f;
    }

    GeneralWander::~GeneralWander()
    {

    }

    TaskResult GeneralWander::pathing(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        return character->pathfinder()->pathing() ? TaskResult ::Success : TaskResult::Failure;
    }

    TaskResult GeneralWander::pathToNeighbourNode(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        auto navGraphNode=character->cell()->navGraphNode();
        if (navGraphNode==nullptr)
            return TaskResult::Failure;

        if (navGraphNode->connectionsCount()==0)
        {
            int t=0;
        }

        auto connection=navGraphNode->connections().front();
        auto newX=(connection->area().left()+connection->area().right())/2;
        auto newY=(connection->area().top()+connection->area().bottom())/2;
        character->pathTo(map::vector3(newX,newY,connection->level()),false);

        return TaskResult::Success;
    }

    TaskResult GeneralWander::wait(std::shared_ptr<game::Character> character, float dt, std::shared_ptr<Node> node)
    {
        node->time-=dt;

        if (node->time>0.0f)
            return TaskResult::Running;

        node->time+=1.0f;

        return TaskResult::Failure;
    }
}