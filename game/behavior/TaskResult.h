//
// Created by tatiana.polozova on 08.06.2018.
//

#ifndef ZERL_TASKRESULT_H
#define ZERL_TASKRESULT_H

namespace behavior
{
    enum class TaskResult
    {
        Success,
        Running,
        Failure
    };

    enum class BehaviorType
    {
        None,
        PlayerCharacter,
        GenericCharacter,
        WildAnimal,
        Beetle,
        MantScout,
        MantWorker,
        Ambassador,
        GoblinRaider,
        GoblinSoldier,
        GoblinTunneler,
        Automaton
    };
}

#endif //ZERL_TASKRESULT_H
