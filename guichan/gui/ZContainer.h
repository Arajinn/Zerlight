//
// Created by tatiana on 28.09.18.
//

#ifndef GUICHAN_CONTAINER_H
#define GUICHAN_CONTAINER_H

#include "guichan/gui/ZWidget.h"

namespace gui {
    class ContainerListener;

    /**
     * An implementation of a container able to contain other widgets. A widget's
     * position in the container is relative to the container itself and not the screen.
     * A container is the most common widget to use as the Gui's top widget as makes the Gui
     * able to contain more than one widget.
     *
     * @see Gui::setTop
     */
    class ZContainer : public ZWidget{
    public:

        /**
         * Constructor. A container is opauqe as default, if you want a
         * none opaque container call setQpaque(false).
         *
         * @see setOpaque, isOpaque
         */
        ZContainer();
        virtual void init();

        /**
         * Destructor.
         */
        virtual ~ZContainer();

        /**
         * Sets the container to be opaque or not. If the container
         * is opaque its background will be drawn, if it's not opaque
         * its background will not be drawn, and thus making the container
         * completely transparent.
         *
         * NOTE: This is not the same as to set visibility. A non visible
         *       container will not itself nor will it draw its content.
         *
         * @param opaque True if the container should be opaque, false otherwise.
         * @see isOpaque
         */
        void setOpaque(bool opaque);

        /**
         * Checks if the container is opaque or not.
         *
         * @return True if the container is opaque, false otherwise.
         * @see setOpaque
         */
        bool isOpaque() const;

        /**
         * Adds a widget to the container.
         *
         * @param widget The widget to add.
         * @see remove, clear
         */
        virtual void add(std::shared_ptr<ZWidget> widget);

        /**
         * Adds a widget to the container and also specifies the widget's
         * position in the container. The position is relative to the container
         * and not relative to the screen.
         *
         * @param widget The widget to add.
         * @param x The x coordinate for the widget.
         * @param y The y coordinate for the widget.
         * @see remove, clear
         */
        virtual void add(std::shared_ptr<ZWidget> widget, int x, int y);

        /**
         * Removes a widget from the Container.
         *
         * @param widget The widget to remove.
         * @throws Exception when the widget has not been added to the
         *                   container.
         * @see add, clear
         */
        virtual void remove(std::shared_ptr<ZWidget> widget);

        /**
         * Clears the container of all widgets.
         *
         * @see add, remove
         */
        virtual void clear();

        /**
         * Finds a widget given an id.
         *
         * @param id The id to find a widget by.
         * @return A widget with a corrosponding id, NULL if no widget
         *         is found.
         * @see Widget::setId
         */
        virtual std::shared_ptr<ZWidget> findWidgetById(const std::string &id);

        /**
         * Adds a container listener to the container. When a widget is
         * added or removed an event will be sent to all container
         * listeners of the container
         *
         * @param containerListener The container listener to add.
         * @since 0.9.0
         */
        void addContainerListener(std::shared_ptr<ContainerListener> containerListener);

        /**
         * Removes a container listener from the container.
         *
         * @param containerListener The container listener to remove.
         * @since 0.9.0
         */
        void removeContainerListener(std::shared_ptr<ContainerListener> containerListener);

        /**
         * Returns the children of the container.
         *
         * @return The children of the container.
         */
        const std::list<std::shared_ptr<ZWidget>>& getChildren() const;

        /**
         * Resizes the Container's size to fit te content exactly.
         */
        void resizeToContent();


        // Inherited from Widget

        virtual void draw_item(std::shared_ptr<Graphics> graphics);

        virtual Rectangle getChildrenArea();

    protected:
        /**
         * Distributes a widget added container event to all container listeners
         * of the container.
         *
         * @param source The source widget of the event.
         * @since 0.9.0
         */
        void distributeWidgetAddedEvent(std::shared_ptr<ZWidget> source);

        /**
         * Distributes a widget removed container event to all container listeners
         * of the container.
         *
         * @param source The source widget of the event.
         * @since 0.9.0
         */
        void distributeWidgetRemovedEvent(std::shared_ptr<ZWidget> source);

        /**
         * True if the container is opaque, false otherwise.
         */
        bool mOpaque;

        /**
         * The container listeners of the container.
         */
        std::list<std::shared_ptr<ContainerListener>> mContainerListeners;
    };
}

#endif //GUICHAN_CONTAINER_H
