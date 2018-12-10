//
// Created by tatiana on 26.09.18.
//

#include "ZColor.h"

#include <vector>
#include <regex>

namespace gui
{
    int ZColor::COLOR_DARK_GRAY=0x353535;
    int ZColor::COLOR_LIGHT_GRAY=0xa0a0a0;
    int ZColor::COLOR_BLACK=0x000000;
    int ZColor::COLOR_WHITE=0xffffff;


    ZColor::ZColor()
            : r(0),
              g(0),
              b(0),
              a(255)
    {
    }

    ZColor::ZColor(int color)
            : r( (color >> 16) & 0xFF),
              g( (color >>  8) & 0xFF),
              b(  color        & 0xFF),
              a(255)
    {
    }

    ZColor::ZColor(int ar, int ag, int ab, int aa)
            : r(ar),
              g(ag),
              b(ab),
              a(aa)
    {
    }

    ZColor::ZColor(std::string color_info)
    {
        std::vector<std::string> items;
        std::regex reg("[^ ]+");
        std::sregex_token_iterator begin(color_info.begin(), color_info.end(), reg), end;
        std::copy(begin, end, std::back_inserter(items));
        if (items.size()!=4)
        {
            r=0;
            g=0;
            b=0;
            a=255;
        }
        else
        {
            r=std::stoi(items.at(0));
            g=std::stoi(items.at(1));
            b=std::stoi(items.at(2));
            a=std::stoi(items.at(3));
        }
    }

    ZColor ZColor::operator+(const ZColor& color) const
    {
        ZColor result(r + color.r, g + color.g, b + color.b, 255);

        result.r = (result.r>255?255:(result.r<0?0:result.r));
        result.g = (result.g>255?255:(result.g<0?0:result.g));
        result.b = (result.b>255?255:(result.b<0?0:result.b));

        return result;
    }

    ZColor ZColor::operator-(const ZColor& color) const
    {
        ZColor result(r - color.r, g - color.g, b - color.b, 255);

        result.r = (result.r>255?255:(result.r<0?0:result.r));
        result.g = (result.g>255?255:(result.g<0?0:result.g));
        result.b = (result.b>255?255:(result.b<0?0:result.b));

        return result;
    }

    ZColor ZColor::operator*(float value) const
    {
        ZColor result((int)(r * value), (int)(g * value), (int)(b * value), a);

        result.r = (result.r>255?255:(result.r<0?0:result.r));
        result.g = (result.g>255?255:(result.g<0?0:result.g));
        result.b = (result.b>255?255:(result.b<0?0:result.b));

        return result;
    }

    bool ZColor::operator==(const ZColor& color) const
    {
        return r == color.r && g == color.g && b == color.b && a == color.a;
    }

    bool ZColor::operator!=(const ZColor& color) const
    {
        return !(r == color.r && g == color.g && b == color.b && a == color.a);
    }

    std::ostream& operator<<(std::ostream& out, const ZColor& color)
    {
        out << "Color [r = " << color.r << ", g = " << color.g << ", b = " << color.b << ", a = " << color.a << "]";

        return out;
    }

    bool ZColor::valid() const
    {
        return ((r>=0) && (r<=255) && (g>=0) && (g<=255) && (b>=0) && (b<=255));
    }
}