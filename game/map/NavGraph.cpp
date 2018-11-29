//
// Created by tatiana.polozova on 2018-04-18.
//

#include "NavGraph.h"
#include "game/core/GameManager.h"
#include "Region.h"
#include "Map.h"
#include "MapCell.h"
#include "NavGraphNode.h"

#include <algorithm>
#include <iostream>

namespace map
{
    NavGraph::NavGraph()
    {
        auto map=GAME->region()->map();
        this->subdivideIntoAllowedAreas(map::rectangle2(0,0,map->mapWidth()-1,map->mapHeight()-1),0,map->mapDepth());
    }

    NavGraph::~NavGraph()
    {

    }

    void NavGraph::subdivideIntoAllowedAreas(map::rectangle2 bounds, int levelStart, int levelEnd)
    {
        auto map=GAME->region()->map();
        for (int level = levelStart; level < levelEnd; ++level)
        {
            for (int y = bounds.top(), yMax=bounds.bottom(); y <= yMax; ++y)
            {
                for (int x = bounds.left(), xMax=bounds.right(); x <= xMax; ++x)
                {
                    auto mapCell=map->cell(x,y,level);
                    if (mapCell->isWalkable())
                    {
                        if (mapCell->navGraphNode()==nullptr)
                        {
                            //std::cout << "new node " << x << " " << y << " " << level << std::endl;
                            auto newNode=this->growSquare(rectangle2(x,y,bounds.right()-x,bounds.bottom()-y),level);
                            x+=newNode->area().width();
                            //std::cout << "new node area:" << newNode->area().left() << " " << newNode->area().right() << " "
                            //    << newNode->area().top() << " " << newNode->area().bottom() << " " << level << std::endl;
                            this->mergeNavGraphIDs(newNode);
                        }
                        else
                        {
                            //std::cout << "old node " << x << " " << y << " " << level << std::endl;
                            x=mapCell->navGraphNode()->area().right();
                        }
                    }
                }
            }
        }
    }

    std::shared_ptr<NavGraphNode> NavGraph::growSquare(rectangle2 bounds, int level)
    {
        bool flag=true;

        int y=bounds.top();
        int x=bounds.left();

        int xMin=bounds.left();
        int yMin=bounds.top();

        int xMax=bounds.right();
        int yMax=bounds.bottom();

        std::shared_ptr<NavGraphNode> node=std::shared_ptr<NavGraphNode>(new NavGraphNode(GAME->nextNavID()));
        auto map=GAME->region()->map();
        map->cell(x,y,level)->setNavGraphNode(node);

        while (flag)
        {
            x++;
            if (x>xMax)
            {
                x=xMax;
                break;
            }

            for (int iy=yMin;iy<=y;iy++)
            {
                auto mapCell=map->cell(x,iy,level);
                if ((mapCell->isWalkable()==false) || (mapCell->navGraphNode()!= nullptr))
                {
                    x--;
                    flag=false;
                    break;
                }
            }

            if (flag)
            {
                y++;
                if (y>yMax)
                {
                    y=yMax;
                    x--;
                    break;
                }

                for (int ix=xMin;ix<=x;ix++)
                {
                    auto mapCell=map->cell(ix,y,level);
                    if ((mapCell->isWalkable()==false) || (mapCell->navGraphNode()!= nullptr))
                    {
                        x--;
                        y--;
                        flag=false;
                        break;
                    }
                }

                if (flag)
                {
                    for (int iy=yMin;iy<=y;iy++)
                        map->cell(x,iy,level)->setNavGraphNode(node);

                    for (int ix=xMin;ix<=x;ix++)
                        map->cell(ix,y,level)->setNavGraphNode(node);
                }
                else
                    break;
            }
            else
                break;
        }

        node->setArea(rectangle2(xMin,yMin,x-xMin,y-yMin),level);
        this->createConnections(node);
        return node;
    }

