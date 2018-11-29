//
// Created by tatiana on 26.09.18.
//

#include "Graphics.h"
#include "Font.h"
#include "Exception.h"
#include "ZImage.h"

namespace gui
{
    Graphics::Graphics()
    {
        mFont = nullptr;
    }

    Graphics::~Graphics()
    {

    }

    void Graphics::beginDraw()
    {

    }

    void Graphics::endDraw()
    {

    }

    void Graphics::drawRectangle(int x, int y, int width, int height)
    {
        drawRectangle(Rectangle(x, y, width, height));
    }

    void Graphics::fillRectangle(int x, int y, int width, int height)
    {
        fillRectangle(Rectangle(x, y, width, height));
    }

    bool Graphics::pushClipArea(Rectangle area)
    {
        // Ignore area with a negate width or height
        // by simple pushing an empty clip area
        // to the stack.
        if (area.isEmpty())
        {
            ClipRectangle carea;
            mClipStack.push(carea);
            return true;
        }

        if (mClipStack.empty())
        {
            ClipRectangle carea;
            carea.x = area.x;
            carea.y = area.y;
            carea.width = area.width;
            carea.height = area.height;
            carea.xOffset = area.x;
            carea.yOffset = area.y;
            mClipStack.push(carea);
            return true;
        }

        const ClipRectangle &top = mClipStack.top();
        ClipRectangle carea;
        carea = area;
        carea.xOffset = top.xOffset + carea.x;
        carea.yOffset = top.yOffset + carea.y;
        carea.x += top.xOffset;
        carea.y += top.yOffset;

        carea = top.intersection(carea);

        mClipStack.push(carea);

        return !carea.isEmpty();
    }

    void Graphics::popClipArea()
    {

        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Tried to pop clip area from empty stack.");
        }

        mClipStack.pop();
    }

    const ClipRectangle& Graphics::getCurrentClipArea()
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("The clip area stack is empty.");
        }

        return mClipStack.top();
    }

    void Graphics::drawImage(std::shared_ptr<const ZImage> image, int dstX, int dstY)
    {
        drawImage(image, 0, 0, dstX, dstY, image->getWidth(), image->getHeight());
    }

    void Graphics::setFont(std::shared_ptr<Font> font)
    {
        mFont = font;
    }

    void Graphics::drawText(const std::string& text, int x, int y, Alignment alignment)
    {
        if (mFont == nullptr)
        {
            throw GCN_EXCEPTION("No font set.");
        }

        switch (alignment)
        {
            case Left:
                mFont->drawString(shared_from_this(), text, x, y);
                break;
            case Center:
                mFont->drawString(shared_from_this(), text, x - mFont->getWidth(text) / 2, y);
                break;
            case Right:
                mFont->drawString(shared_from_this(), text, x - mFont->getWidth(text), y);
                break;
            default:
                throw GCN_EXCEPTION("Unknown alignment.");
        }
    }
}