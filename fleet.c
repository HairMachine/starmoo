#include <math.h>
#include "raylib.h"

#include "event.h"
#include "unit.h"
#include "fleet.h"

Fleet_Entity fleets[8];
int fleetnum;

Fleet_Entity* Fleet_create() {
    Fleet_Entity* f = &fleets[fleetnum];
    f->orders = -1;
    f->fuel = 0;
    f->storageMax = 0;
    fleetnum++;
    return f;
}

void Fleet_addUnit(Fleet_Entity* f, int uid) {
    f->units[f->unitmax] = uid;
    f->unitmax++;
    // Recalculate supplies
    Unit_Entity newUnit = Unit_getCopy(uid);
    f->fuelMax += newUnit.fuelMax;
    f->storageMax += newUnit.storage;
    // Set warp factor
    if (newUnit.warpDriveLevel > 0 && newUnit.warpDriveLevel > f->warpFactor) {
        f->warpFactor = newUnit.warpDriveLevel;
    }
}

void Fleet_removeUnit(Fleet_Entity* f, int uid) {
    for (int i = uid; i < f->unitmax; i++) {
        f->units[i] = f->units[i + 1];
    }
    f->unitmax--;
}

Fleet_Entity* Fleet_getPointer(int index) {
    return &fleets[index];
}

void Fleet_destroy(int index) {
    for (int i = index; i < fleetnum - 1; i++) {
        fleets[i] = fleets[i + 1];
    }
    fleetnum--;
}

int Fleet_count() {
    return fleetnum;
}

int Fleet_getUnitCountByDesign(Fleet_Entity* f, int designId) {
    Unit_Entity u;
    int count = 0;
    for (int i = 0; i < f->unitmax; i++) {
        u = Unit_getCopy(f->units[i]);
        if (u.design == designId) {
            count++;
        }
    }
    return count;
}

void Fleet_generateResources(Fleet_Entity* f) {
    // TODO: Full product / resource system as described in design doc
    Unit_Entity u;
    // Assign population to jobs and stuff
    // TODO: more than just farmers, duh
    int farmers = 0;
    for (int i = 0; i < FLEET_BASIC_NEEDS; i++) {
        farmers++;
    }
    int plots = 0;
    for (int i = 0; i < f->unitmax; i++) {
        u = Unit_getCopy(f->units[i]);
        plots += u.farming;
    }
    farmers = plots > farmers ? farmers : plots;
    f->food += round(farmers / 2);
    f->drink += round(farmers / 2);
    // Plants generate a little oxygen
    f->oxygen += floor(farmers * 0.10);
    // Fill storage containers with goods, discarding excess equally
    // TODO: Discard excess according to storage policies or some other thing
    // TODO: Different things take up different amounts of storage? Or perhaps each unit larger / smaller effects?
    int spaceRequired = f->food + f->drink + f->oxygen - f->storageMax;
    while (spaceRequired > 0) {
        if (f->food > 0) f->food -= 1;
        if (f->drink > 0) f->drink -= 1;
        if (f->oxygen > 0) f->oxygen -= 1;
        spaceRequired -= 3;
    }
}

int Fleet_createUnrest(Fleet_Entity* f, int need, int notBasic) {
    int unrestChange = 0;
    if (!notBasic) {
        unrestChange += FLEET_BASIC_NEEDS - need;
        if (unrestChange < 0) unrestChange = 0;
    }
    unrestChange += FLEET_STANDARD_NEEDS - need * 2;
    if (unrestChange < 0) unrestChange = 0;
    unrestChange += FLEET_HIGH_NEEDS - need * 4;
    if (unrestChange < 0) unrestChange = 0;
    unrestChange += FLEET_ULTRAHIGH_NEEDS - need * 8;
    if (unrestChange < 0) unrestChange = 0;
    return unrestChange;
}

void Fleet_unrestChange(Fleet_Entity* f) {
    int unrestChange = 
        Fleet_createUnrest(f, f->food, 0) +
        Fleet_createUnrest(f, f->drink, 0) +
        Fleet_createUnrest(f, f->recreation, 0) +
        Fleet_createUnrest(f, floor(f->luxury / 2), 1) +
        Fleet_createUnrest(f, floor(f->education / 2), 1);
    f->unrest += floor(unrestChange * 0.10);
    if (unrestChange == 0) f->unrest -= 10;
    if (f->unrest < 0) f->unrest = 0;
    if (f->unrest > 100) f->unrest = 100;
}

