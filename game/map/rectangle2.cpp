//
// Created by tatiana.polozova on 28.03.2018.
//

#include "rectangle2.h"

namespace map
{
    rectangle2::rectangle2()
    {
        mLeft=0;
        mRight=0;
        this->afterChangeX();

        mTop=0;
        mBottom=0;
        this->afterChangeY();
    }

    rectangle2::rectangle2(const int& x,const int& y,const int& width,const int& height)
    {
        mLeft=x;
        mRight=x+width;
        this->afterChangeX();

        mTop=y;
        mBottom=y+height;
        this->afterChangeY();
    }

    rectangle2::~rectangle2()
    {

    }

    const int& rectangle2::left() const
    {
        return mLeft;
    }

    const int& rectangle2::right() const
    {
        return mRight;
    }

    const int& rectangle2::top() const
    {
        return mTop;
    }

    const int& rectangle2::bottom() const
    {
        return mBottom;
    }

    const int& rectangle2::centerX() const
    {
        return mCenterX;
    }

    const int& rectangle2::centerY() const
    {
        return mCenterY;
    }

    const int& rectangle2::width() const
    {
        return mWidth;
    }

    const int& rectangle2::height() const
    {
        return mHeight;
    }

    void rectangle2::afterChangeX()
    {
        mWidth=mRight-mLeft;
        mCenterX=(mRight+mLeft)/2;
    }

    void rectangle2::afterChangeY()
    {
        mHeight=mBottom-mTop;
        mCenterY=(mBottom+mTop)/2;
    }

    bool rectangle2::operator== (const rectangle2& value) const
    {
        return ((this->mTop==value.mTop) && (this->mBottom==value.mBottom) && (this->mLeft==value.mLeft)&& (this->mRight==value.mRight));
    }
}