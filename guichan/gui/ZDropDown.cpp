//
// Created by tatiana on 28.09.18.
//

#include "ZDropDown.h"
#include "FocusHandler.h"
#include "ZScrollArea.h"
#include "ZListBox.h"
#include "Graphics.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "ListModel.h"
#include "FocusHandler.h"
#include "SelectionEvent.h"
#include "Exception.h"

namespace gui
{
    ZDropDown::ZDropDown()
    {

    }

    void ZDropDown::init(std::shared_ptr<ListModel> listModel, std::shared_ptr<ZScrollArea> scrollArea, std::shared_ptr<ZListBox> listBox)
    {
        ZWidget::init();

        setWidth(100);
        setFocusable(true);
        mDroppedDown = false;
        mPushed = false;
        mIsDragged = false;

        mInternalFocusHandler=std::make_shared<FocusHandler>();
        setInternalFocusHandler(mInternalFocusHandler);

        mInternalScrollArea = (scrollArea == nullptr);
        mInternalListBox = (listBox == nullptr);

        if (mInternalScrollArea)
        {
            mScrollArea = std::make_shared<ZScrollArea>();
        }
        else
        {
            mScrollArea = scrollArea;
        }

        if (mInternalListBox)
        {
            mListBox = std::make_shared<ZListBox>();
        }
        else
        {
            mListBox = listBox;
        }

        mScrollArea->setContent(mListBox);
        add_child(mScrollArea);

        mListBox->addActionListener(ActionListener::downcasted_shared_from_this<ZDropDown>());
        mListBox->addSelectionListener(SelectionListener::downcasted_shared_from_this<ZDropDown>());

        setListModel(listModel);

        if (mListBox->getSelected() < 0)
        {
            mListBox->setSelected(0);
        }

        addMouseListener(MouseListener::downcasted_shared_from_this<ZDropDown>());
        addKeyListener(KeyListener::downcasted_shared_from_this<ZDropDown>());
        addFocusListener(FocusListener::downcasted_shared_from_this<ZDropDown>());

        adjustHeight();
    }

    ZDropDown::~ZDropDown()
    {

    }

    void ZDropDown::shutdown()
    {
        if (widgetExists(mListBox))
        {
            mListBox->removeActionListener(ActionListener::downcasted_shared_from_this<ZDropDown>());
            mListBox->removeSelectionListener(SelectionListener::downcasted_shared_from_this<ZDropDown>());
        }

        mScrollArea=nullptr;
        mListBox=nullptr;

        setInternalFocusHandler(nullptr);

        mSelectionListeners.clear();

        ZWidget::shutdown();
    }

    void ZDropDown::draw_item(std::shared_ptr<Graphics> graphics)
    {
        int h;

        if (mDroppedDown)
        {
            h = mFoldedUpHeight;
        }
        else
        {
            h = getHeight();
        }

        ZColor faceColor = getBaseColor();
        ZColor highlightColor, shadowColor;
        int alpha = getBaseColor().a;
        highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

        // Draw a border.
        graphics->setColor(shadowColor);
        graphics->drawLine(0, 0, getWidth() - 1, 0);
        graphics->drawLine(0, 1, 0, h - 2);
        graphics->setColor(highlightColor);
        graphics->drawLine(getWidth() - 1, 1, getWidth() - 1, h - 1);
        graphics->drawLine(0, h - 1, getWidth() - 1, h - 1);

        // Push a clip area so the other drawings don't need to worry
        // about the border.
        graphics->pushClipArea(Rectangle(1, 1, getWidth() - 2, h - 2));
        const Rectangle currentClipArea = graphics->getCurrentClipArea();

        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(0, 0, currentClipArea.width, currentClipArea.height);

        if (isFocused())
        {
            graphics->setColor(getSelectionColor());
            graphics->fillRectangle(0,
                                    0,
                                    currentClipArea.width - currentClipArea.height,
                                    currentClipArea.height);
            graphics->setColor(getForegroundColor());
        }

        if (mListBox->getListModel() && mListBox->getSelected() >= 0)
        {
            graphics->setColor(getForegroundColor());
            graphics->setFont(getFont());

            graphics->drawText(mListBox->getListModel()->getElementAt(mListBox->getSelected()), 1, 0);
        }

        // Push a clip area before drawing the button.
        graphics->pushClipArea(Rectangle(currentClipArea.width - currentClipArea.height,
                                         0,
                                         currentClipArea.height,
                                         currentClipArea.height));
        drawButton(graphics);
        graphics->popClipArea();
        graphics->popClipArea();

        if (mDroppedDown)
        {
            // Draw a border around the children.
            graphics->setColor(shadowColor);
            graphics->drawRectangle(0,
                                    mFoldedUpHeight,
                                    getWidth(),
                                    getHeight() - mFoldedUpHeight);
            //drawChildren(graphics);
        }
    }

