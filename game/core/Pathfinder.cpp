//
// Created by tatiana on 26.03.2018.
//

#include "Pathfinder.h"
#include "defines.h"
#include "GameManager.h"
#include "Character.h"
#include "PriorityQueue.h"
#include "game/map/Region.h"
#include "game/map/Map.h"
#include "game/map/MapCell.h"
#include "game/map/NavGraphNode.h"

#include <limits>
#include <algorithm>
#include <math.h>
#include <iostream>

namespace game
{
    Pathfinder::Pathfinder(std::shared_ptr<Character> parent)
            :owner(parent)
            ,mPathing(false)
    {
        mPathing=false;

        vectorArrayFindPath.resize(10);
        vectorArrayFindPath[0]=map::vector3(-1,-1,0);
        vectorArrayFindPath[1]=map::vector3(0,-1,0);
        vectorArrayFindPath[2]=map::vector3(1,-1,0);
        vectorArrayFindPath[3]=map::vector3(-1,0,0);
        vectorArrayFindPath[4]=map::vector3(1,0,0);
        vectorArrayFindPath[5]=map::vector3(-1,1,0);
        vectorArrayFindPath[6]=map::vector3(0,1,0);
        vectorArrayFindPath[7]=map::vector3(1,1,0);
        vectorArrayFindPath[8]=map::vector3(0,0,1);
        vectorArrayFindPath[9]=map::vector3(0,0,-1);
    }

    Pathfinder::~Pathfinder()
    {

    }

    bool Pathfinder::pathing() const
    {
        return (!mPath.empty());
    }

    const map::vector3& Pathfinder::nextPosition() const
    {
        return mPath.at(0);
    }

    map::vector3 Pathfinder::endPosition() const
    {
        if (mNavPath.size()<=2)
            return mEndPos;

        int index=mNavPath.size()-2;
        auto pos=mNavPath.at(index)->closestPoint(mEndPos);
        for (int i=index-1;i>0;i--)
            pos=mNavPath[i]->closestPoint(pos);

        return pos;
    }

    bool Pathfinder::atEndPosition(const map::vector3& pos) const
    {
        if (mAdjacentPos)
            return Pathfinder::isAdjacent(pos,mEndPos);

        return (pos==mEndPos);
    }

    bool Pathfinder::isAdjacentToNavGraph(const map::vector3& pos, unsigned int startGraphID)
    {
        auto map=WORLD_MAP;
        std::vector<std::shared_ptr<map::MapCell>> mapCellArray(4);
        mapCellArray[0]=map->cell(pos.x()-1,pos.y(),pos.z());
        mapCellArray[1]=map->cell(pos.x()+1,pos.y(),pos.z());
        mapCellArray[2]=map->cell(pos.x(),pos.y()-1,pos.z());
        mapCellArray[3]=map->cell(pos.x(),pos.y()+1,pos.z());

        for (auto mapCell : mapCellArray)
        {
            if (mapCell!=nullptr)
            {
                if ((mapCell->navGraphNode()!= nullptr) && (mapCell->navGraphNode()->navGraphID()==startGraphID))
                    return true;
            }
        }

        return false;
    }

    bool Pathfinder::canReach(const map::vector3& start, const map::vector3& end, bool adjacent) const
    {
        auto map=WORLD_MAP;

        auto start_cell=map->cell(start);
        if (start_cell== nullptr)
            return false;
        if (start_cell->navGraphNode()== nullptr)
            return false;

        auto end_cell=map->cell(end);
        if (end_cell== nullptr)
            return false;

        if ((!adjacent) && (end_cell->navGraphNode()== nullptr))
            return false;

        if ((adjacent) && (!Pathfinder::isAdjacentToNavGraph(end,start_cell->navGraphNode()->navGraphID())))
            return false;

        if ((!start_cell->willSuffocate()) && (!start_cell->hasLava()) && (end_cell->willSuffocate()) && (end_cell->hasLava()))
            return false;

        if ((!adjacent) && (start_cell->navGraphNode()->navGraphID()!=end_cell->navGraphNode()->navGraphID()))
            return false;

        return true;
    }

