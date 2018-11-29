//
// Created by tatiana on 27.09.18.
//

#include "InputEvent.h"

namespace gui
{
    InputEvent::InputEvent(std::shared_ptr<ZWidget> source,
                           std::shared_ptr<ZWidget> distributor,
                           bool isShiftPressed,
                           bool isControlPressed,
                           bool isAltPressed,
                           bool isMetaPressed)
            :Event(source),
             mShiftPressed(isShiftPressed),
             mControlPressed(isControlPressed),
             mAltPressed(isAltPressed),
             mMetaPressed(isMetaPressed),
             mIsConsumed(false),
             mDistributor(distributor)
    {

    }

    bool InputEvent::isShiftPressed() const
    {
        return mShiftPressed;
    }

    bool InputEvent::isControlPressed() const
    {
        return mControlPressed;
    }

    bool InputEvent::isAltPressed() const
    {
        return mAltPressed;
    }

    bool InputEvent::isMetaPressed() const
    {
        return mMetaPressed;
    }

    void InputEvent::consume()
    {
        mIsConsumed = true;
    }

    bool InputEvent::isConsumed() const
    {
        return mIsConsumed;
    }

    std::shared_ptr<ZWidget> InputEvent::getDistributor() const
    {
        return mDistributor;
    }

    void InputEvent::setDistributor(std::shared_ptr<ZWidget> distributor)
    {
        mDistributor=distributor;
    }
}