    void ZDropDown::drawButton(std::shared_ptr<Graphics> graphics)
    {
        ZColor faceColor, highlightColor, shadowColor;
        int offset;
        int alpha = getBaseColor().a;

        if (mPushed)
        {
            faceColor = getBaseColor() - 0x303030;
            faceColor.a = alpha;
            highlightColor = faceColor - 0x303030;
            highlightColor.a = alpha;
            shadowColor = faceColor + 0x303030;
            shadowColor.a = alpha;
            offset = 1;
        }
        else
        {
            faceColor = getBaseColor();
            faceColor.a = alpha;
            highlightColor = faceColor + 0x303030;
            highlightColor.a = alpha;
            shadowColor = faceColor - 0x303030;
            shadowColor.a = alpha;
            offset = 0;
        }

        const Rectangle currentClipArea = graphics->getCurrentClipArea();
        graphics->setColor(highlightColor);
        graphics->drawLine(0,
                           0,
                           currentClipArea.width - 1,
                           0);
        graphics->drawLine(0,
                           1,
                           0,
                           currentClipArea.height - 1);
        graphics->setColor(shadowColor);
        graphics->drawLine(currentClipArea.width - 1,
                           1,
                           currentClipArea.width - 1,
                           currentClipArea.height - 1);
        graphics->drawLine(1,
                           currentClipArea.height - 1,
                           currentClipArea.width - 2,
                           currentClipArea.height - 1);

        graphics->setColor(faceColor);
        graphics->fillRectangle(1,
                                1,
                                currentClipArea.width - 2,
                                currentClipArea.height - 2);

        graphics->setColor(getForegroundColor());

        int i;
        int n = currentClipArea.height / 3;
        int dx = currentClipArea.height / 2;
        int dy = (currentClipArea.height * 2) / 3;
        for (i = 0; i < n; i++)
        {
            graphics->drawLine(dx - i + offset,
                               dy - i + offset,
                               dx + i + offset,
                               dy - i + offset);
        }
    }

    int ZDropDown::getSelected() const
    {
        return mListBox->getSelected();
    }

    void ZDropDown::setSelected(int selected)
    {
        if (selected >= 0)
        {
            mListBox->setSelected(selected);
        }
    }

    void ZDropDown::keyPressed(KeyEvent& keyEvent)
    {
        if (keyEvent.isConsumed())
            return;

        Key key = keyEvent.getKey();

        if ((key.getValue() == Key::Enter || key.getValue() == Key::Space)
            && !mDroppedDown)
        {
            dropDown();
            keyEvent.consume();
        }
        else if (key.getValue() == Key::Up)
        {
            setSelected(getSelected() - 1);
            keyEvent.consume();
        }
        else if (key.getValue() == Key::Down)
        {
            setSelected(getSelected() + 1);
            keyEvent.consume();
        }
    }

