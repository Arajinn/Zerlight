//
// Created by tatiana on 26.09.18.
//

#ifndef GUICHAN_DEFAULTFONT_H
#define GUICHAN_DEFAULTFONT_H

#include "guichan/gui/Font.h"

namespace gui {
    class Graphics;

    /**
     * A font only capable of drawing rectangles. It is used by default
     * in Guichan if no font has been set merely to show that no font has
     * been set.
     */
    class DefaultFont : public Font {
    public:
        /**
         * Destructor.
         */
        virtual ~DefaultFont();

        /**
         * Draws a glyph as a rectangle. The glyphs will always be drawn as
         * rectangles no matter the glyph.
         *
         * NOTE: You normally won't use this function to draw text since
         *       the Graphics class contains better functions for drawing
         *       text.
         *
         * @param graphics A Graphics object to be used for drawing.
         * @param glyph The glyph to draw.
         * @param x The x coordinate where to draw the glyph.
         * @param y The y coordinate where to draw the glyph.
         * @return The width of the drawn glyph in pixels.
         */
        virtual int drawGlyph(std::shared_ptr<Graphics> graphics, unsigned char glyph, int x, int y);


        // Inherited from Font

        virtual void drawString(std::shared_ptr<Graphics> graphics, const std::string& text, int x, int y);

        virtual int getWidth(const std::string& text) const;

        virtual int getHeight() const;

        virtual int getStringIndexAt(const std::string& text, int x) const;
    };
}

#endif //GUICHAN_DEFAULTFONT_H
