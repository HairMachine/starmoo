#include <stdlib.h>
#include "raylib.h"

#include "research.h"
#include "event.h"

char Research_strings[FIELD_ALL][32] = {
    "Propulsion", "Construction", "Weapons"
};

Research_Tech* researching[FIELD_ALL] = {0, 0, 0};

int techLevel = 0;

#define NUMTECHS 3

Research_Tech techs[NUMTECHS] = {
    (Research_Tech) {"Warp 2", FIELD_PROPULSION, 0, 1000},
    (Research_Tech) {"Construction 2", FIELD_CONSTRUCTION, 0, 750},
    (Research_Tech) {"Laser 2", FIELD_WEAPONS, 0, 500}
};

Research_Tech* _getRandomTech(Research_Field field) {
    Research_Tech* availableTechs[NUMTECHS];
    int lasttech = 0;
    // Filter out all the techs that don't meet the criteria
    for (int i = 0; i < NUMTECHS; i++) {
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
        if (!rt) {
            return;
        }
    }
    // Increase points
    rt->points += amount;
    if (!rt->discovered && rt->points >= rt->pointsRequired/5) {
        rt->discovered = 1;
        Event_create(
            "Tech discovered!",
            TextFormat("Scientists have discovered %s\nand have begun working to develop it.", rt->name)
        );
    } else if (rt->points >= rt->pointsRequired) {
        Event_create(
            "Tech developed!",
            TextFormat("Scientists have developed %s,\nunlocking new construction options.", rt->name)
        );
        rt->developed = 1;
        if (rt->level == techLevel) {
            techLevel++;
        }
        researching[field] = 0;
        // TODO: Unlock components somehow
    }
}

