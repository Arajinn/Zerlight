//
// Created by tatiana.polozova on 26.03.2018.
//

#ifndef ZERL_GAMEMANAGER_H
#define ZERL_GAMEMANAGER_H

#include <memory>
#include <vector>

#define GAME game::GameManager::instance()

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

        void testSpawn();

        static const std::shared_ptr<GameManager> instance();

        const std::shared_ptr<map::Region> region() const;
        const std::shared_ptr<properties::GameDefinition> gameDefinition() const;

        void update(const float& dt);

        void addToSpawnList(std::shared_ptr<GameEntity> ent);
        void addToDeleteList(std::shared_ptr<GameEntity> ent);

        unsigned int nextNavNodeID();
        unsigned int nextNavID();
    private:
        void init();

        std::shared_ptr<map::Region> mRegion;
        std::shared_ptr<properties::GameDefinition> mGameDefs;
        std::shared_ptr<AIDirector> mAIDirector;

        void processSpawn();

        void processPreUpdate();
        void processUpdate(const float& dt);
        void processPostUpdate();
        void processDeletion();

        int mNextGameObjectID;
        std::vector<std::shared_ptr<GameEntity>> mSpawnList;
        std::vector<std::shared_ptr<GameEntity>> mActiveList;
        std::vector<std::shared_ptr<GameEntity>> mDeleteList;

        unsigned int mNextNodeID;
        unsigned int mNextNavGraphID;
    };
}

#endif //ZERL_GAMEMANAGER_H
