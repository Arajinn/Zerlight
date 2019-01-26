//
// Created by tatiana on 01.10.18.
//

#include "OpenGLGraphics.h"
#include "OpenGLImage.h"
#include "guichan/gui/Exception.h"

#include <GL/gl.h>

namespace sdl_interface
{
    OpenGLGraphics::OpenGLGraphics()
    {
        setTargetPlane(640, 480);
        mAlpha = false;
    }

    OpenGLGraphics::OpenGLGraphics(int width, int height)
    {
        setTargetPlane(width, height);
    }

    OpenGLGraphics::~OpenGLGraphics()
    {

    }

    void OpenGLGraphics::beginDraw()
    {
        glPushAttrib(
                GL_COLOR_BUFFER_BIT |
                GL_CURRENT_BIT |
                GL_DEPTH_BUFFER_BIT |
                GL_ENABLE_BIT |
                GL_FOG_BIT |
                GL_LIGHTING_BIT |
                GL_LINE_BIT |
                GL_POINT_BIT |
                GL_POLYGON_BIT |
                GL_SCISSOR_BIT |
                GL_STENCIL_BUFFER_BIT |
                GL_TEXTURE_BIT |
                GL_TRANSFORM_BIT |
                GL_POINT_BIT |
                GL_LINE_BIT
        );

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        glOrtho(0.0,
                (double)mWidth,
                (double)mHeight,
                0.0,
                -1.0,
                1.0);

        glDisable(GL_LIGHTING);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);