    void NavGraph::createConnections(std::shared_ptr<NavGraphNode> node)
    {
        auto map=GAME->region()->map();

        for (int iy=node->area().top(),yMax=node->area().bottom();iy<=yMax;iy++)
        {
            for (int ix=node->area().left(),xMax=node->area().right();ix<=xMax;ix++)
            {
                auto this_cell=map->cell(ix,iy,node->level());
                auto upper_cell=map->cell(ix,iy,node->level()+1);

                if (ix==node->area().left())
                {
                    auto mapCell=map->cell(node->area().left()-1,iy,node->level());
                    if (mapCell!= nullptr)
                    {
                        if (mapCell->hasSupport()==true)
                        {
                            auto temp_cell=map->cell(node->area().left()-1,iy,node->level()-1);
                            if (temp_cell!= nullptr)
                            {
                                if ((temp_cell->isWalkable()==true) && (temp_cell->navGraphNode()!= nullptr) && (this_cell->hasRamp()==true))
                                    mapCell=temp_cell;
                            }
                        }
                        else if (mapCell->hasFloor()==false)
                        {
                            auto temp_cell=map->cell(node->area().left()-1,iy,node->level()+1);
                            if (temp_cell!=nullptr) {
                                if ((upper_cell->hasSupport()==true) && (temp_cell->isWalkable()==true) && (temp_cell->navGraphNode()!=nullptr) && (temp_cell->hasRamp()==true))
                                    mapCell=temp_cell;
                            }
                        }

                        if (mapCell->navGraphNode()!=nullptr)
                        {
                            NavGraphNode::makeConnection(node,mapCell->navGraphNode());
                        }
                    }
                }

                if (ix==node->area().right())
                {
                    auto mapCell=map->cell(node->area().right()+1,iy,node->level());
                    if (mapCell!=nullptr)
                    {
                        if (mapCell->hasSupport()==true)
                        {
                            auto temp_cell=map->cell(node->area().right()+1,iy,node->level()-1);
                            if (temp_cell!= nullptr)
                            {
                                if ((temp_cell->isWalkable()==true) && (temp_cell->navGraphNode()!= nullptr) && (this_cell->hasRamp()==true))
                                    mapCell=temp_cell;
                            }
                        }
                        else if (mapCell->hasFloor()==false)
                        {
                            auto temp_cell=map->cell(node->area().right()+1,iy,node->level()+1);
                            if (temp_cell!= nullptr)
                            {
                                if ((upper_cell->hasSupport()==true) && (temp_cell->isWalkable()==true) && (temp_cell->navGraphNode()!=nullptr) && (temp_cell->hasRamp()==true))
                                    mapCell=temp_cell;
                            }
                        }

                        if (mapCell->navGraphNode()!= nullptr)
                        {
                            NavGraphNode::makeConnection(node, mapCell->navGraphNode());
                        }
                    }
                }

                if (iy==node->area().top())
                {
                    auto mapCell=map->cell(ix,node->area().top()-1,node->level());
                    if (mapCell!= nullptr)
                    {
                        if (mapCell->hasSupport()==true)
                        {
                            auto temp_cell=map->cell(ix,node->area().top()-1,node->level()-1);
                            if (temp_cell!=nullptr)
                            {
                                if ((temp_cell->isWalkable()==true) && (temp_cell->navGraphNode()!= nullptr) && (this_cell->hasRamp()))
                                    mapCell=temp_cell;
                            }
                        }
                        else if (mapCell->hasFloor()==false)
                        {
                            auto temp_cell=map->cell(ix,node->area().top()-1,node->level()+1);
                            if (temp_cell!=nullptr)
                            {
                                if ((upper_cell->hasSupport() == true) && (temp_cell->isWalkable()==true) && (temp_cell->navGraphNode()!=nullptr) && (temp_cell->hasRamp()==true))
                                    mapCell=temp_cell;
                            }
                        }

                        if (mapCell->navGraphNode()!= nullptr)
                        {
                            NavGraphNode::makeConnection(node, mapCell->navGraphNode());
                        }
                    }
                }

                if (iy==node->area().bottom())
                {
                    auto mapCell=map->cell(ix,node->area().bottom()+1,node->level());
                    if (mapCell!= nullptr)
                    {
                        if (mapCell->hasSupport()==true)
                        {
                            auto temp_cell=map->cell(ix,node->area().bottom()+1,node->level()-1);
                            if (temp_cell!= nullptr)
                            {
                                if ((temp_cell->isWalkable()==true) && (temp_cell->navGraphNode()!= nullptr) && (this_cell->hasRamp()==true))
                                    mapCell=temp_cell;
                            }
                        }
                        else if (mapCell->hasFloor()==false)
                        {
                            auto temp_cell=map->cell(ix,node->area().bottom()+1,node->level()+1);
                            if (temp_cell!=nullptr)
                            {
                                if ((upper_cell->hasSupport() == true) && (temp_cell->isWalkable()==true) && (temp_cell->navGraphNode()!=nullptr) && (temp_cell->hasRamp()==true))
                                    mapCell=temp_cell;
                            }
                        }

                        if (mapCell->navGraphNode()!= nullptr)
                        {
                            NavGraphNode::makeConnection(node, mapCell->navGraphNode());
                        }
                    }
                }

                if (this_cell->hasStairs()==true)
                {
                    auto down_cell=map->cell(ix,iy,node->level()-1);
                    if (down_cell!= nullptr)
                    {
                        if ((down_cell->hasNaturalFloor()==false) && (down_cell->navGraphNode()!= nullptr))
                        {
                            NavGraphNode::makeConnection(node, down_cell->navGraphNode());
                        }

                        if ((this_cell->hasScaffolding()==true) && (upper_cell->hasScaffolding()==true) && (upper_cell->navGraphNode()!=nullptr))
                        {
                            NavGraphNode::makeConnection(node, upper_cell->navGraphNode());
                        }
                    }
                }
                else if (upper_cell->hasStairs()==true)
                {
                    if ((this_cell->hasNaturalFloor()==false) && (upper_cell->navGraphNode()!= nullptr))
                    {
                        NavGraphNode::makeConnection(node, upper_cell->navGraphNode());
                    }
                }
            }
        }
    }

