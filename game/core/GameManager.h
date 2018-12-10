//
// Created by tatiana on 26.03.2018.
//

#ifndef ZERL_GAMEMANAGER_H
#define ZERL_GAMEMANAGER_H

#include <memory>
#include <vector>

namespace game
{
    class GameEntity;
    class AIDirector;
}

namespace map
{
    class Region;
}

namespace properties
{
    struct GameDefinition;
}

namespace game {
class GameManager : public std::enable_shared_from_this<GameManager> {
    public:
        GameManager();
        ~GameManager();

        static std::shared_ptr<GameManager> create();

        bool testSpawn();

        const std::shared_ptr<map::Region> region() const;

        void update(const float& dt);

        void addToSpawnList(std::shared_ptr<GameEntity> ent);
        void addToDeleteList(std::shared_ptr<GameEntity> ent);

        unsigned int nextNavNodeID();
        unsigned int nextNavID();

        bool generateNewWorld();
        float getInitProgress();

        bool getIsGenerateFinish() const;
    private:
        void init();

        std::shared_ptr<map::Region> mRegion;
        std::shared_ptr<AIDirector> mAIDirector;

        void processSpawn();

        void processPreUpdate();
        void processUpdate(const float& dt);
        void processPostUpdate();
        void processDeletion();

        int mNextGameObjectID;
        std::vector<std::shared_ptr<GameEntity>> mSpawnList;
        std::vector<std::shared_ptr<GameEntity>> mActiveList;
        std::vector<std::shared_ptr<GameEntity>> mNonActiveList;
        std::vector<std::shared_ptr<GameEntity>> mDeleteList;

        unsigned int mNextNodeID;
        unsigned int mNextNavGraphID;

        bool mIsGenerateFinish;
    };
}

#endif //ZERL_GAMEMANAGER_H
