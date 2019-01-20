//
// Created by tatiana on 29.12.18.
//

#ifndef ZERLIGHT_DAMAGEDEF_H
#define ZERLIGHT_DAMAGEDEF_H

namespace properties {
    struct DamageDef {
        float Blocks{1000.0f};
        float Break{100.0f};
        float PercentTransfered{0.0f};
        float PercentReceived{1.0f};
    };
}

#endif //ZERLIGHT_DAMAGEDEF_H
