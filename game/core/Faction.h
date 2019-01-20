//
// Created by tatiana on 10.01.19.
//

#ifndef ZERLIGHT_FACTION_H
#define ZERLIGHT_FACTION_H

#include <memory>
#include <string>
#include <vector>

namespace game
{
    class Character;
    class CharacterHistory;
    class Ambassador;
    class Envoy;
}

namespace properties
{
    struct FactionDef;
}

namespace game {
    class Faction {
    public:
        Faction(std::shared_ptr<const properties::FactionDef> factionDef, const std::string& name,
                const size_t& factionGUID, float distance=6.0f);
        ~Faction();

        std::shared_ptr<const properties::FactionDef> factionDef() const;

        bool isHostile(const size_t& factionID) const;

        void setFactionRelation(const size_t& factionID, const float& relation);
        float factionRelation(const size_t& factionID) const;

        size_t id() const;

        void addMember(std::shared_ptr<Character> character);
        void removeMember(std::shared_ptr<Character> character);
        std::vector<std::shared_ptr<Character>> members() const;

        void memberDied(std::shared_ptr<Character> character);

        bool isDeceasedMembersContain(const unsigned int& characterID) const;
    private:
        size_t mFactionGUID;
        std::string mName;
        std::shared_ptr<const properties::FactionDef> mFactionDef;
        std::vector<std::pair<size_t,float>> mFactionRelations;

        size_t mHighestPopulation;
        std::vector<std::shared_ptr<Character>> mMembers;
        std::vector<std::shared_ptr<CharacterHistory>> mDeceasedMembers;

        std::shared_ptr<Ambassador> mAmbassador;
        std::vector<std::shared_ptr<Envoy>> mEnvoys;

        float mDistance;
    };
}

#endif //ZERLIGHT_FACTION_H
