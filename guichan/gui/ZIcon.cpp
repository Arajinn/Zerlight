//
// Created by tatiana on 28.09.18.
//

#include "ZIcon.h"
#include "ZImage.h"
#include "Graphics.h"

namespace gui
{
    ZIcon::ZIcon()
        :ZWidget()
    {

    }

    void ZIcon::init()
    {
        ZWidget::init();

        mImage=nullptr;
        mInternalImage=false;

        setSize(0, 0);
    }

    void ZIcon::init(const std::string& filename)
    {
        ZWidget::init();

        mImage = ZImage::load(filename);
        mInternalImage = true;
        setSize(mImage->getWidth(), mImage->getHeight());
    }

    void ZIcon::init(std::shared_ptr<const ZImage> image)
    {
        ZWidget::init();

        mImage=image;
        mInternalImage=false;

        setSize(mImage->getWidth(), mImage->getHeight());
    }

    ZIcon::~ZIcon()
    {

    }

    void ZIcon::shutdown()
    {
        if (mInternalImage)
        {
            mImage=nullptr;
        }

        ZWidget::shutdown();
    }

    void ZIcon::setImage(std::shared_ptr<const ZImage> image)
    {
        if (mInternalImage)
        {
            mImage=nullptr;
        }

        mImage = image;
        mInternalImage = false;
        setSize(mImage->getWidth(), mImage->getHeight());
    }

    std::shared_ptr<const ZImage> ZIcon::getImage() const
    {
        return mImage;
    }

    void ZIcon::draw_item(std::shared_ptr<Graphics> graphics)
    {
        if (mImage != nullptr)
        {
            const int x = (getWidth() - mImage->getWidth()) / 2;
            const int y = (getHeight() - mImage->getHeight()) / 2;
            graphics->drawImage(mImage, x, y);
        }
    }

    Rectangle ZIcon::getChildrenArea()
    {
        return Rectangle(0, 0, getWidth(), getHeight());
    }
}