//
// Created by tatiana.polozova on 28.03.2018.
//

#include "NavGraphNode.h"
#include "game/core/GameManager.h"
#include "game/map/Region.h"
#include "game/map/Map.h"
#include "game/map/MapCell.h"

#include <algorithm>

namespace map
{
    NavGraphNode::NavGraphNode(int id)
    {
        mNodeID=id;
        mNavGraphID=-1;
    }

    NavGraphNode::~NavGraphNode()
    {

    }

    void NavGraphNode::setArea(rectangle2 area, int level)
    {
        mArea=area;
        mLevel=level;

        this->findCenter();
    }

    void NavGraphNode::findCenter()
    {
        mCenter=vector3(mArea.centerX(),mArea.centerY(),mLevel);
    }

    const rectangle2& NavGraphNode::area() const
    {
        return mArea;
    }

    const int& NavGraphNode::level() const
    {
        return mLevel;
    }

    void NavGraphNode::makeConnection(std::shared_ptr<NavGraphNode> node1,std::shared_ptr<NavGraphNode> node2)
    {
        node1->addConnection(node2);
        node2->addConnection(node1);
    }

    void NavGraphNode::addConnection(std::shared_ptr<NavGraphNode> node)
    {
        auto iter=std::find_if(std::begin(mConnections),std::end(mConnections),[&node](std::shared_ptr<NavGraphNode> const& value)
        {
           return node->mNodeID==value->mNodeID;
        });

        if (iter==std::end(mConnections))
            mConnections.push_back(node);
    }

    const int& NavGraphNode::nodeNavID() const
    {
        return mNodeID;
    }

    const int& NavGraphNode::navGraphID() const
    {
        return mNavGraphID;
    }

    void NavGraphNode::setNavGraphID(int id)
    {
        mNavGraphID=id;
    }

    int NavGraphNode::connectionsCount() const
    {
        return mConnections.size();
    }

    std::vector<unsigned int> NavGraphNode::connectionsID()
    {
        std::vector<unsigned int> result;
        for (int i=0,isize=mConnections.size();i<isize;i++)
        {
            auto id=mConnections.at(i)->navGraphID();
            if (id!=-1)
                result.push_back(id);
        }

        return result;
    }

    std::vector<std::shared_ptr<NavGraphNode>> NavGraphNode::connections() const
    {
        return mConnections;
    }

    void NavGraphNode::removeConnection(std::shared_ptr<NavGraphNode> node)
    {
        auto iter=std::find_if(std::begin(mConnections),std::end(mConnections),[&node](std::shared_ptr<NavGraphNode> const& value)
        {
            return node->mNodeID==value->mNodeID;
        });

        if (iter!=std::end(mConnections))
            mConnections.erase(iter);
    }

    void NavGraphNode::clearConnections()
    {
        for (auto connection : mConnections)
            connection->removeConnection(shared_from_this());

        mConnections.clear();
    }

    bool NavGraphNode::equals(std::shared_ptr<NavGraphNode> node) const
    {
        if (node==nullptr)
            return false;

        return ((mArea==node->mArea) && (mLevel==node->mLevel));
    }

    bool NavGraphNode::contains(const map::vector3& position)
    {
        if (mLevel!=position.z())
            return false;

        return this->contains2D(position);
    }

    bool NavGraphNode::contains2D(const map::vector3& position) const
    {
        auto pos_x=position.x();
        if ((pos_x<mArea.left()) || (pos_x>mArea.right()))
            return false;

        auto pos_y=position.y();
        if ((pos_y<mArea.top()) || (pos_y>mArea.bottom()))
            return false;

        return true;
    }

    map::vector3 NavGraphNode::closestPoint(const map::vector3& position) const
    {
        if (this->contains2D(position))
            return map::vector3(position.x(),position.y(),mLevel);

        int newX=std::min(std::max(mArea.left(),position.x()),mArea.right());
        int newY=std::min(std::max(mArea.top(),position.y()),mArea.bottom());
        return map::vector3(newX,newY,mLevel);
    }

    bool NavGraphNode::hasDryLand() const
    {
        auto map=GAME->region()->map();
        for (int i=0,isize=mArea.height();i<isize;i++)
        {
            for (int j=0,jsize=mArea.width();j<jsize;j++)
            {
                auto cell=map->cell(i+mArea.left(),j+mArea.top(),mLevel);
                if (!cell->willSuffocate() && !cell->hasLava())
                    return true;
            }
        }

        return false;
    }
}