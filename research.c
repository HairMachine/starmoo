#include <stdlib.h>
#include <math.h>
#include "raylib.h"

#include "research.h"
#include "event.h"

char Research_fieldStrings[FIELD_ALL][32] = {
    "Propulsion", "Construction", "Weapons", "Espionage", "Luxuries", "Forcefields"
};

Research_Tech* researching[FIELD_ALL] = {0, 0, 0, 0, 0};
Research_Tech* developing[FIELD_ALL] = {0, 0, 0, 0, 0};

int techLevel = 0;

Research_Tech techs[RT_ALL] = {
    (Research_Tech) {"None", FIELD_ALL, 0, 0, 1, 1},
    // Propulsion techs
    (Research_Tech) {"Warp 2", FIELD_PROPULSION, 0, 1000},
    (Research_Tech) {"Warp 3", FIELD_PROPULSION, 1, 5000},
    (Research_Tech) {"Warp 4", FIELD_PROPULSION, 2, 15000},
    (Research_Tech) {"Warp 5", FIELD_PROPULSION, 3, 30000},
    (Research_Tech) {"Warp 6", FIELD_PROPULSION, 4, 60000},
    (Research_Tech) {"Warp 7", FIELD_PROPULSION, 5, 120000},
    (Research_Tech) {"Warp 8", FIELD_PROPULSION, 6, 240000},
    (Research_Tech) {"Warp 9", FIELD_PROPULSION, 7, 480000},
    (Research_Tech) {"Warp 10", FIELD_PROPULSION, 8, 960000},
    // Construction techs
    (Research_Tech) {"Armour 2", FIELD_CONSTRUCTION, 0, 1000},
    (Research_Tech) {"Armour 3", FIELD_CONSTRUCTION, 1, 5000},
    (Research_Tech) {"Armour 4", FIELD_CONSTRUCTION, 2, 15000},
    (Research_Tech) {"Armour 5", FIELD_CONSTRUCTION, 4, 60000},
    (Research_Tech) {"Armour 6", FIELD_CONSTRUCTION, 6, 240000},
    (Research_Tech) {"Mining 2", FIELD_CONSTRUCTION, 2, 15000},
    (Research_Tech) {"Mining 3", FIELD_CONSTRUCTION, 3, 30000},
    (Research_Tech) {"Mining 4", FIELD_CONSTRUCTION, 4, 60000},
    (Research_Tech) {"Mining 5", FIELD_CONSTRUCTION, 5, 120000},
    (Research_Tech) {"Mining 6", FIELD_CONSTRUCTION, 6, 240000},
    (Research_Tech) {"Carrier 1", FIELD_CONSTRUCTION, 1, 5000},
    (Research_Tech) {"Carrier 2", FIELD_CONSTRUCTION, 3, 30000},
    (Research_Tech) {"Carrier 3", FIELD_CONSTRUCTION, 5, 120000},
    (Research_Tech) {"Carrier 4", FIELD_CONSTRUCTION, 6, 24000},
    (Research_Tech) {"Carrier 5", FIELD_CONSTRUCTION, 8, 960000},
    (Research_Tech) {"Construct 1", FIELD_CONSTRUCTION, 2, 15000},
    (Research_Tech) {"Construct 2", FIELD_CONSTRUCTION, 4, 60000},
    (Research_Tech) {"Construct 3", FIELD_CONSTRUCTION, 5, 120000},
    (Research_Tech) {"Construct 4", FIELD_CONSTRUCTION, 7, 480000},
    (Research_Tech) {"Construct 5", FIELD_CONSTRUCTION, 8, 960000},
    // Weapon techs
    (Research_Tech) {"Multilaster", FIELD_WEAPONS, 0, 1000},
    (Research_Tech) {"Ion Cannon", FIELD_WEAPONS, 1, 5000},
    (Research_Tech) {"Proton Blaster", FIELD_WEAPONS, 2, 15000},
    (Research_Tech) {"Plasma Cannon", FIELD_WEAPONS, 3, 30000},
    (Research_Tech) {"Turbolaser", FIELD_WEAPONS, 4, 60000},
    (Research_Tech) {"Tachyon Generator", FIELD_WEAPONS, 6, 240000},
    (Research_Tech) {"Hyperbeam Emitter", FIELD_WEAPONS, 8, 960000},
    (Research_Tech) {"Mass Driver", FIELD_WEAPONS, 1, 5000},
    (Research_Tech) {"Gauss Cannon", FIELD_WEAPONS, 3, 30000},
    (Research_Tech) {"Macrodriver", FIELD_WEAPONS, 4, 60000},
    (Research_Tech) {"Assault Cannon", FIELD_WEAPONS, 6, 240000},
    (Research_Tech) {"Planetoid Launcher", FIELD_WEAPONS, 8, 960000},
    (Research_Tech) {"Fusion Torpedo", FIELD_WEAPONS, 1, 1000},
    (Research_Tech) {"Proton Torpedo", FIELD_WEAPONS, 3, 30000},
    (Research_Tech) {"Antimatter Torpedo", FIELD_WEAPONS, 5, 120000},
    (Research_Tech) {"Singularity Torpedo", FIELD_WEAPONS, 8, 960000},
    // Espionage techs
    (Research_Tech) {"Subspace Antenna 1", FIELD_ESPIONAGE, 0, 1000},
    (Research_Tech) {"Subspace Antenna 2", FIELD_ESPIONAGE, 2, 15000},
    (Research_Tech) {"Subspace Antenna 3", FIELD_ESPIONAGE, 4, 60000},
    (Research_Tech) {"Subspace Antenna 4", FIELD_ESPIONAGE, 5, 120000},
    (Research_Tech) {"Subspace Antenna 5", FIELD_ESPIONAGE, 7, 480000},
    (Research_Tech) {"Imperial Encryptor 1", FIELD_ESPIONAGE, 1, 5000},
    (Research_Tech) {"Imperial Encryptor 2", FIELD_ESPIONAGE, 3, 30000},
    (Research_Tech) {"Imperial Encryptor 3", FIELD_ESPIONAGE, 5, 120000},
    (Research_Tech) {"Imperial Encryptor 4", FIELD_ESPIONAGE, 6, 240000},
    (Research_Tech) {"Imperial Encryptor 5", FIELD_ESPIONAGE, 7, 480000},
    (Research_Tech) {"Subspace Override 1", FIELD_ESPIONAGE, 2, 15000},
    (Research_Tech) {"Subspace Override 2", FIELD_ESPIONAGE, 4, 60000},
    (Research_Tech) {"Subspace Override 3", FIELD_ESPIONAGE, 5, 120000},
    (Research_Tech) {"Subspace Override 4", FIELD_ESPIONAGE, 7, 480000},
    (Research_Tech) {"Subspace Override 5", FIELD_ESPIONAGE, 8, 960000},
    // Luxury techs
    (Research_Tech) {"Luxury Goods 1", FIELD_LUXURIES, 0, 1000},
    (Research_Tech) {"Luxury Goods 2", FIELD_LUXURIES, 1, 5000},
    (Research_Tech) {"Luxury Goods 3", FIELD_LUXURIES, 2, 15000},
    (Research_Tech) {"Luxury Goods 4", FIELD_LUXURIES, 4, 60000},
    (Research_Tech) {"Luxury Goods 5", FIELD_LUXURIES, 5, 120000},
    (Research_Tech) {"Barathian Wine 1", FIELD_LUXURIES, 2, 15000},
    (Research_Tech) {"Barathian Wine 2", FIELD_LUXURIES, 3, 30000},
    (Research_Tech) {"Barathian Wine 3", FIELD_LUXURIES, 4, 60000},
    (Research_Tech) {"Barathian Wine 4", FIELD_LUXURIES, 5, 120000},
    (Research_Tech) {"Barathian Wine 5", FIELD_LUXURIES, 7, 480000},
    (Research_Tech) {"Medical Supplies 1", FIELD_LUXURIES, 1, 5000},
    (Research_Tech) {"Medical Supplies 2", FIELD_LUXURIES, 2, 15000},
    (Research_Tech) {"Medical Supplies 3", FIELD_LUXURIES, 4, 60000},
    (Research_Tech) {"Medical Supplies 4", FIELD_LUXURIES, 6, 240000},
    (Research_Tech) {"Medical Supplies 5", FIELD_LUXURIES, 8, 960000},
    (Research_Tech) {"Stims 1", FIELD_LUXURIES, 0, 1000},
    (Research_Tech) {"Stims 2", FIELD_LUXURIES, 1, 5000},
    (Research_Tech) {"Stims 3", FIELD_LUXURIES, 3, 30000},
    (Research_Tech) {"Stims 4", FIELD_LUXURIES, 5, 120000},
    (Research_Tech) {"Stims 5", FIELD_LUXURIES, 7, 480000},
    // Forcefield techs
    (Research_Tech) {"Sheild Generator 2", FIELD_FORCEFIELDS, 0, 1000},
    (Research_Tech) {"Sheild Generator 3", FIELD_FORCEFIELDS, 1, 5000},
    (Research_Tech) {"Sheild Generator 4", FIELD_FORCEFIELDS, 2, 15000},
    (Research_Tech) {"Sheild Generator 5", FIELD_FORCEFIELDS, 3, 30000},
    (Research_Tech) {"Sheild Generator 6", FIELD_FORCEFIELDS, 4, 60000},
    (Research_Tech) {"Reflector Shields 1", FIELD_FORCEFIELDS, 2, 15000},
    (Research_Tech) {"Reflector Shields 2", FIELD_FORCEFIELDS, 3, 30000},
    (Research_Tech) {"Reflector Shields 3", FIELD_FORCEFIELDS, 5, 120000},
    (Research_Tech) {"Reflector Shields 4", FIELD_FORCEFIELDS, 7, 480000},
    (Research_Tech) {"Reflector Shields 5", FIELD_FORCEFIELDS, 8, 960000},
    (Research_Tech) {"Shield Recharger 1", FIELD_FORCEFIELDS, 1, 1000},
    (Research_Tech) {"Shield Recharger 2", FIELD_FORCEFIELDS, 3, 30000},
    (Research_Tech) {"Shield Recharger 3", FIELD_FORCEFIELDS, 4, 60000},
    (Research_Tech) {"Shield Recharger 4", FIELD_FORCEFIELDS, 6, 240000},
    (Research_Tech) {"Shield Recharger 5", FIELD_FORCEFIELDS, 7, 480000},
    (Research_Tech) {"Energy Drainer 1", FIELD_FORCEFIELDS, 3, 30000},
    (Research_Tech) {"Energy Drainer 2", FIELD_FORCEFIELDS, 4, 60000},
    (Research_Tech) {"Energy Drainer 3", FIELD_FORCEFIELDS, 6, 240000},
    (Research_Tech) {"Energy Drainer 4", FIELD_FORCEFIELDS, 7, 480000},
    (Research_Tech) {"Energy Drainer 5", FIELD_FORCEFIELDS, 8, 960000},
};

