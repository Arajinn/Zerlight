//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_VECTOR3_H
#define ZERL_VECTOR3_H

namespace map {
    #define vector3_one vector3(-1,-1,-1)

    class vector3 {
    public:
        vector3();
        vector3(int a_x, int a_y, int a_z);
        ~vector3();

        const int& x() const;
        const int& y() const;
        const int& z() const;

        vector3 operator+(const vector3& value);
        vector3 operator-(const vector3& value);

        bool operator== (const vector3& value) const;
        bool operator!= (const vector3& value) const;

        //vector3& operator=(vector3 value);
        vector3& operator=(const vector3& value);

        const float lengthSquared() const;

        static float distance(const vector3& start, const vector3& end);

        void set_x(const int& value);
        void set_y(const int& value);
        void set_z(const int& value);
    private:
        int mX;
        int mY;
        int mZ;
    };
}

#endif //ZERL_VECTOR3_H
