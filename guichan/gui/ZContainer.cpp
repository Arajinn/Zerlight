//
// Created by tatiana on 28.09.18.
//

#include "ZContainer.h"
#include "Graphics.h"
#include "ContainerEvent.h"
#include "ContainerListener.h"

namespace gui
{
    ZContainer::ZContainer()
        :ZWidget()
    {

    }

    void ZContainer::init()
    {
        ZWidget::init();
        
        mOpaque = true;
    }

    ZContainer::~ZContainer()
    {

    }

    void ZContainer::draw_item(std::shared_ptr<Graphics> graphics)
    {
        if (isOpaque())
        {
            graphics->setColor(getBaseColor());
            graphics->fillRectangle(0, 0, getWidth(), getHeight());
        }
    }

    void ZContainer::setOpaque(bool opaque)
    {
        mOpaque = opaque;
    }

    bool ZContainer::isOpaque() const
    {
        return mOpaque;
    }

    void ZContainer::add(std::shared_ptr<ZWidget> widget)
    {
        ZWidget::add_child(widget);
        distributeWidgetAddedEvent(widget);
    }

    void ZContainer::add(std::shared_ptr<ZWidget> widget, int x, int y)
    {
        widget->setPosition(x, y);
        ZWidget::add_child(widget);
        distributeWidgetAddedEvent(widget);
    }

    void ZContainer::remove(std::shared_ptr<ZWidget> widget)
    {
        ZWidget::remove(widget);
        distributeWidgetRemovedEvent(widget);
    }

    void ZContainer::clear()
    {
        ZWidget::clear();
    }

    std::shared_ptr<ZWidget> ZContainer::findWidgetById(const std::string &id)
    {
        return ZWidget::findWidgetById(id);
    }

    void ZContainer::addContainerListener(std::shared_ptr<ContainerListener> containerListener)
    {
        mContainerListeners.push_back(containerListener);
    }

    void ZContainer::removeContainerListener(std::shared_ptr<ContainerListener> containerListener)
    {
        mContainerListeners.remove(containerListener);
    }

    void ZContainer::distributeWidgetAddedEvent(std::shared_ptr<ZWidget> source)
    {
        for (auto listener : mContainerListeners)
        {
            ContainerEvent event(source, ZContainer::downcasted_shared_from_this<ZContainer>());
            listener->widgetAdded(event);
        }
    }

    void ZContainer::distributeWidgetRemovedEvent(std::shared_ptr<ZWidget> source)
    {
        for (auto listener : mContainerListeners)
        {
            ContainerEvent event(source, ZContainer::downcasted_shared_from_this<ZContainer>());
            listener->widgetRemoved(event);
        }
    }

    const std::list<std::shared_ptr<ZWidget>>& ZContainer::getChildren() const
    {
        return ZWidget::getChildren();
    }

    void ZContainer::resizeToContent()
    {
        ZWidget::resizeToChildren();
    }

    Rectangle ZContainer::getChildrenArea()
    {
        return Rectangle(0, 0, getWidth(), getHeight());
    }
}