Research_Tech* _getRandomTech(Research_Field field) {
    Research_Tech* availableTechs[RT_ALL];
    int lasttech = 0;
    // Filter out all the techs that don't meet the criteria
    for (int i = 0; i < RT_ALL; i++) {
        if (
            techs[i].field == field &&
            techs[i].level <= techLevel &&
            !techs[i].discovered
        ) {
            availableTechs[lasttech] = &techs[i];
            lasttech++;
        }
    }
    if (lasttech == 0) {
        return 0;
    }
    return availableTechs[rand() % lasttech];
}

void Research_advance(Research_Field field, int amount) {
    Research_Tech* rt = researching[field];
    // If not reseraching anything, randomly assign a tech to be researched
    if (!rt) {
        rt = _getRandomTech(field);
        researching[field] = rt;
    }
    if (rt) {
        rt->points += amount - ceil(rand() % amount/4) + ceil(rand() % amount/4);
        if (!rt->discovered && rt->points >= rt->pointsRequired/5) {
            rt->discovered = 1;
            Event_create(
                "Tech discovered!",
                TextFormat("Scientists have discovered %s.\nYou may now select this tech for development.", rt->name)
            );
            researching[field] = 0;
        }
    }
    // Develop any tech selected for development
    rt = developing[field];
    if (!rt) {
        return;
    }
    rt->points += amount - ceil(rand() % amount/4) + ceil(rand() % amount/4);
    if (rt->points >= rt->pointsRequired) {
        Event_create(
            "Tech developed!",
            TextFormat("Scientists have developed %s,\nunlocking new construction options.", rt->name)
        );
        rt->developed = 1;
        if (rt->level == techLevel) {
            techLevel++;
        }
        developing[field] = 0;
    }
}

int Research_techIsDeveloped(Research_TechType rt) {
    return techs[rt].developed;
}

Research_Tech* Research_getTechPointer(int r) {
    return &techs[r];
}

Research_Tech* Research_getDeveloping(int field) {
    return developing[field];
}

void Research_startDeveloping(int r) {
    Research_Tech* tech = &techs[r];
    if (!developing[tech->field]) {
        developing[tech->field] = tech;
    }
}