    bool Pathfinder::has2DLOS(map::vector3 start, map::vector3 end, std::vector<map::vector3> &path)
    {
        if (start.z()!=end.z())
            return false;

        int x1=start.x();
        int y1=start.y();

        int x2=end.x();
        int y2=end.y();

        int num1=std::abs(x1-x2);
        int num2=std::abs(y1-y2);
        int num3=(x1>=x2)?-1:1;
        int num4=(y1>=y2)?-1:1;
        int num5=num1-num2;

        std::vector<map::vector3> positions_list;
        auto map=WORLD_MAP;

        while ((x1!=x2) || (y1!=y2))
        {
            start.set_x(x1);
            start.set_y(y1);

            positions_list.push_back(start);

            if (!map->isWalkable(start))
                return false;

            int num6=2*num5;
            if (num6>-num2)
            {
                num5-=num2;
                x1+=num3;
            }

            if (num6<num1)
            {
                num5+=num1;
                y1+=num4;
            }
        }

        positions_list.push_back(end);
        path=positions_list;
        return true;
    }

    bool Pathfinder::isAdjacent(std::shared_ptr<map::NavGraphNode> node, const map::vector3& pos)
    {
        if (node->level()!=pos.z())
            return false;

        if ((pos.y()>=node->area().top()) && (pos.y()<=node->area().bottom()) && (((pos.x()+1)==node->area().left()) || ((pos.x()-1)==node->area().right())))
            return true;

        if ((pos.x()<node->area().left()) || (pos.x()>node->area().right()))
            return false;

        if (((pos.y()+1)==node->area().top()) || ((pos.y()-1)==node->area().bottom()))
            return true;

        return false;
    }

    bool Pathfinder::isAdjacent(const map::vector3& pos1, const map::vector3& pos2)
    {
        if (pos1.z()==pos2.z())
            return Pathfinder::isAdjacent2D(pos1,pos2);
        else
            return false;
    }

    bool Pathfinder::isAdjacent2D(const map::vector3& pos1, const map::vector3& pos2)
    {
        if ((pos1.x()==pos2.x()) && (std::abs(pos1.y()-pos2.y())==1))
            return true;
        else if ((pos1.y()==pos2.y()) && (std::abs(pos1.x()-pos2.x())==1))
            return true;
        else
            return false;
    }

    bool Pathfinder::containsAdjacentSpot(std::shared_ptr<map::NavGraphNode> node, const map::vector3& pos)
    {
        std::vector<map::vector3> array(4);
        array[0]=map::vector3(-1,0,0);
        array[1]=map::vector3(0,-1,0);
        array[2]=map::vector3(1,0,0);
        array[3]=map::vector3(0,1,0);

        for (auto shift : array)
        {
            if (node->contains(shift+pos))
                return true;
        }

        return false;
    }

    bool Pathfinder::isDirectlyAboveOrBelow(const map::vector3& pos1, const map::vector3& pos2)
    {
        if ((pos1.x()!=pos2.x()) || (pos1.y()!=pos2.y()))
            return false;

        return (std::abs(pos1.z()-pos2.z())==1);
    }

    bool Pathfinder::isDiagonal(const map::vector3& pos1, const map::vector3& pos2)
    {
        if (pos1.z()!=pos2.z())
            return false;

        return ((std::abs(pos1.x()-pos2.x())==1) && ((std::abs(pos1.y()-pos2.y()))==1));
    }

