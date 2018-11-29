//
// Created by tatiana on 28.11.18.
//

#include "ResolutionListModel.h"

int ResolutionListModel::getNumberOfElements()
{
    return 2;
}

std::string ResolutionListModel::getElementAt(int i)
{
    switch(i)
    {
        case 0:
            return std::string("1024x768");
        case 1:
            return std::string("800x600");
        default:
            return std::string("");
    }
}