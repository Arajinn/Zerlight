//
// Created by tatiana on 28.03.2018.
//

#ifndef ZERL_RECTANGLE2_H
#define ZERL_RECTANGLE2_H

#include <utility>

namespace map {
    class rectangle2 {
    public:
        rectangle2();
        rectangle2(const int& x,const int& y,const int& width,const int& height);
        ~rectangle2();

        const int& left() const;
        const int& right() const;

        const int& top() const;
        const int& bottom() const;

        const int& centerX() const;
        const int& centerY() const;

        const int& width() const;
        const int& height() const;

        bool operator== (const rectangle2& value) const;
    private:
        int mLeft, mRight, mTop, mBottom;

        int mWidth, mHeight;
        int mCenterX, mCenterY;

        void afterChangeX();
        void afterChangeY();
    };
}

#endif //ZERL_RECTANGLE2_H
