//
// Created by tatiana on 26.09.18.
//

#ifndef GUICHAN_GRAPHICS_H
#define GUICHAN_GRAPHICS_H


#include "guichan/gui/ZColor.h"
#include "guichan/gui/ClipRectangle.h"

#include <stack>
#include <memory>

namespace gui {
    class ZImage;
    class Font;

    /**
     * Abstract class for providing drawing primitve functions.
     * It contains all vital functions for drawing.
     *
     * Guichan contains implementations of Graphics for common
     * libraries like the Allegro library, the HGE library,
     * the OpenGL library, the OpenLayer library, and the SDL library.
     * To make Guichan usable with other libraries, a Graphics class
     * must be implemented.
     *
     * In Graphics you can set clip areas to limit drawing to certain
     * areas of the screen. Clip areas are put on a stack, which
     * means that you can push smaller and smaller clip areas onto the
     * stack. All coordinates will be relative to the top most clip area.
     * In most cases you won't have to worry about the clip areas,
     * unless you want to implement some really complex widget.
     * Pushing and poping of clip areas are handled automatically by
     * container widgets when their child widgets are drawn.
     *
     * IMPORTANT: Remember to pop each clip area that you pushed on the stack
     * after you are done with it.
     *
     * If you feel that Graphics is to restrictive for your needs,
     * there is no one stopping you from using your own code for drawing
     * in widgets. You could for instance use pure SDL in the drawing of
     * widgets bypassing Graphics. This might however hurt portability of
     * your application.
     *
     * If you implement a Graphics class not present in Guichan we would
     * be very happy to add it to Guichan.
     *
     * @see AllegroGraphics, HGEGraphics, OpenLayerGraphics, OpenGLGraphics,
     *      SDLGraphics, Image
     * @since 0.1.0
     */
class Graphics : public std::enable_shared_from_this<Graphics> {
    public:
        /**
         * Alignments for text drawing.
         */
        enum Alignment
        {
            Left = 0,
            Center,
            Right
        };

        /**
         * Constructor.
         */
        Graphics();

        /**
         * Destructor.
         */
        virtual ~Graphics();

        /**
         * Initializes drawing. Called by the Gui when Gui::draw() is called.
         * It is needed by some implementations of Graphics to perform
         * preparations before drawing. An example of such an implementation
         * is the OpenGLGraphics.
         *
         * NOTE: You will never need to call this function yourself, unless
         *       you use a Graphics object outside of Guichan.
         *
         * @see endDraw, Gui::draw
         */
        virtual void beginDraw();

        /**
         * Deinitializes drawing. Called by the Gui when a Gui::draw() is done.
         * done. It should reset any state changes made by beginDraw().
         *
         * NOTE: You will never need to call this function yourself, unless
         *       you use a Graphics object outside of Guichan.
         *
         * @see beginDraw, Gui::draw
         */
        virtual void endDraw();

        /**
         * Pushes a clip area onto the stack. The x and y coordinates in the
         * rectangle is  relative to the last pushed clip area.
         * If the new area falls outside the current clip area, it will be
         * clipped as necessary.
         *
         * If a clip area is outside of the top clip area a clip area with
         * zero width and height will be pushed.
         *
         * @param area The clip area to be pushed onto the stack.
         * @return False if the the new area lays outside the current clip
         *         area.
         */
        virtual bool pushClipArea(Rectangle area);

        /**
         * Removes the top most clip area from the stack.
         *
         * @throws Exception if the stack is empty.
         */
        virtual void popClipArea();

        /**
         * Gets the current clip area. Usefull if you want to do drawing
         * bypassing Graphics.
         *
         * @return The current clip area.
         */
        virtual const ClipRectangle& getCurrentClipArea();

