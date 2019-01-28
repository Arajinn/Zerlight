//
// Created by tatiana on 26.09.18.
//

#ifndef GUICHAN_ZCOLOR_H
#define GUICHAN_ZCOLOR_H

#include <iostream>

namespace gui {

#define COLOR_DARK_GRAY 0x353535
#define COLOR_LIGHT_GRAY 0xa0a0a0
#define COLOR_BLACK 0x000000
#define COLOR_WHITE 0xffffff
    /**
     * Represents a color with red, green, blue and alpha components.
     */
    class ZColor {
    public:
        /**
         * Constructor. Initializes the color to black.
         */
        ZColor();

        /**
         * Constructor. Constructs a color from the bytes in an integer.
         * Call it with a hexadecimal constant for HTML-style color representation.
         * The alpha component is 255 by default.
         *
         * EXAMPLE: Color(0xff50a0) constructs a very nice pinkish color.
         *
         * NOTE: Because of this constructor, integers will be automatically
         *       casted to a color by your compiler.
         *
         * @param color The color to initialise the object with.
         */
        ZColor(int color);

        /**
         * Constructor. The default alpha value is 255.
         *
         * @param r Red color component (range 0-255).
         * @param g Green color component  (range 0-255).
         * @param b Blue color component (range 0-255).
         * @param a Alpha, used for transparency. A value of 0 means
         *          totaly transparent, 255 is totaly opaque.
         */
        ZColor(int r, int g, int b, int a = 255);

        ZColor(std::string color_info);

        /**
         * Adds the RGB values of two colors together. The values will be
         * clamped if they go out of range.
         *
         * WARNING: This function will reset the alpha value of the
         *          returned color to 255.
         *
         * @param color A color to add to this color.
         * @return The added colors with an alpha value set to 255.
         */
        ZColor operator+(const ZColor& color) const;

        /**
         * Subtracts the RGB values of one color from another.
         * The values will be clamped if they go out of range.
         *
         * WARNING: This function will reset the alpha value of the
         *          returned color to 255.
         *
         * @param color A color to subtract from this color.
         * @return The subtracted colors with an alpha value set to 255.
         */
        ZColor operator-(const ZColor& color) const;

        /**
         * Multiplies the RGB values of a color with a float value.
         * The values will be clamped if they go out of range.
         *
         * @param value The value to multiply the color with.
         * @return The multiplied colors. The alpha value will, unlike
         *         the add and subtract operations, be multiplied as
         *         well.
         */
        ZColor operator*(float value) const;

        /**
         * Compares two colors.
         *
         * @return True if the two colors have the same RGBA components
         *         false otherwise.
         */
        bool operator==(const ZColor& color) const;

        /**
         * Compares two colors.
         *
         * @return True if the two colors have different RGBA components,
         *         false otherwise.
         */
        bool operator!=(const ZColor& color) const;

        /**
         * Output operator for output.
         *
         * @param out The stream to output to.
         * @param color The color to output.
         */
        friend std::ostream& operator<<(std::ostream& out, const ZColor& Color);

        /**
         * Holds the red color component (range 0-255).
         */
        int r;

        /**
         *  Holds the green color component (range 0-255).
         */
        int g;

        /**
         *  Holds the blue color component (range 0-255).
         */
        int b;

        /**
         * Holds the alpha color component. A value of 0 means totally
         * transparent while a value of 255 is considered opaque.
         */
        int a;

        bool valid() const;
    };
}

#endif //GUICHAN_ZCOLOR_H
