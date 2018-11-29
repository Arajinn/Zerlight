//
// Created by tatiana on 29.11.18.
//

#ifndef ZERLIGHT_MAPWIDGET_H
#define ZERLIGHT_MAPWIDGET_H

#include "guichan/gui/ZContainer.h"
#include "guichan/gui/MouseListener.h"
#include "guichan/gui/KeyListener.h"

class MapWidget : public gui::ZContainer, public gui::MouseListener, public gui::KeyListener{
public:
    MapWidget();
    ~MapWidget();

    virtual void init();
    virtual void shutdown();
};


#endif //ZERLIGHT_MAPWIDGET_H
