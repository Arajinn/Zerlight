//
// Created by tatiana on 26.09.18.
//

#include "ZImage.h"
#include "ImageLoader.h"
#include "Exception.h"

namespace gui
{
    std::shared_ptr<ImageLoader> ZImage::mImageLoader = nullptr;

    ZImage::ZImage()
    {
    }

    ZImage::~ZImage()
    {
    }

    void ZImage::setImageLoader(std::shared_ptr<ImageLoader> imageLoader)
    {
        mImageLoader = imageLoader;
    }

    std::shared_ptr<ImageLoader> ZImage::getImageLoader()
    {
        return mImageLoader;
    }

    std::shared_ptr<ZImage> ZImage::load(const std::string& filename, bool convertToDisplayFormat)
    {
        if (mImageLoader == nullptr)
        {
            throw GCN_EXCEPTION("Trying to load an image but no image loader is set.");
        }

        return mImageLoader->load(filename, convertToDisplayFormat);
    }
}