//
// Created by tatiana on 28.09.18.
//

#include "ZListBox.h"
#include "Graphics.h"
#include "ListModel.h"
#include "KeyEvent.h"
#include "MouseEvent.h"
#include "SelectionEvent.h"
#include "SelectionListener.h"

namespace gui
{
    ZListBox::ZListBox()
        :ZWidget()
    {

    }

    void ZListBox::init()
    {
        ZWidget::init();

        mSelected=-1;
        mListModel=nullptr;
        mWrappingEnabled=false;

        setWidth(100);
        setFocusable(true);

        addMouseListener(MouseListener::downcasted_shared_from_this<MouseListener>());
        addKeyListener(KeyListener::downcasted_shared_from_this<KeyListener>());
        addFocusListener(FocusListener::downcasted_shared_from_this<FocusListener>());
    }

    void ZListBox::init(std::shared_ptr<ListModel> listModel)
    {
        ZWidget::init();

        mSelected=-1;
        mWrappingEnabled=false;

        setWidth(100);
        setListModel(listModel);
        setFocusable(true);

        addMouseListener(MouseListener::downcasted_shared_from_this<MouseListener>());
        addKeyListener(KeyListener::downcasted_shared_from_this<KeyListener>());
        addFocusListener(FocusListener::downcasted_shared_from_this<FocusListener>());
    }

    void ZListBox::shutdown()
    {
        mSelectionListeners.clear();

        ZWidget::shutdown();
    }

    ZListBox::~ZListBox()
    {

    }

    void ZListBox::draw_item(std::shared_ptr<Graphics> graphics)
    {
        graphics->setColor(getBackgroundColor());
        graphics->fillRectangle(0, 0, getWidth(), getHeight());

        if (mListModel == nullptr)
        {
            return;
        }

        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());

        // Check the current clip area so we don't draw unnecessary items
        // that are not visible.
        const ClipRectangle currentClipArea = graphics->getCurrentClipArea();
        int rowHeight = getRowHeight();

        // Calculate the number of rows to draw by checking the clip area.
        // The addition of two makes covers a partial visible row at the top
        // and a partial visible row at the bottom.
        int numberOfRows = currentClipArea.height / rowHeight + 2;

        if (numberOfRows > mListModel->getNumberOfElements())
        {
            numberOfRows = mListModel->getNumberOfElements();
        }

        // Calculate which row to start drawing. If the list box
        // has a negative y coordinate value we should check if
        // we should drop rows in the begining of the list as
        // they might not be visible. A negative y value is very
        // common if the list box for instance resides in a scroll
        // area and the user has scrolled the list box downwards.
        int startRow;
        if (getY() < 0)
        {
            startRow = -1 * (getY() / rowHeight);
        }
        else
        {
            startRow = 0;
        }

        int i;
        // The y coordinate where we start to draw the text is
        // simply the y coordinate multiplied with the font height.
        int y = rowHeight * startRow;
        for (i = startRow; i < startRow + numberOfRows; ++i)
        {
            if (i == mSelected)
            {
                graphics->setColor(getSelectionColor());
                graphics->fillRectangle(0, y, getWidth(), rowHeight);
                graphics->setColor(getForegroundColor());
            }

            // If the row height is greater than the font height we
            // draw the text with a center vertical alignment.
            if (rowHeight > getFont()->getHeight())
            {
                graphics->drawText(mListModel->getElementAt(i), 1, y + rowHeight / 2 - getFont()->getHeight() / 2);
            }
            else
            {
                graphics->drawText(mListModel->getElementAt(i), 1, y);
            }

            y += rowHeight;
        }
    }

    void ZListBox::logic_item()
    {
        adjustSize();
    }

    int ZListBox::getSelected() const
    {
        return mSelected;
    }

    void ZListBox::setSelected(int selected)
    {
        if (mListModel == nullptr)
        {
            mSelected = -1;
        }
        else
        {
            if (selected < 0)
            {
                mSelected = -1;
            }
            else if (selected >= mListModel->getNumberOfElements())
            {
                mSelected = mListModel->getNumberOfElements() - 1;
            }
            else
            {
                mSelected = selected;
            }
        }

        Rectangle scroll;

        if (mSelected < 0)
        {
            scroll.y = 0;
        }
        else
        {
            scroll.y = getRowHeight() * mSelected;
        }

        scroll.height = getRowHeight();
        showPart(scroll);

        distributeValueChangedEvent();
    }

    void ZListBox::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::Enter || key.getValue() == Key::Space)
        {
            distributeActionEvent();
            keyEvent.consume();
        }
        else if (key.getValue() == Key::Up)
        {
            setSelected(mSelected - 1);

            if (mSelected == -1)
            {
                if (mWrappingEnabled)
                {
                    setSelected(getListModel()->getNumberOfElements() - 1);
                }
                else
                {
                    setSelected(0);
                }
            }

            keyEvent.consume();
        }
        else if (key.getValue() == Key::Down)
        {
            if (mWrappingEnabled
                && getSelected() == getListModel()->getNumberOfElements() - 1)
            {
                setSelected(0);
            }
            else
            {
                setSelected(getSelected() + 1);
            }

            keyEvent.consume();
        }
        else if (key.getValue() == Key::Home)
        {
            setSelected(0);
            keyEvent.consume();
        }
        else if (key.getValue() == Key::End)
        {
            setSelected(getListModel()->getNumberOfElements() - 1);
            keyEvent.consume();
        }
    }

    void ZListBox::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Left)
        {
            setSelected(mouseEvent.getY() / getRowHeight());
            distributeActionEvent();
        }
    }

    void ZListBox::mouseWheelMovedUp(MouseEvent& mouseEvent)
    {
        if (isFocused())
        {
            if (getSelected() > 0 )
            {
                setSelected(getSelected() - 1);
            }

            mouseEvent.consume();
        }
    }

    void ZListBox::mouseWheelMovedDown(MouseEvent& mouseEvent)
    {
        if (isFocused())
        {
            setSelected(getSelected() + 1);

            mouseEvent.consume();
        }
    }

    void ZListBox::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void ZListBox::setListModel(std::shared_ptr<ListModel> listModel)
    {
        mSelected = -1;
        mListModel = listModel;
        adjustSize();
    }

    std::shared_ptr<ListModel> ZListBox::getListModel() const
    {
        return mListModel;
    }

    void ZListBox::adjustSize()
    {
        if (mListModel != nullptr)
        {
            setHeight(getRowHeight() * mListModel->getNumberOfElements());
        }
    }

    bool ZListBox::isWrappingEnabled() const
    {
        return mWrappingEnabled;
    }

    void ZListBox::setWrappingEnabled(bool wrappingEnabled)
    {
        mWrappingEnabled = wrappingEnabled;
    }

    void ZListBox::addSelectionListener(std::shared_ptr<SelectionListener> selectionListener)
    {
        mSelectionListeners.push_back(selectionListener);
    }

    void ZListBox::removeSelectionListener(std::shared_ptr<SelectionListener> selectionListener)
    {
        mSelectionListeners.remove(selectionListener);
    }

    void ZListBox::distributeValueChangedEvent()
    {
        for (auto listener : mSelectionListeners)
        {
            SelectionEvent event(ZWidget::downcasted_shared_from_this<ZListBox>());
            listener->valueChanged(event);
        }
    }

    unsigned int ZListBox::getRowHeight() const
    {
        return getFont()->getHeight();
    }
}