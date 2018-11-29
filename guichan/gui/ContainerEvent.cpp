//
// Created by tatiana on 28.09.18.
//

#include "ContainerEvent.h"

namespace gui
{
    ContainerEvent::ContainerEvent(std::shared_ptr<ZWidget> source, std::shared_ptr<ZContainer> container)
            :Event(source),
             mContainer(container)
    {

    }

    ContainerEvent::~ContainerEvent()
    {

    }

    std::shared_ptr<ZContainer> ContainerEvent::getContainer() const
    {
        return mContainer;
    }
}