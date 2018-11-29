//
// Created by tatiana on 28.09.18.
//

#include "SelectionEvent.h"

namespace gui
{
    SelectionEvent::SelectionEvent(std::shared_ptr<ZWidget> source)
            :Event(source)
    {

    }

    SelectionEvent::~SelectionEvent()
    {

    }
}