//
// Created by tatiana on 26.03.2018.
//

#include "vector3.h"
#include <math.h>

namespace map
{
    vector3::vector3()
    {
        mX=-1;
        mY=-1;
        mZ=-1;
    }

    vector3::vector3(int a_x, int a_y, int a_z)
    {
        mX=a_x;
        mY=a_y;
        mZ=a_z;
    }

    vector3::~vector3()
    {

    }

    const int& vector3::x() const
    {
        return mX;
    }

    const int& vector3::y() const
    {
        return mY;
    }

    const int& vector3::z() const
    {
        return mZ;
    }

    vector3 vector3::operator+(const vector3& value)
    {
        return vector3(this->mX+value.mX,this->mY+value.mY,this->mZ+value.mZ);
    }

    vector3 vector3::operator-(const vector3& value)
    {
        return vector3(this->mX-value.mX,this->mY-value.mY,this->mZ-value.mZ);
    }

    bool vector3::operator== (const vector3& value) const
    {
        return ((this->mX==value.mX) && (this->mY==value.mY) && (this->mZ==value.mZ));
    }

    bool vector3::operator!= (const vector3& value) const
    {
        return ((this->mX!=value.mX) || (this->mY!=value.mY) || (this->mZ!=value.mZ));
    }

    //vector3& vector3::operator=(vector3 value)
    //{
    //    mX=value.mX;
    //    mY=value.mY;
    //    mZ=value.mZ;

    //    return *this;
    //}

    vector3& vector3::operator=(const vector3& value)
    {
        mX=value.mX;
        mY=value.mY;
        mZ=value.mZ;

        return *this;
    }

    const float vector3::lengthSquared() const
    {
        return mX*mX+mY*mY+mZ*mZ;
    }

    const float vector3::length() const
    {
        const float d2=lengthSquared();
        return sqrt(d2);
    }

    float vector3::distance(const vector3& start, const vector3& end)
    {
        float deltaX=float(start.x()-end.x());
        float deltaY=float(start.y()-end.y());
        float deltaZ=float(start.z()-end.z());
        return sqrtf(deltaX*deltaX+deltaY*deltaY+deltaZ*deltaZ);
    }

    void vector3::set_x(const int& value)
    {
        mX=value;
    }

    void vector3::set_y(const int& value)
    {
        mY=value;
    }

    void vector3::set_z(const int& value)
    {
        mZ=value;
    }
}
