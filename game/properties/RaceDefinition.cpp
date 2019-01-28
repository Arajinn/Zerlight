//
// Created by tatiana on 26.03.2018.
//

#include "RaceDefinition.h"
#include "GenderDef.h"
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
        auto iter=std::find_if(AdditionalDiet.begin(),AdditionalDiet.end(),[&itemID](std::pair<std::string,float> const& value)
        {
            return value.first==itemID;
        });

        if (iter==AdditionalDiet.end())
            return 0.0f;
        else
            return iter->second;
    }

    game::GenderType RaceDefinition::generateGender() const
    {
        float rand=utils::Randomizer::instance()->uniform(0.0f,1.0f);

        float summ=0.0f;
        for (size_t i=0,isize=Genders.size();i<isize;i++)
            summ+=Genders.at(i)->RandomWeight;

        float threshold1=0.0f;
        for (size_t i=0,isize=Genders.size();i<isize;i++)
        {
            float threshold2=threshold1+Genders.at(i)->RandomWeight/summ;

            if (rand<threshold2)
                return Genders.at(i)->Gender;

            threshold1=threshold2;
        }

        return game::GenderType::Neuter;
    }

    std::shared_ptr<const GenderDef> RaceDefinition::gender(const game::GenderType& type) const
    {
        const auto iter=std::find_if(Genders.begin(),Genders.end(),[&type](std::shared_ptr<GenderDef> const& elem)
        {
            return elem->Gender==type;
        });

        if (iter!=Genders.end())
            return (*iter);
        else
            return nullptr;
    }
}
