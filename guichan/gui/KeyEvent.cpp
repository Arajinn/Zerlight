//
// Created by tatiana on 27.09.18.
//

#include "KeyEvent.h"

namespace gui
{
    KeyEvent::KeyEvent(std::shared_ptr<ZWidget> source,
                       std::shared_ptr<ZWidget> distributor,
                       bool isShiftPressed,
                       bool isControlPressed,
                       bool isAltPressed,
                       bool isMetaPressed,
                       unsigned int type,
                       bool isNumericPad,
                       const Key& key)
            :InputEvent(source,
                        distributor,
                        isShiftPressed,
                        isControlPressed,
                        isAltPressed,
                        isMetaPressed),
             mType(type),
             mIsNumericPad(isNumericPad),
             mKey(key)
    {

    }

    KeyEvent::~KeyEvent()
    {

    }

    unsigned int KeyEvent::getType() const
    {
        return mType;
    }

    bool KeyEvent::isNumericPad() const
    {
        return mIsNumericPad;
    }

    const Key& KeyEvent::getKey() const
    {
        return mKey;
    }
}