        /**
         * Draws a part of an image.
         *
         * NOTE: Width and height arguments will not scale the image but
         *       specifies the size of the part to be drawn. If you want
         *       to draw the whole image there is a simplified version of
         *       this function.
         *
         * EXAMPLE: @code drawImage(myImage, 10, 10, 20, 20, 40, 40); @endcode
         *          Will draw a rectangular piece of myImage starting at
         *          coordinate (10, 10) in myImage, with width and height 40.
         *          The piece will be drawn with it's top left corner at
         *          coordinate (20, 20).
         *
         * @param image The image to draw.
         * @param srcX The source image x coordinate.
         * @param srcY The source image y coordinate.
         * @param dstX The destination x coordinate.
         * @param dstY The destination y coordinate.
         * @param width The width of the piece.
         * @param height The height of the piece.
         */
        virtual void drawImage(std::shared_ptr<const ZImage> image,
                               int srcX,
                               int srcY,
                               int dstX,
                               int dstY,
                               int width,
                               int height) = 0;
        /**
         * Draws an image. A simplified version of the other drawImage.
         * It will draw a whole image at the coordinate you specify.
         * It is equivalent to calling:
         * @code drawImage(myImage, 0, 0, dstX, dstY, image->getWidth(), \
         image->getHeight()); @endcode
         */
        virtual void drawImage(std::shared_ptr<const ZImage> image, int dstX, int dstY);

        virtual void drawImageScale(std::shared_ptr<const ZImage> image,
                           int srcX,
                           int srcY,
                           int dstX,
                           int dstY,
                           int width,
                           int height,
                           const gui::ZColor& color) = 0;

        /**
         * Draws a single point/pixel.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         */
        virtual void drawPoint(int x, int y) = 0;

        /**
         * Ddraws a line.
         *
         * @param x1 The first x coordinate.
         * @param y1 The first y coordinate.
         * @param x2 The second x coordinate.
         * @param y2 The second y coordinate.
         */
        virtual void drawLine(int x1, int y1, int x2, int y2) = 0;

        /**
         * Draws a simple, non-filled rectangle with a one pixel width.
         *
         * @param rectangle The rectangle to draw.
         */
        virtual void drawRectangle(const Rectangle& rectangle) = 0;

        /**
         * Draws a simple, non-filled rectangle with a one pixel width.
         *
         * This is an overload provided for convenience.
         *
         * @param x      The x coordinate of the rectangle
         * @param y      The y coordinate of the rectangle
         * @param width  The width of the rectangle
         * @param height The height of the rectangle
         *
         * @since 0.9.0
         */
        void drawRectangle(int x, int y, int width, int height);

        /**
         * Draws a filled rectangle.
         *
         * @param rectangle The filled rectangle to draw.
         */
        virtual void fillRectangle(const Rectangle& rectangle) = 0;

        /**
         * Draws a filled rectangle.
         *
         * This is an overload provided for convenience.
         *
         * @param x      The x coordinate of the rectangle
         * @param y      The y coordinate of the rectangle
         * @param width  The width of the rectangle
         * @param height The height of the rectangle
         *
         * @since 0.9.0
         */
        void fillRectangle(int x, int y, int width, int height);

        /**
         * Sets the color to use when drawing.
         *
         * @param color A color.
         * @see getColor
         */
        virtual void setColor(const ZColor& color) = 0;

        /**
         * Gets the color to use when drawing.
         *
         * @return The color used when drawing.
         * @see setColor
         */
        virtual const ZColor& getColor() const = 0;

        /**
         * Sets the font to use when drawing text.
         *
         * @param font The font to use when drawing.
         */
        virtual void setFont(std::shared_ptr<Font> font);

        /**
         * Draws text.
         *
         * @param text The text to draw.
         * @param x The x coordinate where to draw the text.
         * @param y The y coordinate where to draw the text.
         * @param alignment The alignemnt to use when drawing.
         * @throws Exception when no font has been set.
         */
        virtual void drawText(const std::string& text,
                              int x,
                              int y,
                              Alignment alignment = Left);

    virtual void textureMixOn() = 0;
    virtual void textureMixOff() = 0;
    protected:
        /**
         * Holds the clip area stack.
         */
        std::stack<ClipRectangle> mClipStack;

        /**
         * Holds the current font.
         */
        std::shared_ptr<Font> mFont;
    };
}

#endif //GUICHAN_GRAPHICS_H
