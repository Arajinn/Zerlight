//
// Created by tatiana on 27.09.18.
//

#include "ZCheckBox.h"
#include "Graphics.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace gui
{
    ZCheckBox::ZCheckBox()
        :ZWidget()
    {

    }

    ZCheckBox::~ZCheckBox()
    {

    }

    void ZCheckBox::init()
    {
        ZWidget::init();

        setSelected(false);

        setFocusable(true);
        addMouseListener(MouseListener::downcasted_shared_from_this<ZCheckBox>());
        addKeyListener(KeyListener::downcasted_shared_from_this<ZCheckBox>());
    }

    void ZCheckBox::init(const std::string &caption, bool selected)
    {
        ZWidget::init();

        setCaption(caption);
        setSelected(selected);

        setFocusable(true);
        addMouseListener(MouseListener::downcasted_shared_from_this<ZCheckBox>());
        addKeyListener(KeyListener::downcasted_shared_from_this<ZCheckBox>());

        adjustSize();
    }

    void ZCheckBox::draw_item(std::shared_ptr<Graphics> graphics)
    {
        drawBox(graphics);

        graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());

        const int h = getHeight() + getHeight() / 2;

        graphics->drawText(getCaption(), h - 2, 0);
    }

    void ZCheckBox::drawBox(std::shared_ptr<Graphics> graphics)
    {
        const int h = getHeight() - 2;
        const int alpha = getBaseColor().a;
        ZColor faceColor = getBaseColor();
        faceColor.a = alpha;
        ZColor highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        ZColor shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

        graphics->setColor(shadowColor);
        graphics->drawLine(1, 1, h, 1);
        graphics->drawLine(1, 1, 1, h);

        graphics->setColor(highlightColor);
        graphics->drawLine(h, 1, h, h);
        graphics->drawLine(1, h, h - 1, h);

        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(2, 2, h - 2, h - 2);

        graphics->setColor(getForegroundColor());

        if (isFocused())
        {
            graphics->drawRectangle(0, 0, h + 2, h + 2);
        }

        if (mSelected)
        {
            graphics->drawLine(3, 5, 3, h - 2);
            graphics->drawLine(4, 5, 4, h - 2);

            graphics->drawLine(5, h - 3, h - 2, 4);
            graphics->drawLine(5, h - 4, h - 4, 5);
        }
    }

    bool ZCheckBox::isSelected() const
    {
        return mSelected;
    }

    void ZCheckBox::setSelected(bool selected)
    {
        mSelected = selected;
    }

    const std::string& ZCheckBox::getCaption() const
    {
        return mCaption;
    }

    void ZCheckBox::setCaption(const std::string& caption)
    {
        mCaption = caption;
    }

    void ZCheckBox::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::Enter ||
            key.getValue() == Key::Space)
        {
            toggleSelected();
            keyEvent.consume();
        }
    }

    void ZCheckBox::mouseClicked(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Left)
        {
            toggleSelected();
        }
    }

    void ZCheckBox::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void ZCheckBox::adjustSize()
    {
        int height = getFont()->getHeight();

        setHeight(height);
        setWidth(getFont()->getWidth(mCaption) + height + height / 2);
    }

    void ZCheckBox::toggleSelected()
    {
        mSelected = !mSelected;
        distributeActionEvent();
    }
}