//
// Created by tatiana on 01.10.18.
//

#ifndef GUICHAN_OPENGLGRAPHICS_H
#define GUICHAN_OPENGLGRAPHICS_H

#include "guichan/gui/Graphics.h"
#include "guichan/gui/Rectangle.h"

namespace gui
{
    class ZImage;
}

namespace sdl_interface {
    /**
     * OpenGL implementation of the Graphics.
     */
    class OpenGLGraphics : public gui::Graphics{
    public:
        /**
         * Constructor.
         */
        OpenGLGraphics();

        /**
         * Constructor.
		 *
		 * @param width the width of the logical drawing surface. Should be the
         *              same as the screen resolution.
		 *
		 * @param height the height ot the logical drawing surface. Should be
		 *               the same as the screen resolution.
		 */
        OpenGLGraphics(int width, int height);

        /**
         * Destructor.
         */
        virtual ~OpenGLGraphics();

        /**
         * Sets the target plane on where to draw.
		 *
		 * @param width the width of the logical drawing surface. Should be the
		 *              same as the screen resolution.
		 * @param height the height ot the logical drawing surface. Should be
		 *               the same as the screen resolution.
         */
        virtual void setTargetPlane(int width, int height);

        /**
         * Gets the target plane width.
         *
         * @return The target plane width.
         */
        virtual int getTargetPlaneWidth() const;

        /**
         * Gets the target plane height.
         *
         * @return The target plane height.
         */
        virtual int getTargetPlaneHeight() const;


        // Inherited from Graphics

        virtual void beginDraw();

        virtual void endDraw();

        virtual bool pushClipArea(gui::Rectangle area);

        virtual void popClipArea();

        virtual void drawImage(std::shared_ptr<const gui::ZImage> image,
                               int srcX,
                               int srcY,
                               int dstX,
                               int dstY,
                               int width,
                               int height);

        void drawImageScale(std::shared_ptr<const gui::ZImage> image,
                                    int srcX,
                                    int srcY,
                                    int dstX,
                                    int dstY,
                                    int width,
                                    int height,
                                    const gui::ZColor& color);

        virtual void drawPoint(int x, int y);

        virtual void drawLine(int x1, int y1, int x2, int y2);

        virtual void drawRectangle(const gui::Rectangle& rectangle);

        virtual void fillRectangle(const gui::Rectangle& rectangle);
        virtual void fillRectangle(int x, int y, int width, int height);

        virtual void setColor(const gui::ZColor& color);

        virtual const gui::ZColor& getColor() const;

        virtual void textureMixOn();
        virtual void textureMixOff();
    protected:
        int mWidth, mHeight;
        bool mAlpha;
        gui::ZColor mColor;
    };
}

#endif //GUICHAN_OPENGLGRAPHICS_H
