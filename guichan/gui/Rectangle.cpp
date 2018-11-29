//
// Created by tatiana on 26.09.18.
//

#include "Rectangle.h"

namespace gui
{
    Rectangle::Rectangle()
            : x(0),
              y(0),
              width(0),
              height(0)
    {
    }

    Rectangle::Rectangle(int x_, int y_, int width_, int height_)
            : x(x_),
              y(y_),
              width(width_),
              height(height_)
    {
    }

    void Rectangle::setAll(int x_, int y_, int width_, int height_)
    {
        x = x_;
        y = y_;
        width = width_;
        height = height_;
    }

    bool Rectangle::isIntersecting(const Rectangle& rectangle) const
    {
        int x_ = x;
        int y_ = y;
        int width_ = width;
        int height_ = height;

        x_ -= rectangle.x;
        y_ -= rectangle.y;

        if (x_ < 0)
        {
            width_ += x_;
            x_ = 0;
        }
        else if (x_ + width_ > rectangle.width)
        {
            width_ = rectangle.width - x_;
        }

        if (y_ < 0)
        {
            height_ += y_;
            y_ = 0;
        }
        else if (y_ + height_ > rectangle.height)
        {
            height_ = rectangle.height - y_;
        }

        if (width_ <= 0 || height_ <= 0)
        {
            return false;
        }

        return true;
    }

    bool Rectangle::isContaining(int x_, int y_) const
    {
        return x_ >= x && y_ >= y && x_ < x + width && y_ < y + height;
    }

    bool Rectangle::isContaining(const Rectangle& other) const
    {
        if (other.isEmpty())
            return false;

        return other.x >= x && other.y >= y && other.x + other.width <= x + width && other.y + other.height <= y + height;
    }

    bool Rectangle::isEmpty() const
    {
        return width <= 0 || height <= 0;
    }

    Rectangle Rectangle::operator+(const Rectangle& rh) const
    {
        if (rh.isEmpty())
            return Rectangle(x, y, width, height);

        int nx = x < rh.x ? x : rh.x;
        int ny = y < rh.y ? y : rh.y;
        int nx2 = x + width > rh.x + rh.width ? x + width : rh.x + rh.width;
        int ny2 = y + height > rh.y + rh.height ? y + height : rh.y + rh.height;
        return Rectangle(nx, ny, nx2 - nx, ny2 - ny);
    }

    const Rectangle& Rectangle::operator+=(const Rectangle& rh)
    {
        if (rh.isEmpty())
            return *(this);

        if (isEmpty())
            return rh;

        x = x < rh.x ? x : rh.x;
        y = y < rh.y ? y : rh.y;
        int x2 = x + width > rh.x + rh.width ? x + width : rh.x + rh.width;
        int y2 = y + height > rh.y + rh.height ? y + height : rh.y + rh.height;
        width = x2 - x;
        height = y2 - y;
        return *(this);
    }

    Rectangle Rectangle::intersection(const Rectangle& rh) const
    {
        int nx = x > rh.x ? x : rh.x;
        int ny = y > rh.y ? y : rh.y;

        if (rh.isEmpty() || isEmpty())
            return Rectangle(nx, ny, 0, 0);

        int nx2 = x + width < rh.x + rh.width ? x + width : rh.x + rh.width;
        int ny2 = y + height < rh.y + rh.height ? y + height : rh.y + rh.height;
        Rectangle result(nx, ny, nx2 - nx, ny2 - ny);

        if (result.isEmpty())
            return Rectangle(nx, ny, 0, 0);

        return result;
    }

    std::ostream& operator<<(std::ostream& out, const Rectangle& rectangle)
    {
        out << "Rectangle [x = " << rectangle.x << ", y = " << rectangle.y
        << ", width = " << rectangle.width << ", height = " << rectangle.height << "]";

        return out;
    }
}