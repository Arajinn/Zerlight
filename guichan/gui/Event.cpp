//
// Created by tatiana on 26.09.18.
//

#include "Event.h"

namespace gui
{
    Event::Event(std::shared_ptr<ZWidget> source)
            :mSource(source)
    {

    }

    Event::~Event()
    {

    }

    std::shared_ptr<ZWidget> Event::getSource() const
    {
        return mSource;
    }
}