//
// Created by tatiana on 26.09.18.
//

#include "Font.h"

namespace gui
{
    Font::~Font()
    {

    }

    int Font::getStringIndexAt(const std::string& text, int x) const
    {
        unsigned int i;
        int size = 0;

        for (i = 0; i < text.size(); ++i)
        {
            size = getWidth(text.substr(0,i));

            if (size > x)
            {
                return i;
            }
        }

        return text.size();
    }
}