//
// Created by tatiana on 28.09.18.
//

#include "ZTextBox.h"
#include "Graphics.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
#include "ZText.h"

namespace gui
{
    ZTextBox::ZTextBox()
        :ZWidget()
    {

    }

    void ZTextBox::init()
    {
        ZWidget::init();

        mEditable=true;
        mOpaque=true;

        mText=std::shared_ptr<ZText>(new ZText());

        setFocusable(true);

        addMouseListener(MouseListener::downcasted_shared_from_this<ZTextBox>());
        addKeyListener(KeyListener::downcasted_shared_from_this<ZTextBox>());
        adjustSize();
    }

    void ZTextBox::init(const std::string& text)
    {
        ZWidget::init();

        mEditable=true;
        mOpaque=true;

        mText=std::shared_ptr<ZText>(new ZText(text));

        setFocusable(true);

        addMouseListener(MouseListener::downcasted_shared_from_this<ZTextBox>());
        addKeyListener(KeyListener::downcasted_shared_from_this<ZTextBox>());
        adjustSize();
    }

    void ZTextBox::setText(const std::string& text)
    {
        mText->setContent(text);
        adjustSize();
    }

    void ZTextBox::draw_item(std::shared_ptr<Graphics> graphics)
    {
        if (mOpaque)
        {
            graphics->setColor(getBackgroundColor());
            graphics->fillRectangle(0, 0, getWidth(), getHeight());
        }

        if (isFocused() && isEditable())
        {
            drawCaret(graphics, mText->getCaretX(getFont()), mText->getCaretY(getFont()));
        }

        graphics->setColor(getForegroundColor());
        graphics->setFont(getFont());

        unsigned int i;
        for (i = 0; i < mText->getNumberOfRows(); i++)
        {
            // Move the text one pixel so we can have a caret before a letter.
            graphics->drawText(mText->getRow(i), 1, i * getFont()->getHeight());
        }
    }

    void ZTextBox::drawCaret(std::shared_ptr<Graphics> graphics, int x, int y)
    {
        graphics->setColor(getForegroundColor());
        graphics->drawLine(x, y, x, y + getFont()->getHeight());
    }

    void ZTextBox::mousePressed(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::Left)
        {
            mText->setCaretPosition(mouseEvent.getX(), mouseEvent.getY(), getFont());
            mouseEvent.consume();
        }
    }

    void ZTextBox::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void ZTextBox::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::Left)
            mText->setCaretPosition(mText->getCaretPosition() - 1);

        else if (key.getValue() == Key::Right)
            mText->setCaretPosition(mText->getCaretPosition() + 1);

        else if (key.getValue() == Key::Down)
            mText->setCaretRow(mText->getCaretRow() + 1);

        else if (key.getValue() == Key::Up)
            mText->setCaretRow(mText->getCaretRow() - 1);

        else if (key.getValue() == Key::Home)
            mText->setCaretColumn(0);

        else if (key.getValue() == Key::End)
            mText->setCaretColumn(mText->getNumberOfCharacters(mText->getCaretRow()));

        else if (key.getValue() == Key::Enter && mEditable)
            mText->insert('\n');

        else if (key.getValue() == Key::Backspace && mEditable)
            mText->remove(-1);

        else if (key.getValue() == Key::Delete && mEditable)
            mText->remove(1);

        else if(key.getValue() == Key::PageUp)
        {
            auto par = getParent();

            if (par != nullptr)
            {
                int rowsPerPage = par->getChildrenArea().height / getFont()->getHeight();
                mText->setCaretRow(mText->getCaretRow() - rowsPerPage);
            }
        }

        else if(key.getValue() == Key::PageDown)
        {
            auto par = getParent();

            if (par != nullptr)
            {
                int rowsPerPage = par->getChildrenArea().height / getFont()->getHeight();
                mText->setCaretRow(mText->getCaretRow() + rowsPerPage);
            }
        }

        else if(key.getValue() == Key::Tab && mEditable)
        {
            mText->insert(' ');
            mText->insert(' ');
            mText->insert(' ');
            mText->insert(' ');
        }

        else if (key.isCharacter() && mEditable)
            mText->insert(key.getValue());

        adjustSize();
        scrollToCaret();

        keyEvent.consume();
    }

    void ZTextBox::adjustSize()
    {
        const Rectangle& dim = mText->getDimension(getFont());
        setSize(dim.width, dim.height);
    }

    void ZTextBox::setCaretPosition(unsigned int position)
    {
        mText->setCaretPosition(position);
    }

    unsigned int ZTextBox::getCaretPosition() const
    {
        return mText->getCaretPosition();
    }

    void ZTextBox::setCaretRowColumn(int row, int column)
    {
        mText->setCaretRow(row);
        mText->setCaretColumn(column);
    }

    void ZTextBox::setCaretRow(int row)
    {
        mText->setCaretRow(row);
    }

    unsigned int ZTextBox::getCaretRow() const
    {
        return mText->getCaretRow();
    }

    void ZTextBox::setCaretColumn(int column)
    {
        mText->setCaretColumn(column);
    }

    unsigned int ZTextBox::getCaretColumn() const
    {
        return mText->getCaretColumn();
    }

    std::string ZTextBox::getTextRow(int row) const
    {
        return mText->getRow(row);
    }

    void ZTextBox::setTextRow(int row, const std::string& text)
    {
        mText->setRow(row, text);
        adjustSize();
    }

    unsigned int ZTextBox::getNumberOfRows() const
    {
        return mText->getNumberOfRows();
    }

    std::string ZTextBox::getText() const
    {
        return mText->getContent();
    }

    void ZTextBox::fontChanged()
    {
        adjustSize();
    }

    void ZTextBox::scrollToCaret()
    {
        showPart(mText->getCaretDimension(getFont()));
    }

    void ZTextBox::setEditable(bool editable)
    {
        mEditable = editable;
    }

    bool ZTextBox::isEditable() const
    {
        return mEditable;
    }

    void ZTextBox::addRow(const std::string &row)
    {
        mText->addRow(row);
        adjustSize();
    }

    bool ZTextBox::isOpaque()
    {
        return mOpaque;
    }

    void ZTextBox::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }
}