    void NavGraph::mergeNavGraphIDs(std::shared_ptr<NavGraphNode> node)
    {
        if (node->connectionsCount()==0)
        {
            if (node->navGraphID()!=-1)
                return;

            node->setNavGraphID(GAME->nextNavNodeID());
        }
        else
        {
            auto idList=node->connectionsID();

            if (idList.size()>0)
            {
                auto id=idList.at(0);
                auto connectionsList=node->connections();
                for (auto connection : connectionsList)
                {
                    if (connection->navGraphID()!=id)
                        this->propagateNavGraphID(connection,id);
                }
            }
        }
    }

    void NavGraph::propagateNavGraphID(std::shared_ptr<NavGraphNode> node, unsigned int id)
    {
        std::vector<std::shared_ptr<NavGraphNode>> source;
        std::vector<std::shared_ptr<NavGraphNode>> navGraphNodeSet1;
        std::vector<std::shared_ptr<NavGraphNode>> navGraphNodeSet2;

        source.push_back(node);
        navGraphNodeSet1.push_back(node);

        while (source.size()>0)
        {
            auto navGraphNode=source[0];
            source.erase(source.begin());

            auto iter=std::find_if(std::begin(navGraphNodeSet1),std::end(navGraphNodeSet1),[&navGraphNode](std::shared_ptr<NavGraphNode> const& value)
            {
                return navGraphNode==value;
            });

            if (iter!=std::end(navGraphNodeSet1))
                navGraphNodeSet1.erase(iter);

            navGraphNodeSet2.push_back(navGraphNode);

            navGraphNode->setNavGraphID(id);

            auto connectionList=navGraphNode->connections();
            for (auto connection : connectionList)
            {
                if (connection->navGraphID()!=id)
                {
                    auto iter1=std::find_if(std::begin(navGraphNodeSet1),std::end(navGraphNodeSet1),[&connection](std::shared_ptr<NavGraphNode> const& value)
                    {
                        return connection==value;
                    });

                    auto iter2=std::find_if(std::begin(navGraphNodeSet2),std::end(navGraphNodeSet2),[&connection](std::shared_ptr<NavGraphNode> const& value)
                    {
                        return connection==value;
                    });

                    if ((iter1==std::end(navGraphNodeSet1)) && (iter2==std::end(navGraphNodeSet2)))
                    {
                        source.push_back(connection);
                        navGraphNodeSet1.push_back(connection);
                    }
                }
            }
        }
    }

