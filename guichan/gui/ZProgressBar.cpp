//
// Created by tatiana on 06.10.18.
//

#include "ZProgressBar.h"
#include "Graphics.h"

#include <sstream>

namespace gui
{
    ZProgressBar::ZProgressBar()
        :ZWidget()
    {

    }

    void ZProgressBar::init(float progress, int width, int height, gui::ZColor color)
    {
        ZWidget::init();

        mProgress=0.0f;

        mAlignment=Graphics::Center;

        setProgress(progress);
        setSize(width, height);
    }

    ZProgressBar::~ZProgressBar()
    {

    }

    void ZProgressBar::shutdown()
    {
        ZWidget::shutdown();
    }

    void ZProgressBar::adjustSize()
    {
        adjustWidth();
        adjustHeight();
    }

    void ZProgressBar::adjustWidth()
    {
        setWidth(getFont()->getWidth(mText) + 8);
    }

    void ZProgressBar::adjustHeight()
    {
        setHeight(getFont()->getHeight() + 8);
    }

    void ZProgressBar::fontChanged()
    {
        if (!mText.empty())
            adjustHeight();
    }

    void ZProgressBar::draw_item(std::shared_ptr<Graphics> graphics)
    {
        ZColor shadowColor = getBaseColor() - 0x252525;
        int alpha = getBaseColor().a;
        shadowColor.a = alpha;

        graphics->setColor(shadowColor);
        graphics->fillRectangle(0, 0, getWidth(), getHeight());

        if (mProgress > 0)
        {
            ZColor faceColor = getBaseColor();
            graphics->setColor(faceColor);
            int progress_width=int(mProgress*float(getWidth())) - 8;
            graphics->fillRectangle(Rectangle(4, 4, progress_width, getHeight() - 8));
        }

        if (!mText.empty())
        {
            auto boldFont = getFont();

            const int textX = getWidth() / 2;
            const int textY = (getHeight() - boldFont->getHeight()) / 2;

            graphics->setFont(boldFont);
            graphics->drawText(mText, textX, textY, getAlignment());
        }
    }

    void ZProgressBar::setAlignment(Graphics::Alignment alignment)
    {
        mAlignment = alignment;
    }

    Graphics::Alignment ZProgressBar::getAlignment() const
    {
        return mAlignment;
    }

    void ZProgressBar::setProgress(const float progress)
    {
        if (progress < 0.0f)
            mProgress = 0.0;
        else if (progress > 1.0f)
            mProgress = 1.0;
        else
            mProgress = progress;

        std::string str;
        std::ostringstream os(str);
        os << (int)(mProgress * 100) << "%";
        mText = os.str();
    }

    float ZProgressBar::getProgress() const
    {
        return mProgress;
    }

    void ZProgressBar::setText(const std::string &text)
    {
        mText = text;
    }

    const std::string& ZProgressBar::text() const
    {
        return mText;
    }

    void ZProgressBar::reset()
    {
        mProgress = 0.0f;
    }
}