    bool Pathfinder::findNavPath(const map::vector3& start, const map::vector3& end, bool adjacent)
    {
        if(!this->canReach(start,end,adjacent))
            return false;

        auto map=WORLD_MAP;

        auto start_cell=map->cell(start);
        if (start_cell== nullptr)
            return false;
        if (start_cell->navGraphNode()== nullptr)
            return false;

        auto end_cell=map->cell(end);
        if (end_cell== nullptr)
            return false;
        if (!adjacent && (end_cell->navGraphNode()== nullptr))
            return false;

        mNavPath.clear();
        mEndPos=end;
        mAdjacentPos=adjacent;
        mPathing=false;

        if (mAdjacentPos)
        {
            if (!Pathfinder::isAdjacentToNavGraph(mEndPos,start_cell->navGraphNode()->navGraphID()))
                return false;

            if (Pathfinder::isAdjacent(start_cell->navGraphNode(),mEndPos) || Pathfinder::containsAdjacentSpot(start_cell->navGraphNode(),mEndPos))
            {
                mNavPath.clear();
                mNavPath.push_back(start_cell->navGraphNode());
                return this->findPath(start,mEndPos);
            }
        }
        else
        {
            if (start_cell->navGraphNode()->nodeNavID()==end_cell->navGraphNode()->nodeNavID())
            {
                mNavPath.clear();
                mNavPath.push_back(start_cell->navGraphNode());
                return this->findPath(start,mEndPos);
            }
            else if (start_cell->navGraphNode()->navGraphID()!=end_cell->navGraphNode()->navGraphID())
                return false;
        }

        std::vector<PriorityQueueNavGraphPathNode> priority_queue;
        std::vector<std::shared_ptr<NavGraphPathNode>> sortedDictionary1,sortedDictionary2;

        std::shared_ptr<NavGraphPathNode> key1=std::make_shared<NavGraphPathNode>(start_cell->navGraphNode());
        key1->distanceCost=0.0f;
        key1->heuristicCost=map::vector3::distance(start,end)+float(std::abs(start.x()*end.y()-start.y()*end.z()-start.z()*end.x()))/1000.0f;
        key1->closestPoint=start;

        std::shared_ptr<NavGraphPathNode> other= (end_cell==nullptr) ? nullptr : std::make_shared<NavGraphPathNode>(end_cell->navGraphNode());

        priority_queue.push_back(PriorityQueueNavGraphPathNode(key1->distanceCost+key1->heuristicCost,key1));

        sortedDictionary1.push_back(key1);

        int k=0;
        while (!sortedDictionary1.empty())
        {
            k++;

            std::sort(priority_queue.begin(),priority_queue.end(),
                    [](const PriorityQueueNavGraphPathNode& left,const PriorityQueueNavGraphPathNode& right)
            {
                return left.priority<right.priority;
            });

            std::shared_ptr<NavGraphPathNode> key2=priority_queue.front().node;
            priority_queue.erase(priority_queue.begin());
            {
                auto iter = std::find_if(sortedDictionary1.begin(), sortedDictionary1.end(),
                        [&key2](std::shared_ptr<NavGraphPathNode> const &elem)
                {
                    return (key2->node->nodeNavID() == elem->node->nodeNavID());
                });

                if (iter != sortedDictionary1.end())
                    sortedDictionary1.erase(iter);
            }

            sortedDictionary2.push_back(key2);

            auto connections=key2->node->connections();
            for (auto connection : connections)
            {
                std::shared_ptr<NavGraphPathNode> pathfinderNode=std::make_shared<NavGraphPathNode>(connection);
                pathfinderNode->closestPoint=pathfinderNode->node->closestPoint(key2->closestPoint);
                pathfinderNode->distanceCost=key2->distanceCost+map::vector3::distance(pathfinderNode->closestPoint,key2->closestPoint);
                pathfinderNode->heuristicCost=map::vector3::distance(pathfinderNode->closestPoint,mEndPos);

                auto iter2 = std::find_if(sortedDictionary2.begin(), sortedDictionary2.end(),
                        [&pathfinderNode](std::shared_ptr<NavGraphPathNode> const &elem)
                {
                    return pathfinderNode->node->nodeNavID() == elem->node->nodeNavID();
                });

                // node not contain int sortedDictionary2
                if (iter2 == sortedDictionary2.end())
                {
                    pathfinderNode->next=key2;

                    auto iter1 = std::find_if(sortedDictionary1.begin(), sortedDictionary1.end(),
                            [&pathfinderNode](std::shared_ptr<NavGraphPathNode> const &elem)
                    {
                        return pathfinderNode->node->nodeNavID() == elem->node->nodeNavID();
                    });

                    // node not contain int sortedDictionary1
                    if (iter1 == sortedDictionary1.end())
                    {
                        if ((pathfinderNode->equals(other)) || (mAdjacentPos && Pathfinder::isAdjacent(pathfinderNode->node,mEndPos)))
                        {
                            if (!start_cell->willSuffocate() && !start_cell->hasLava() && !pathfinderNode->node->hasDryLand())
                                return false;

                            this->createNavPath(pathfinderNode);
                            return this->findPath(start,this->endPosition());
                        }

                        priority_queue.push_back(PriorityQueueNavGraphPathNode(pathfinderNode->distanceCost+pathfinderNode->heuristicCost,pathfinderNode));
                        sortedDictionary1.push_back(pathfinderNode);
                    }
                    else
                    {
                        const auto index=std::distance(sortedDictionary1.begin(),iter1);
                        if (sortedDictionary1.at(index)->distanceCost > pathfinderNode->distanceCost)
                        {
                            {
                                auto nodeId = sortedDictionary1.at(index)->node->nodeNavID();
                                auto iter = std::find_if(priority_queue.begin(), priority_queue.end(),
                                                         [&nodeId](PriorityQueueNavGraphPathNode const &elem)
                                                         {
                                                             return elem.node->node->nodeNavID() == nodeId;
                                                         });

                                if (iter != priority_queue.end())
                                    priority_queue.erase(iter);
                            }

                            priority_queue.push_back(PriorityQueueNavGraphPathNode(pathfinderNode->distanceCost+pathfinderNode->heuristicCost,pathfinderNode));
                            sortedDictionary1.push_back(pathfinderNode);
                        }
                    }
                }
            }
        }

        if (end_cell->navGraphNode()!=nullptr)
        {
            int t=0;
        }
        else if (mAdjacentPos)
        {
            int t=0;
        }

        return false;
    }

