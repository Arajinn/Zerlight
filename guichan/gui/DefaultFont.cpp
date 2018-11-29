//
// Created by tatiana on 26.09.18.
//

#include "DefaultFont.h"
#include "Graphics.h"

namespace gui
{
    DefaultFont::~DefaultFont()
    {

    }

    int DefaultFont::getHeight() const
    {
        return 8;
    }

    int DefaultFont::getWidth(const std::string& text) const
    {
        return 8 * text.size();
    }

    int DefaultFont::drawGlyph(std::shared_ptr<Graphics> graphics, unsigned char glyph, int x, int y)
    {
        graphics->drawRectangle(x, y, 8, 8);

        return 8;
    }

    void DefaultFont::drawString(std::shared_ptr<Graphics> graphics, const std::string& text, int x, int y)
    {
        unsigned int i;

        for (i = 0; i< text.size(); ++i)
        {
            drawGlyph(graphics, text.at(i), x, y);
            x += getWidth(text);
        }
    }

    int DefaultFont::getStringIndexAt(const std::string& text, int x) const
    {
        if (x > (int)text.size() * 8)
        {
            return text.size();
        }

        return x / 8;
    }
}