    void NavGraph::setNewNodeOnCells(const rectangle2& area, const int& level, std::shared_ptr<NavGraphNode> node)
    {
        auto map=GAME->region()->map();
        for (int y=area.top(), yMax=area.bottom();y<=yMax;y++)
        {
            for (int x=area.left(),xMax=area.right();x<=xMax;x++)
            {
                auto mapCell=map->cell(x,y,level);
                if (mapCell!= nullptr)
                    mapCell->setNavGraphNode(node);
            }
        }
    }

    void NavGraph::setNewNodeOnCells(std::shared_ptr<NavGraphNode> oldNode, std::shared_ptr<NavGraphNode> newNode)
    {
        this->setNewNodeOnCells(oldNode->area(),oldNode->level(),newNode);
    }

    void NavGraph::removeTile(std::shared_ptr<MapCell> mapCell)
    {
        if (mapCell->navGraphNode()==nullptr)
        {
            //std::cout << "Trying to remove tile to nav graph when it isn't on the nav graph." << std::endl;
        }
        else
        {
            auto navGraphNode=mapCell->navGraphNode();
            auto navGraphNodeList=navGraphNode->connections();

            navGraphNode->clearConnections();
            this->setNewNodeOnCells(navGraphNode,nullptr);
            this->subdivideIntoAllowedAreas(navGraphNode->area(),navGraphNode->level(),navGraphNode->level()+1);

            std::vector<unsigned int> idList;
            for (auto node : navGraphNodeList)
            {
                auto nodeID=node->navGraphID();
                auto iter=std::find_if(std::begin(idList),std::end(idList),[&nodeID](unsigned int& value)
                {
                    return (nodeID==value);
                });

                if (iter==std::end(idList))
                {
                    auto id=GAME->nextNavNodeID();
                    idList.push_back(id);
                    this->propagateNavGraphID(node,id);
                }
            }
        }
    }

    void NavGraph::addTile(std::shared_ptr<MapCell> mapCell)
    {
        if (mapCell->navGraphNode()!=nullptr)
        {
            //std::cout << "Trying to add tile to nav graph when it's already on the nav graph." << std::endl;
        }
        else
        {
            mapCell->setNavGraphNode(std::shared_ptr<NavGraphNode>(new NavGraphNode(GAME->nextNavID())));
            auto pos=mapCell->position();
            mapCell->navGraphNode()->setArea(rectangle2(pos.x(),pos.y(),0,0),pos.z());
            this->createConnections(mapCell->navGraphNode());

            auto navGraphNode=mapCell->navGraphNode();
            this->mergeSquareNodeWithNeighbours(navGraphNode);
            while ((mapCell->navGraphNode()== nullptr) && (navGraphNode->equals(mapCell->navGraphNode())))
            {
                navGraphNode=mapCell->navGraphNode();
                this->mergeSquareNodeWithNeighbours(navGraphNode);
            }

            if (mapCell->navGraphNode()==nullptr)
                return;

            this->mergeNavGraphIDs(mapCell->navGraphNode());
        }
    }

    std::shared_ptr<NavGraphNode> NavGraph::largestNeighbour(std::shared_ptr<NavGraphNode> node)
    {
        auto navGraphNode=node;
        auto connectionList=node->connections();
        for (auto connection : connectionList)
        {
            if ((navGraphNode->level()==connection->level()) && (connection->area().width() > navGraphNode->area().width()))
                navGraphNode=connection;
        }

        return navGraphNode;
    }