    bool Pathfinder::stepPath()
    {
        if (mPath.empty())
            return false;

        if (mNavPath.empty())
            return false;

        auto pos=mPath.at(0);

        mPath.erase(mPath.begin());
        if (mNavPath.size()>2)
        {
            if (!mNavPath.at(1)->contains(pos))
                return true;

            mNavPath.erase(mNavPath.begin());

            if (mNavPath.at(0)->connectionsCount()==0)
                this->findNavPath(pos,mEndPos,mAdjacentPos);
        }

        return mPathing;
    }

    void Pathfinder::clearPath()
    {
        mPathing=false;
    }

    void Pathfinder::createPath(std::shared_ptr<PathfinderNode> end)
    {
        mPath.clear();

        std::shared_ptr<PathfinderNode> pathfinderNode=end;
        std::shared_ptr<PathfinderNode> next;

        while (pathfinderNode!= nullptr)
        {
            next=pathfinderNode->next;
            std::vector<map::vector3> path;
            while ((next!= nullptr) && (this->has2DLOS(pathfinderNode->position,next->position,path)))
                next=next->next;

            int path_size=path.size();
            if (path_size>0)
            {
                for (int i = 0; i < path_size; i++)
                {
                    if ((mPath.size() > 1) && (Pathfinder::isDiagonal(mPath.at(1), path.at(i))))
                        mPath[0] = path.at(i);
                    else
                        mPath.insert(mPath.begin(), path.at(i));
                }
            }
            else if ((mPath.size()>1) && (Pathfinder::isDiagonal(mPath.at(1),pathfinderNode->position)))
                mPath[0]=pathfinderNode->position;
            else
                mPath.insert(mPath.begin(),pathfinderNode->position);

            pathfinderNode=next;
        }

        mPath.erase(mPath.begin());
        mPathing=true;
    }

    void Pathfinder::createNavPath(std::shared_ptr<NavGraphPathNode> node)
    {
       mNavPath.clear();

       while (node!=nullptr)
       {
           mNavPath.push_back(node->node);
           node=node->next;
       }
    }

    void Pathfinder::startNewPathing(const map::vector3& end_pos)
    {
        mPathing=true;
        mEndPos=end_pos;
    }

