//
// Created by tatiana on 26.09.18.
//

#include "ActionEvent.h"

namespace gui
{
    ActionEvent::ActionEvent(std::shared_ptr<ZWidget> source, const std::string& id)
            :Event(source),
             mId(id)
    {

    }

    ActionEvent::~ActionEvent()
    {

    }

    const std::string& ActionEvent::getId() const
    {
        return mId;
    }
}