    void ZDropDown::mousePressed(MouseEvent& mouseEvent)
    {
        // If we have a mouse press on the widget.
        if (0 <= mouseEvent.getY()
            && mouseEvent.getY() < getHeight()
            && mouseEvent.getX() >= 0
            && mouseEvent.getX() < getWidth()
            && mouseEvent.getButton() == MouseEvent::Left
            && !mDroppedDown
            && mouseEvent.getSource() == ZWidget::downcasted_shared_from_this<ZDropDown>())
        {
            mPushed = true;
            dropDown();
            requestModalMouseInputFocus();
        }
            // Fold up the listbox if the upper part is clicked after fold down
        else if (0 <= mouseEvent.getY()
                 && mouseEvent.getY() < mFoldedUpHeight
                 && mouseEvent.getX() >= 0
                 && mouseEvent.getX() < getWidth()
                 && mouseEvent.getButton() == MouseEvent::Left
                 && mDroppedDown
                 && mouseEvent.getSource() == ZWidget::downcasted_shared_from_this<ZDropDown>())
        {
            mPushed = false;
            foldUp();
            releaseModalMouseInputFocus();
        }
            // If we have a mouse press outside the widget
        else if (0 > mouseEvent.getY()
                 || mouseEvent.getY() >= getHeight()
                 || mouseEvent.getX() < 0
                 || mouseEvent.getX() >= getWidth())
        {
            mPushed = false;
            foldUp();
        }
    }

    void ZDropDown::mouseReleased(MouseEvent& mouseEvent)
    {
        if (mIsDragged)
        {
            mPushed = false;
        }

        // Released outside of widget. Can happen when we have modal input focus.
        if ((0 > mouseEvent.getY()
             || mouseEvent.getY() >= getHeight()
             || mouseEvent.getX() < 0
             || mouseEvent.getX() >= getWidth())
            && mouseEvent.getButton() == MouseEvent::Left
            && isModalMouseInputFocused())
        {
            releaseModalMouseInputFocus();

            if (mIsDragged)
            {
                foldUp();
            }
        }
        else if (mouseEvent.getButton() == MouseEvent::Left)
        {
            mPushed = false;
        }

        mIsDragged = false;
    }

    void ZDropDown::mouseDragged(MouseEvent& mouseEvent)
    {
        mIsDragged = true;

        mouseEvent.consume();
    }

    void ZDropDown::setListModel(std::shared_ptr<ListModel> listModel)
    {
        mListBox->setListModel(listModel);

        if (mListBox->getSelected() < 0)
        {
            mListBox->setSelected(0);
        }

        adjustHeight();
    }

    std::shared_ptr<ListModel> ZDropDown::getListModel() const
    {
        return mListBox->getListModel();
    }

    void ZDropDown::adjustHeight()
    {
        if (mScrollArea == nullptr)
        {
            throw GCN_EXCEPTION("Scroll area has been deleted.");
        }

        if (mListBox == nullptr)
        {
            throw GCN_EXCEPTION("List box has been deleted.");
        }

        int listBoxHeight = mListBox->getHeight();

        // We add 2 for the border
        int h2 = getFont()->getHeight() + 2;

        setHeight(h2);

        // The addition/subtraction of 2 compensates for the seperation lines
        // seperating the selected element view and the scroll area.

        if (mDroppedDown && getParent())
        {
            int h = getParent()->getChildrenArea().height - getY();

            if (listBoxHeight > h - h2 - 2)
            {
                mScrollArea->setHeight(h - h2 - 2);
                setHeight(h);
            }
            else
            {
                setHeight(listBoxHeight + h2 + 2);
                mScrollArea->setHeight(listBoxHeight);
            }
        }

        mScrollArea->setWidth(getWidth());
        mScrollArea->setPosition(0, 0);
        // Resize the ListBox to exactly fit the ScrollArea.
        mListBox->setWidth(mScrollArea->getChildrenArea().width);
        mListBox->setPosition(0, 0);
    }

