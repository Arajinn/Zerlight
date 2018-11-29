//
// Created by tatiana on 06.10.18.
//

#ifndef GUICHAN_ZPROGRESSBAR_H
#define GUICHAN_ZPROGRESSBAR_H

#include "guichan/gui/ZWidget.h"
#include "guichan/gui/Graphics.h"

namespace gui {
    class ZProgressBar : public ZWidget{
    public:
        ZProgressBar();
        ~ZProgressBar();

        virtual void init(float progress = 0.0f, int width = 40, int height = 7, ZColor color = ZColor(150, 150, 150));
        virtual void shutdown();

        void draw_item(std::shared_ptr<Graphics> graphics);

        void setProgress(const float progress);
        float getProgress() const;

        void setText(const std::string &text);
        const std::string &text() const;

        void setAlignment(Graphics::Alignment alignment);
        Graphics::Alignment getAlignment() const;

        void reset();

        virtual void adjustSize();

        virtual void adjustWidth();

        virtual void adjustHeight();

        void fontChanged();

    private:
        float mProgress;

        std::string mText;
        Graphics::Alignment mAlignment;
    };
}

#endif //GUICHAN_ZPROGRESSBAR_H
