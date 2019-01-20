//
// Created by tatiana on 25.06.2018.
//

#include "MaterialDef.h"
#include "DamageDef.h"

namespace properties
{
    MaterialDef::MaterialDef()
    {
        DamageProperties.resize((size_t)game::DamageType::Undef);
        for (size_t i=0,isize=DamageProperties.size();i<isize;i++)
            DamageProperties[i]=std::make_shared<DamageDef>();
    }

    MaterialDef::~MaterialDef()
    {

    }

    std::string MaterialDef::GroupNameOrName() const
    {
        if (GroupName.empty())
            return Name;
        else
            return GroupName;
    }
}