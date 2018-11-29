//
// Created by tatiana on 28.11.18.
//

#ifndef ZERLIGHT_RESOLUTIONLISTMODEL_H
#define ZERLIGHT_RESOLUTIONLISTMODEL_H

#include "guichan/gui/ListModel.h"

class ResolutionListModel : public gui::ListModel{
public:
    int getNumberOfElements();

    std::string getElementAt(int i);
};


#endif //ZERLIGHT_RESOLUTIONLISTMODEL_H
