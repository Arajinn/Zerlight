//
// Created by tatiana on 26.09.18.
//

#include "ClipRectangle.h"

namespace gui
{
    ClipRectangle::ClipRectangle()
        :Rectangle()
    {
        x = y = width = height = xOffset = yOffset  = 0;
    }

    ClipRectangle::ClipRectangle(int x, int y, int width, int height, int xOffset, int yOffset)
        :Rectangle(x,y,width,height)
    {
        this->xOffset = xOffset;
        this->yOffset = yOffset;
    }

    const ClipRectangle& ClipRectangle::operator=(const Rectangle& other)
    {
        x = other.x;
        y = other.y;
        width = other.width;
        height = other.height;

        return *this;
    }
}