int Fleet_excessDeaths(Fleet_Entity* f) {
    int excessDeaths = 0;
    if (f->food < FLEET_BASIC_NEEDS) {
        Event_create(
            "Famine!",
            TextFormat("Captain, our people are starving!\nWe lacked %d food last month.\n", FLEET_BASIC_NEEDS - f->food)
        );
        excessDeaths += floor((FLEET_BASIC_NEEDS - f->food) * 0.1);
    }
    if (f->drink < FLEET_BASIC_NEEDS) {
        Event_create(
            "Drought!",
            TextFormat("Captain, our people are dying of thirst!\nWe lacked %d drink last month.\n", FLEET_BASIC_NEEDS - f->drink)
        );
        excessDeaths += floor((FLEET_BASIC_NEEDS - f->drink) * 0.25);
    }
    if (f->oxygen < FLEET_BASIC_NEEDS) {
        Event_create(
            "Asphyxiation!",
            TextFormat("Captain, our people are suffocating!\nWe lacked %d oxygen last month.\n", FLEET_BASIC_NEEDS - f->oxygen)
        );
        excessDeaths += FLEET_BASIC_NEEDS - f->oxygen;
    }
    return excessDeaths;
}

void Fleet_consumeResources(Fleet_Entity* f) {
    // Consume resources
    f->food -= f->lowpop + (f->medpop * 2) + (f->highpop * 4) + (f->ultrapop * 8);
    if (f->food < 0) f->food = 0;
    f->oxygen -= FLEET_BASIC_NEEDS;
    if (f->oxygen < 0) f->oxygen = 0;
    f->drink -= f->lowpop + (f->medpop * 2) + (f->highpop * 4) + (f->ultrapop * 8);
    if (f->drink < 0) f->drink = 0;
    f->recreation -= f->lowpop + (f->medpop * 2) + (f->highpop * 4) + (f->ultrapop * 8);
    if (f->recreation < 0) f->recreation = 0;
    f->education = f->medpop + (f->highpop * 2) + (f->ultrapop * 8);
    if (f->education < 0) f->education = 0;
    f->luxury = f->medpop + (f->highpop * 2) + (f->ultrapop * 8);
    if (f->luxury < 0) f->luxury = 0;
}

int Fleet_newPop(Fleet_Entity* f, float birthChance) {
    int popCurrent = FLEET_BASIC_NEEDS;
    return ceil(popCurrent/2 * birthChance * (100-f->unrest)/100);
}

void Fleet_changePop(Fleet_Entity* f, int excessDeaths) {
    int popMax = 0;
    Unit_Entity u;
    for (int i = 0; i < f->unitmax; i++) {
        u = Unit_getCopy(f->units[i]);
        popMax += u.popMax;
    }
    int popCurrent = FLEET_BASIC_NEEDS;
    int births = 0;
    if (popCurrent <= popMax) {
        switch (f->development) {
            case 0:
                births = Fleet_newPop(f, 0.1) - excessDeaths;
                f->lowpop += births;
                break;
            case 1: 
                births = Fleet_newPop(f, 0.2) - excessDeaths;
                f->lowpop += ceil(births * 0.9);
                f->medpop += ceil(births * 0.1);
                break;
            case 2: 
                births = Fleet_newPop(f, 0.3) - excessDeaths;
                f->lowpop += ceil(births * 0.6);
                f->medpop = ceil(births * 0.35);
                f->highpop = ceil(births * 0.05);
                break;
            case 3: 
                births = Fleet_newPop(f, 0.2) - excessDeaths;
                f->lowpop += ceil(births * 0.2);
                f->medpop += ceil(births * 0.55);
                f->highpop += ceil(births * 0.2);
                f->ultrapop += ceil(births * 0.5);
                break;
            case 4: 
                births = Fleet_newPop(f, 0.1) - excessDeaths;
                f->lowpop += ceil(births * 0.1);
                f->medpop += ceil(births * 0.3);
                f->highpop += ceil(births * 0.4);
                f->ultrapop += ceil(births * 0.2);
                break;
            case 5:
                births = Fleet_newPop(f, 0.05) - excessDeaths;
                f->highpop += ceil(births * 0.6);
                f->ultrapop += ceil(births * 0.4);
                break;
        }
    }
    if (f->lowpop < 0) f->lowpop = 0;
    if (f->medpop < 0) f->medpop = 0;
    if (f->highpop < 0) f->highpop = 0;
    if (f->ultrapop < 0) f->ultrapop = 0;
    while (FLEET_BASIC_NEEDS > popMax) {
        f->lowpop--;
        if (FLEET_BASIC_NEEDS > popMax) f->medpop--;
        if (FLEET_BASIC_NEEDS > popMax) f->highpop--;
        if (FLEET_BASIC_NEEDS > popMax) f->ultrapop--;
    }
}

void Fleet_simulate(Fleet_Entity* f) {
    Fleet_unrestChange(f);
    int excessDeaths = Fleet_excessDeaths(f);
    Fleet_consumeResources(f);
    //TODO: Update development level
    f->development = 0;
    Fleet_changePop(f, excessDeaths);
    Fleet_generateResources(f);
    // TODO: scientists perform research
}

int Fleet_canMine(Fleet_Entity* f) {
    for (int i = 0; i < f->unitmax; i++) {
        Unit_Entity u = Unit_getCopy(f->units[i]);
        if (u.mining > 0) {
            return 1;
        }
    }
    return 0;
}