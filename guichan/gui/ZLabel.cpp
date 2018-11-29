//
// Created by tatiana on 28.09.18.
//

#include "ZLabel.h"
#include "Exception.h"

namespace gui
{
    ZLabel::ZLabel()
        :ZWidget()
    {

    }

    void ZLabel::init()
    {
        ZWidget::init();

        mAlignment = Graphics::Left;
    }

    void ZLabel::init(const std::string& caption)
    {
        ZWidget::init();

        mCaption = caption;
        mAlignment = Graphics::Left;

        setWidth(getFont()->getWidth(caption));
        setHeight(getFont()->getHeight());
    }

    const std::string &ZLabel::getCaption() const
    {
        return mCaption;
    }

    void ZLabel::setCaption(const std::string& caption)
    {
        mCaption = caption;
    }

    void ZLabel::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment ZLabel::getAlignment() const
    {
        return mAlignment;
    }

    void ZLabel::draw_item(std::shared_ptr<Graphics> graphics)
    {
        int textX;
        int textY = getHeight() / 2 - getFont()->getHeight() / 2;

        switch (getAlignment())
        {
            case Graphics::Left:
                textX = 0;
                break;
            case Graphics::Center:
                textX = getWidth() / 2;
                break;
            case Graphics::Right:
                textX = getWidth();
                break;
            default:
                throw GCN_EXCEPTION("Unknown alignment.");
        }

        graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());
        graphics->drawText(getCaption(), textX, textY, getAlignment());
    }

    void ZLabel::adjustSize()
    {
        setWidth(getFont()->getWidth(getCaption()));
        setHeight(getFont()->getHeight());
    }
}