    bool Pathfinder::findPath(const map::vector3& start, const map::vector3& end)
    {
        mPath.clear();

        if (((start==end) && (!mAdjacentPos)) || ((mAdjacentPos) && (mEndPos==end) && (Pathfinder::isAdjacent(end,start))))
        {
            mPath.push_back(start);
            mPathing=true;
            return true;
        }

        auto map=WORLD_MAP;

        std::vector<PriorityQueuePathfinderNode> priority_queue;
        std::vector<std::shared_ptr<PathfinderNode>> sortedDictionary1,sortedDictionary2;

        std::shared_ptr<PathfinderNode> key1=std::make_shared<PathfinderNode>(start);
        key1->distanceCost=0.0f;
        key1->heuristicCost=map::vector3::distance(start,end)+float(std::abs(start.x()*end.y()-start.y()*end.z()-start.z()*end.x()))/1000.0f;

        priority_queue.push_back(PriorityQueuePathfinderNode(key1->distanceCost+key1->heuristicCost,key1));

        sortedDictionary1.push_back(key1);

        auto navGraphNode1=mNavPath.at(0);
        auto navGraphNode2=mNavPath.at(0);

        if (mNavPath.size()>1)
            navGraphNode2=mNavPath.at(1);

        int k=0;
        while (!sortedDictionary1.empty())
        {
            k++;

            std::sort(priority_queue.begin(),priority_queue.end(),[](const PriorityQueuePathfinderNode& left,const PriorityQueuePathfinderNode& right)
            {
                return left.priority<right.priority;
            });

            std::shared_ptr<PathfinderNode> key2=priority_queue.front().node;
            priority_queue.erase(priority_queue.begin());
            {
                auto iter = std::find_if(sortedDictionary1.begin(), sortedDictionary1.end(),
                                         [&key2](std::shared_ptr<PathfinderNode> const &elem)
                                         {
                                             return (key2->position == elem->position);
                                         });
                if (iter != sortedDictionary1.end())
                    sortedDictionary1.erase(iter);
            }

            sortedDictionary2.push_back(key2);

            for (auto shift : vectorArrayFindPath)
            {
                std::shared_ptr<PathfinderNode> pathfinderNode=std::make_shared<PathfinderNode>(key2->position+shift);
                if (!navGraphNode1->contains(pathfinderNode->position))
                {
                    if (navGraphNode2->contains2D(pathfinderNode->position))
                    {
                        if (pathfinderNode->position.z()!=navGraphNode2->level())
                        {
                            bool flag=false;
                            pathfinderNode->position.set_z(navGraphNode2->level());
                            auto cell1=map->cell(key2->position);
                            auto cell2=map->cell(pathfinderNode->position);
                            if (Pathfinder::isAdjacent2D(key2->position,pathfinderNode->position) && (((cell1->hasRamp()) && (pathfinderNode->position.z()<key2->position.z()))
                                    || ((cell2->hasRamp()) && (pathfinderNode->position.z()>key2->position.z()))))
                                flag=true;

                            if (!flag)
                                continue;
                        }
                        else if (key2->position.z()!=navGraphNode2->level())
                        {
                            bool flag=false;
                            auto cell1=map->cell(key2->position);
                            auto cell2=map->cell(pathfinderNode->position);
                            if (Pathfinder::isDirectlyAboveOrBelow(key2->position,pathfinderNode->position) && (((cell1->hasStairs()) && (pathfinderNode->position.z()<key2->position.z()))
                                    || ((cell2->hasStairs()) && (pathfinderNode->position.z()>key2->position.z()))))
                                flag=true;

                            if (!flag)
                                continue;
                        }
                    }
                    else
                        continue;
                }

                pathfinderNode->heuristicCost=map::vector3::distance(pathfinderNode->position,end)+float(std::abs(pathfinderNode->position.x()*end.y()
                        -pathfinderNode->position.y()*end.z()-pathfinderNode->position.z()*end.x()))/1000.0f;
                pathfinderNode->distanceCost=key2->distanceCost+map::vector3::distance(pathfinderNode->position,key2->position);

                auto iter2 = std::find_if(sortedDictionary2.begin(), sortedDictionary2.end(),
                        [&pathfinderNode](std::shared_ptr<PathfinderNode> const &elem)
                        {
                            return pathfinderNode->position == elem->position;
                        });

                // node not contain int sortedDictionary2
                if (iter2 == sortedDictionary2.end())
                {
                    pathfinderNode->next=key2;

                    auto iter1 = std::find_if(sortedDictionary1.begin(), sortedDictionary1.end(),
                            [&pathfinderNode](std::shared_ptr<PathfinderNode> const &elem)
                    {
                        return pathfinderNode->position == elem->position;
                    });

                    // node not contain int sortedDictionary1
                    if (iter1 == sortedDictionary1.end())
                    {
                        if ((mAdjacentPos) && (mEndPos==end))
                        {
                            if (Pathfinder::isAdjacent(end,pathfinderNode->position))
                            {
                                this->createPath(pathfinderNode);
                                return true;
                            }
                        }
                        else if (pathfinderNode->equals(end))
                        {
                            this->createPath(pathfinderNode);
                            return true;
                        }

                        if ((navGraphNode1->contains2D(pathfinderNode->position)) || (navGraphNode2->contains(pathfinderNode->position)))
                        {
                            //std::cout << k << " " << pathfinderNode->position.x() << " " << pathfinderNode->position.y() << " "
                            //    << pathfinderNode->distanceCost << " " << pathfinderNode->heuristicCost << " " << pathfinderNode->distanceCost+pathfinderNode->heuristicCost << std::endl;
                            priority_queue.push_back(PriorityQueuePathfinderNode(pathfinderNode->distanceCost+pathfinderNode->heuristicCost,pathfinderNode));
                            sortedDictionary1.push_back(pathfinderNode);
                        }
                        else
                        {
                            sortedDictionary2.push_back(pathfinderNode);
                        }
                    }
                }
            }
        }

        return false;
    }

