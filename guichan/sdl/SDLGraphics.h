//
// Created by tatiana on 27.09.18.
//

#ifndef GUICHAN_SDLGRAPHICS_H
#define GUICHAN_SDLGRAPHICS_H

#include "guichan/gui/Graphics.h"

#include <SDL2/SDL_rect.h>

class SDL_Renderer;
class SDL_Surface;
class SDL_Texture;

namespace gui
{
    class ZImage;
}

namespace sdl_interface {
    /**
     * SDL implementation of the Graphics.
     */
    class SDLGraphics : public gui::Graphics{
    public:
        /**
         * Constructor.
         */
        SDLGraphics();

        /**
         * Sets the target SDL_Surface to draw to. The target can be any
         * SDL_Surface. This funtion also pushes a clip areas corresponding to
         * the dimension of the target.
         *
         * @param target the target to draw to.
         */
        virtual void setTarget(std::shared_ptr<SDL_Renderer> renderer, size_t width, size_t height);

        /**
         * Gets the target SDL_Surface.
         *
         * @return the target SDL_Surface.
         */
        virtual std::shared_ptr<SDL_Surface> getTarget() const;

        /**
         * Draws an SDL_Surface on the target surface. Normaly you'll
         * use drawImage, but if you want to write SDL specific code
         * this function might come in handy.
         *
         * NOTE: The clip areas will be taken into account.
         */
        virtual void drawSDLSurface(std::shared_ptr<SDL_Surface> surface, SDL_Rect source, SDL_Rect destination);


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

        virtual void drawPoint(int x, int y);

        virtual void drawLine(int x1, int y1, int x2, int y2);

        virtual void drawRectangle(const gui::Rectangle& rectangle);

        virtual void fillRectangle(const gui::Rectangle& rectangle);

        virtual void setColor(const gui::ZColor& color);

        virtual const gui::ZColor& getColor() const;

    protected:
        /**
         * Draws a horizontal line.
         *
         * @param x1 the start coordinate of the line.
         * @param y the y coordinate of the line.
         * @param x2 the end coordinate of the line.
         */
        virtual void drawHLine(int x1, int y, int x2);

        /**
         * Draws a vertical line.
         *
         * @param x the x coordinate of the line.
         * @param y1 the start coordinate of the line.
         * @param y2 the end coordinate of the line.
         */
        virtual void drawVLine(int x, int y1, int y2);

        std::shared_ptr<SDL_Surface> mTarget;
        gui::ZColor mColor;
        bool mAlpha;

        std::shared_ptr<SDL_Renderer> mRenderer;
        std::shared_ptr<SDL_Texture> mTargetTexture;
    };
}

#endif //GUICHAN_SDLGRAPHICS_H