        glEnable(GL_SCISSOR_TEST);
        glPointSize(1.0);
        glLineWidth(1.0);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        pushClipArea(gui::Rectangle(0, 0, mWidth, mHeight));
    }

    void OpenGLGraphics::textureMixOn()
    {
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    }

    void OpenGLGraphics::textureMixOff()
    {
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
    }

    void OpenGLGraphics::endDraw()
    {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glMatrixMode(GL_TEXTURE);
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glPopAttrib();

        popClipArea();
    }

    bool OpenGLGraphics::pushClipArea(gui::Rectangle area)
    {
        bool result = Graphics::pushClipArea(area);

        glScissor(mClipStack.top().x,
                  mHeight - mClipStack.top().y - mClipStack.top().height,
                  mClipStack.top().width,
                  mClipStack.top().height);

        return result;
    }

    void OpenGLGraphics::popClipArea()
    {
        Graphics::popClipArea();

        if (mClipStack.empty())
        {
            return;
        }

        glScissor(mClipStack.top().x,
                  mHeight - mClipStack.top().y - mClipStack.top().height,
                  mClipStack.top().width,
                  mClipStack.top().height);
    }

    void OpenGLGraphics::setTargetPlane(int width, int height)
    {
        mWidth = width;
        mHeight = height;
    }

    void OpenGLGraphics::drawImage(std::shared_ptr<const gui::ZImage> image,
                                   int srcX,
                                   int srcY,
                                   int dstX,
                                   int dstY,
                                   int width,
                                   int height)
    {
        auto srcImage = std::dynamic_pointer_cast<const OpenGLImage>(image);

        if (srcImage == NULL)
        {
            throw GCN_EXCEPTION("Trying to draw an image of unknown format, must be an OpenGLImage.");
        }

        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const gui::ClipRectangle& top = mClipStack.top();

        dstX += top.xOffset;
        dstY += top.yOffset;

        // Find OpenGL texture coordinates
        float texX1 = srcX / (float)srcImage->getTextureWidth();
        float texY1 = srcY / (float)srcImage->getTextureHeight();
        float texX2 = (srcX+width) / (float)srcImage->getTextureWidth();
        float texY2 = (srcY+height) / (float)srcImage->getTextureHeight();

        glBindTexture(GL_TEXTURE_2D, srcImage->getTextureHandle());

        glEnable(GL_TEXTURE_2D);

        // Check if blending already is enabled
        if (!mAlpha)
        {
            glEnable(GL_BLEND);
        }

        // Draw a textured quad -- the image
        glBegin(GL_QUADS);
        glTexCoord2f(texX1, texY1);
        glVertex3i(dstX, dstY, 0);

        glTexCoord2f(texX1, texY2);
        glVertex3i(dstX, dstY + height, 0);

        glTexCoord2f(texX2, texY2);
        glVertex3i(dstX + width, dstY + height, 0);

        glTexCoord2f(texX2, texY1);
        glVertex3i(dstX + width, dstY, 0);
        glEnd();
        glDisable(GL_TEXTURE_2D);

        // Don't disable blending if the color has alpha
        if (!mAlpha)
        {
            glDisable(GL_BLEND);
        }
    }

    void OpenGLGraphics::drawPoint(int x, int y)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const gui::ClipRectangle& top = mClipStack.top();

        x += top.xOffset;
        y += top.yOffset;

        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
    }

    void OpenGLGraphics::drawLine(int x1, int y1, int x2, int y2)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const gui::ClipRectangle& top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        glBegin(GL_LINES);
        glVertex2f(x1 + 0.375f,
                   y1 + 0.375f);
        glVertex2f(x2 + 1.0f - 0.375f,
                   y2 + 1.0f - 0.375f);
        glEnd();

        glBegin(GL_POINTS);
        glVertex2f(x2 + 1.0f - 0.375f,
                   y2 + 1.0f - 0.375f);
        glEnd();

        glBegin(GL_POINTS);
        glVertex2f(x1 + 0.375f,
                   y1 + 0.375f);
        glEnd();
    }

    void OpenGLGraphics::drawRectangle(const gui::Rectangle& rectangle)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const gui::ClipRectangle& top = mClipStack.top();

        glBegin(GL_LINE_LOOP);
        glVertex2f(rectangle.x + top.xOffset,
                   rectangle.y + top.yOffset);
        glVertex2f(rectangle.x + rectangle.width + top.xOffset - 1.0f,
                   rectangle.y + top.yOffset + 0.375f);
        glVertex2f(rectangle.x + rectangle.width + top.xOffset - 1.0f,
                   rectangle.y + rectangle.height + top.yOffset);
        glVertex2f(rectangle.x + top.xOffset,
                   rectangle.y + rectangle.height + top.yOffset);
        glEnd();
    }

    void OpenGLGraphics::fillRectangle(const gui::Rectangle& rectangle)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const gui::ClipRectangle& top = mClipStack.top();

        glBegin(GL_QUADS);
        glVertex2i(rectangle.x + top.xOffset,
                   rectangle.y + top.yOffset);
        glVertex2i(rectangle.x + rectangle.width + top.xOffset,
                   rectangle.y + top.yOffset);
        glVertex2i(rectangle.x + rectangle.width + top.xOffset,
                   rectangle.y + rectangle.height + top.yOffset);
        glVertex2i(rectangle.x + top.xOffset,
                   rectangle.y + rectangle.height + top.yOffset);
        glEnd();
    }

    void OpenGLGraphics::fillRectangle(int x, int y, int width, int height)
    {
        fillRectangle(gui::Rectangle(x, y, width, height));
    }

    void OpenGLGraphics::setColor(const gui::ZColor& color)
    {
        mColor = color;
        glColor4ub((GLubyte) color.r,
                   (GLubyte) color.g,
                   (GLubyte) color.b,
                   (GLubyte) color.a);

        mAlpha = color.a != 255;

        if (mAlpha)
        {
            glEnable(GL_BLEND);
        }
    }

    const gui::ZColor& OpenGLGraphics::getColor() const
    {
        return mColor;
    }

    int OpenGLGraphics::getTargetPlaneWidth() const
    {
        return mWidth;
    }

    int OpenGLGraphics::getTargetPlaneHeight() const
    {
        return mHeight;
    }

    void OpenGLGraphics::drawImageScale(std::shared_ptr<const gui::ZImage> image,
                                   int srcX,
                                   int srcY,
                                   int dstX,
                                   int dstY,
                                   int width,
                                   int height,
                                   const gui::ZColor& color,
                                   bool xFlip)
    {
        auto srcImage = std::dynamic_pointer_cast<const OpenGLImage>(image);

        if (srcImage == NULL)
        {
            throw GCN_EXCEPTION("Trying to draw an image of unknown format, must be an OpenGLImage.");
        }

        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const gui::ClipRectangle& top = mClipStack.top();

        dstX += top.xOffset;
        dstY += top.yOffset;

        // Find OpenGL texture coordinates
        float texX1,texX2;
        if (xFlip)
        {
            texX1=1.0f;
            texX2=0.0f;
        }
        else
        {
            texX1=0.0f;
            texX2=1.0f;
        }

        float texY1 = 0.0f;
        float texY2 = 1.0f;

        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, srcImage->getTextureHandle());

        if (color.valid())
            glColor3f(double(color.r)/255.0, double(color.g)/255.0, double(color.b)/255.0);

        // Check if blending already is enabled
        if (!mAlpha)
        {
            glEnable(GL_BLEND);
        }

        // Draw a textured quad -- the image
        glBegin(GL_QUADS);

        glTexCoord2f(texX1, texY1);
        glVertex3f(srcX, srcY, 0);

        glTexCoord2f(texX1, texY2);
        glVertex3f(srcX, dstY, 0);

        glTexCoord2f(texX2, texY2);
        glVertex3f(dstX, dstY, 0);

        glTexCoord2f(texX2, texY1);
        glVertex3f(dstX, srcY, 0);

        glEnd();
        glDisable(GL_TEXTURE_2D);

        // Don't disable blending if the color has alpha
        if (!mAlpha)
        {
            glDisable(GL_BLEND);
        }
    }
}