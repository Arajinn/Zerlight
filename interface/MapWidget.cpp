//
// Created by tatiana on 29.11.18.
//

#include "MapWidget.h"

MapWidget::MapWidget()
        :gui::ZContainer()
        ,gui::MouseListener()
        ,gui::KeyListener()
{

}

MapWidget::~MapWidget()
{

}

void MapWidget::init()
{
    ZContainer::init();

    addMouseListener(gui::MouseListener::downcasted_shared_from_this<MapWidget>());
    addKeyListener(gui::KeyListener::downcasted_shared_from_this<MapWidget>());
}

void MapWidget::shutdown()
{
    gui::ZContainer::shutdown();
}