    bool Pathfinder::findDryLand(const map::vector3& start)
    {
        mNavPath.clear();
        mAdjacentPos=false;
        mPathing=false;

        auto map=WORLD_MAP;

        auto cell=map->cell(start);
        if ((cell==nullptr) || (cell->navGraphNode()==nullptr))
            return false;

        auto position=map::vector3_one;
        if (cell->navGraphNode()->hasDryLand(position))
        {
            mEndPos=position;
            mNavPath.push_back(cell->navGraphNode());
            return findPath(start,mEndPos);
        }

        std::vector<PriorityQueueNavGraphPathNode> priority_queue;
        std::vector<std::shared_ptr<NavGraphPathNode>> sortedDictionary1,sortedDictionary2;

        std::shared_ptr<NavGraphPathNode> key1=std::make_shared<NavGraphPathNode>(cell->navGraphNode());
        key1->distanceCost=0.0f;
        key1->heuristicCost=0.0f;
        key1->closestPoint=start;

        priority_queue.push_back(PriorityQueueNavGraphPathNode(key1->distanceCost+key1->heuristicCost,key1));

        sortedDictionary1.push_back(key1);

        while (!sortedDictionary1.empty())
        {
            std::sort(priority_queue.begin(),priority_queue.end(),[](const PriorityQueueNavGraphPathNode& left,const PriorityQueueNavGraphPathNode& right)
            {
                return left.priority<right.priority;
            });

            std::shared_ptr<NavGraphPathNode> key2=priority_queue.front().node;
            priority_queue.erase(priority_queue.begin());
            {
                auto iter = std::find_if(sortedDictionary1.begin(), sortedDictionary1.end(),
                                         [&key2](std::shared_ptr<NavGraphPathNode> const &elem)
                                         {
                                             return (key2->node->navGraphID() == elem->node->navGraphID());
                                         });

                if (iter != sortedDictionary1.end())
                    sortedDictionary1.erase(iter);
            }

            sortedDictionary2.push_back(key2);

            auto connections=key2->node->connections();
            for (auto connection : connections)
            {
                std::shared_ptr<NavGraphPathNode> pathfinderNode=std::make_shared<NavGraphPathNode>(connection);
                pathfinderNode->closestPoint=pathfinderNode->node->closestPoint(key2->closestPoint);
                pathfinderNode->distanceCost=key2->distanceCost+map::vector3::distance(pathfinderNode->closestPoint,key2->closestPoint);
                pathfinderNode->heuristicCost=0.0f;

                auto iter2 = std::find_if(sortedDictionary2.begin(), sortedDictionary2.end(),
                        [&pathfinderNode](std::shared_ptr<NavGraphPathNode> const &elem)
                {
                    return pathfinderNode->node->nodeNavID() == elem->node->nodeNavID();
                });

                // node not contain int sortedDictionary2
                if (iter2 == sortedDictionary2.end())
                {
                    pathfinderNode->next=key2;

                    auto iter1 = std::find_if(sortedDictionary1.begin(), sortedDictionary1.end(),
                            [&pathfinderNode](std::shared_ptr<NavGraphPathNode> const &elem)
                    {
                        return pathfinderNode->node->nodeNavID() == elem->node->nodeNavID();
                    });

                    // node not contain int sortedDictionary1
                    if (iter1 == sortedDictionary1.end())
                    {
                        if (connection->hasDryLand(position))
                        {
                            mEndPos=position;
                            createNavPath(pathfinderNode);
                            return findPath(start,endPosition());
                        }

                        priority_queue.push_back(PriorityQueueNavGraphPathNode(pathfinderNode->distanceCost+pathfinderNode->heuristicCost,
                                pathfinderNode));
                        sortedDictionary1.push_back(pathfinderNode);
                    }
                }
            }
        }

        return false;
    }
}