    bool NavGraph::growRight(const int&, const int& right, const int& top, const int& bottom, const int& level, std::vector<std::shared_ptr<NavGraphNode>>& victimNodes)
    {
        auto map=GAME->region()->map();

        for (int index=top;index<=bottom;index++)
        {
            auto mapCell=map->cell(right,index,level);

            if (mapCell== nullptr)
                return false;

            if (mapCell->isWalkable()==false)
                return false;

            auto node=mapCell->navGraphNode();
            if (node!=nullptr)
            {
                auto iter=std::find_if(std::begin(victimNodes),std::end(victimNodes),[&node](std::shared_ptr<NavGraphNode> const& value)
                {
                    return (node==value);
                });

                if (iter==std::end(victimNodes))
                    victimNodes.push_back(node);
            }
        }

        return true;
    }

    bool NavGraph::growLeft(const int& left, const int&, const int& top, const int& bottom, const int& level, std::vector<std::shared_ptr<NavGraphNode>>& victimNodes)
    {
        auto map=GAME->region()->map();

        for (int index=top;index<=bottom;index++)
        {
            auto mapCell=map->cell(left,index,level);

            if (mapCell->isWalkable()==false)
                return false;

            auto node=mapCell->navGraphNode();
            if (node!=nullptr)
            {
                auto iter=std::find_if(std::begin(victimNodes),std::end(victimNodes),[&node](std::shared_ptr<NavGraphNode> const& value)
                {
                    return (node==value);
                });

                if (iter==std::end(victimNodes))
                    victimNodes.push_back(node);
            }
        }

        return true;
    }

    bool NavGraph::growTop(const int& left, const int& right, const int& top, const int& bottom, const int& level, std::vector<std::shared_ptr<NavGraphNode>>& victimNodes)
    {
        auto map=GAME->region()->map();

        for (int index=left;index<=right;index++)
        {
            auto mapCell=map->cell(index,top,level);

            if (mapCell->isWalkable()==false)
                return false;

            auto node=mapCell->navGraphNode();
            if (node!=nullptr)
            {
                auto iter=std::find_if(std::begin(victimNodes),std::end(victimNodes),[&node](std::shared_ptr<NavGraphNode> const& value)
                {
                    return (node==value);
                });

                if (iter==std::end(victimNodes))
                    victimNodes.push_back(node);
            }
        }

        return true;
    }

    bool NavGraph::growBottom(const int& left, const int& right, const int& top, const int& bottom, const int& level, std::vector<std::shared_ptr<NavGraphNode>>& victimNodes)
    {
        auto map=GAME->region()->map();

        for (int index=left;index<=right;index++)
        {
            auto mapCell=map->cell(index,bottom,level);

            if (mapCell==nullptr)
                return false;

            if (mapCell->isWalkable()==false)
                return false;

            auto node=mapCell->navGraphNode();
            if (node!=nullptr)
            {
                auto iter=std::find_if(std::begin(victimNodes),std::end(victimNodes),[&node](std::shared_ptr<NavGraphNode> const& value)
                {
                    return (node==value);
                });

                if (iter==std::end(victimNodes))
                    victimNodes.push_back(node);
            }
        }

        return true;
    }

    bool NavGraph::growTopBottom(const int& left, const int& right, int& top, int& bottom, const int& level, std::vector<std::shared_ptr<NavGraphNode>>& victimNodes)
    {
        if (!this->growBottom(left,right,top,bottom=bottom+1,level,victimNodes))
        {
            bottom=bottom-1;
            victimNodes.clear();

            if (!this->growTop(left,right,top=top-1,bottom,level,victimNodes))
            {
                top=top+1;
                return false;
            }
        }

        return true;
    }

