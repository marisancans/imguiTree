#pragma once


struct PlayerStats {
    int health;
    int attack;
    int defense;
    int heal;
    int attackCount;
    int defenseCount;
    int healCount;

    inline bool operator==(PlayerStats const& parent){
        return health == parent.health;
    }
};