    void ZDropDown::dropDown()
    {
        if (!mDroppedDown)
        {
            mDroppedDown = true;
            mFoldedUpHeight = getHeight();
            adjustHeight();

            if (getParent())
            {
                getParent()->moveToTop(ZWidget::downcasted_shared_from_this<ZDropDown>());
            }
        }

        mListBox->requestFocus();
    }

    void ZDropDown::foldUp()
    {
        if (mDroppedDown)
        {
            mDroppedDown = false;
            adjustHeight();
            mInternalFocusHandler->focusNone();
        }
    }

    void ZDropDown::focusLost(const Event& event)
    {
        foldUp();
        mInternalFocusHandler->focusNone();
    }


    void ZDropDown::death(const Event& event)
    {
        if (event.getSource() == mScrollArea)
        {
            mScrollArea = nullptr;
        }
    }

    void ZDropDown::action(const ActionEvent& actionEvent)
    {
        foldUp();
        releaseModalMouseInputFocus();
        distributeActionEvent();
    }

    Rectangle ZDropDown::getChildrenArea()
    {
        if (mDroppedDown)
        {
            // Calculate the children area (with the one pixel border in mind)
            return Rectangle(1,
                             mFoldedUpHeight + 1,
                             getWidth() - 2,
                             getHeight() - mFoldedUpHeight - 2);
        }

        return Rectangle();
    }

    void ZDropDown::setBaseColor(const ZColor& color)
    {
        if (mInternalScrollArea)
        {
            mScrollArea->setBaseColor(color);
        }

        if (mInternalListBox)
        {
            mListBox->setBaseColor(color);
        }

        ZWidget::setBaseColor(color);
    }

    void ZDropDown::setBackgroundColor(const ZColor& color)
    {
        if (mInternalScrollArea)
        {
            mScrollArea->setBackgroundColor(color);
        }

        if (mInternalListBox)
        {
            mListBox->setBackgroundColor(color);
        }

        ZWidget::setBackgroundColor(color);
    }

    void ZDropDown::setForegroundColor(const ZColor& color)
    {
        if (mInternalScrollArea)
        {
            mScrollArea->setForegroundColor(color);
        }

        if (mInternalListBox)
        {
            mListBox->setForegroundColor(color);
        }

        ZWidget::setForegroundColor(color);
    }

    void ZDropDown::setFont(std::shared_ptr<Font> font)
    {
        if (mInternalScrollArea)
        {
            mScrollArea->setFont(font);
        }

        if (mInternalListBox)
        {
            mListBox->setFont(font);
        }

        ZWidget::setFont(font);
    }

    void ZDropDown::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        if (isFocused() && mouseEvent.getSource() == ZWidget::downcasted_shared_from_this<ZDropDown>())
        {
            mouseEvent.consume();

            if (mListBox->getSelected() > 0)
            {
                mListBox->setSelected(mListBox->getSelected() - 1);
            }
        }
    }

    void ZDropDown::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        if (isFocused() && mouseEvent.getSource() == ZWidget::downcasted_shared_from_this<ZDropDown>())
        {
            mouseEvent.consume();

            mListBox->setSelected(mListBox->getSelected() + 1);
        }
    }

    void ZDropDown::setSelectionColor(const ZColor& color)
    {
        ZWidget::setSelectionColor(color);

        if (mInternalListBox)
        {
            mListBox->setSelectionColor(color);
        }
    }

    void ZDropDown::valueChanged(const SelectionEvent& event)
    {
        distributeValueChangedEvent();
    }

    void ZDropDown::addSelectionListener(std::shared_ptr<SelectionListener> selectionListener)
    {
        mSelectionListeners.push_back(selectionListener);
    }

    void ZDropDown::removeSelectionListener(std::shared_ptr<SelectionListener> selectionListener)
    {
        mSelectionListeners.remove(selectionListener);
    }

    void ZDropDown::distributeValueChangedEvent()
    {
        for (auto listener : mSelectionListeners)
        {
            SelectionEvent event(ZWidget::downcasted_shared_from_this<ZDropDown>());
            listener->valueChanged(event);
        }
    }
}