    bool NavGraph::growLeftRightTopBottom(int& left, int& right, int& top, int& bottom, int& level, std::vector<std::shared_ptr<NavGraphNode>>& victimNodes)
    {
        int temp_left=left;
        int temp_right=right;

        if ((this->growRight(left,right=right+1,top,bottom,level,victimNodes)) && (this->growTopBottom(left,right,top,bottom,level,victimNodes)))
            return true;

        right=temp_right;
        victimNodes.clear();

        if ((this->growLeft(left=left-1,right,top,bottom,level,victimNodes)) && (this->growTopBottom(left,right,top,bottom,level,victimNodes)))
            return true;

        left=temp_left;
        victimNodes.clear();

        return false;
    }

    bool NavGraph::mergeSquareNodeWithNeighbours(std::shared_ptr<NavGraphNode> node)
    {
        auto navGraphNode=this->largestNeighbour(node);
        if (navGraphNode== nullptr)
            return false;

        int left=navGraphNode->area().left();
        int right=navGraphNode->area().right();
        int top=navGraphNode->area().top();
        int bottom=navGraphNode->area().bottom();
        int level=navGraphNode->level();

        std::vector<std::shared_ptr<NavGraphNode>> navGraphNodeList;

        while (true)
        {
            std::vector<std::shared_ptr<NavGraphNode>> victimNodes;

            if (this->growLeftRightTopBottom(left,right,top,bottom,level,victimNodes))
            {
                for (auto victimNode : victimNodes)
                {
                    auto iter=std::find_if(std::begin(navGraphNodeList),std::end(navGraphNodeList),[&victimNode](std::shared_ptr<NavGraphNode> const& value)
                    {
                        return (victimNode==value);
                    });

                    if (iter==std::end(navGraphNodeList))
                        navGraphNodeList.push_back(victimNode);
                }
            }
            else
            {
                rectangle2 bounds=rectangle2(left,top,right-left,bottom-top);

                if (bounds==navGraphNode->area())
                    return false;

                navGraphNodeList.push_back(navGraphNode);

                for (auto oldNode : navGraphNodeList)
                {
                    if (oldNode->area().left()<left)
                        left=oldNode->area().left();

                    if (oldNode->area().right()>right)
                        right=oldNode->area().right();

                    if (oldNode->area().top()<top)
                        top=oldNode->area().top();

                    if (oldNode->area().bottom()>bottom)
                        bottom=oldNode->area().bottom();

                    oldNode->clearConnections();

                    this->setNewNodeOnCells(oldNode, nullptr);
                }

                bounds=rectangle2(left,top,right-left,bottom-top);

                this->subdivideIntoAllowedAreas(bounds,level,level+1);
                return true;
            }
        }

        return true;
    }

    void NavGraph::reestablishConnections(std::shared_ptr<MapCell> mapCell)
    {
        if (mapCell->navGraphNode()==nullptr)
        {
            //std::cout << "Trying to reestablish connections when this tile isn't on the nav graph." << std::endl;
        }
        else
        {
            auto navGraphNodeList=mapCell->navGraphNode()->connections();
            mapCell->navGraphNode()->clearConnections();
            this->createConnections(mapCell->navGraphNode());

            std::vector<unsigned int> idList;
            for (auto node : navGraphNodeList)
            {
                auto node_id=node->navGraphID();
                auto iter1=std::find_if(std::begin(idList),std::end(idList),[&node_id](unsigned int& value)
                {
                    return (value==node_id);
                });

                auto connections=mapCell->navGraphNode()->connections();
                auto iter2=std::find_if(std::begin(connections),std::end(connections),[&node](std::shared_ptr<NavGraphNode> const& value)
                {
                    return (value==node);
                });

                if ((iter1==std::end(idList)) && (iter2==std::end(connections)))
                {
                    auto id=GAME->nextNavNodeID();
                    idList.push_back(id);
                    this->propagateNavGraphID(node,id);
                }
            }

            if (mapCell->navGraphNode()->connectionsCount()==0)
                return;

            this->propagateNavGraphID(mapCell->navGraphNode(),mapCell->navGraphNode()->connections().at(0)->navGraphID());
        }
    }
}