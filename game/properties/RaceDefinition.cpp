//
// Created by tatiana on 26.03.2018.
//

#include "RaceDefinition.h"
#include "game/utils/Randomizer.h"
#include <algorithm>

namespace properties
{
    RaceDefinition::RaceDefinition()
    {

    }

    RaceDefinition::~RaceDefinition()
    {

    }

    float RaceDefinition::additionalDietValue(std::string itemID) const
    {
        auto iter=std::find_if(std::begin(AdditionalDiet),std::end(AdditionalDiet),[&itemID](std::pair<std::string,float> const& value)
        {
            return value.first==itemID;
        });

        if (iter==std::end(AdditionalDiet))
            return 0.0f;
        else
            return iter->second;
    }

    game::GenderType RaceDefinition::generateGender() const
    {
        float rand=utils::Randomizer::instance()->uniform(0.0f,1.0f);

        float summ=0.0f;
        for (int i=0,isize=Genders.size();i<isize;i++)
            summ+=Genders.at(i).second;

        float threshold1=0.0f;
        for (int i=0,isize=Genders.size();i<isize;i++)
        {
            float threshold2=threshold1+Genders.at(i).second/summ;

            if (rand<threshold2)
                return Genders.at(i).first;

            threshold1=threshold2;
        }

        return game::GenderType::Neuter;
    }
}
