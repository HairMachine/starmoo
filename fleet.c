#include <math.h>
#include "raylib.h"

#include "event.h"
#include "unit.h"
#include "research.h"
#include "fleet.h"

Fleet_Entity fleets[8];
int fleetnum;

Fleet_Entity* Fleet_create() {
    Fleet_Entity* f = &fleets[fleetnum];
    f->orders = -1;
    fleetnum++;
    return f;
}

void Fleet_addUnit(Fleet_Entity* f, int uid) {
    f->units[f->unitmax] = uid;
    f->unitmax++;
    // Recalculate supplies
    Unit_Entity newUnit = Unit_getCopy(uid);
    // Set warp factor
    if (newUnit.warpDriveLevel > 0 && newUnit.warpDriveLevel > f->warpFactor) {
        f->warpFactor = newUnit.warpDriveLevel;
    }
    f->food += newUnit.farming;
    f->size += newUnit.size;
    f->maxSize += newUnit.totalShipStorage;
    if (f->largestAllowedShip < newUnit.allowsShipSize) {
        f->largestAllowedShip = newUnit.allowsShipSize;
    }
}

void Fleet_removeUnitAtIndex(Fleet_Entity* f, int index) {
    for (int i = index; i < f->unitmax; i++) {
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

void _generateResources(Fleet_Entity* f) {
    // TODO: Full product / resource system as described in design doc
}

int _createUnrest(Fleet_Entity* f, int need, int notBasic) {
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

void _unrestChange(Fleet_Entity* f) {
    int unrestChange = 
        _createUnrest(f, f->recreation, 0) +
        _createUnrest(f, floor(f->luxury / 2), 1) +
        _createUnrest(f, floor(f->education / 2), 1);
    f->unrest += floor(unrestChange * 0.10);
    if (unrestChange == 0) f->unrest -= 10;
    if (f->unrest < 0) f->unrest = 0;
    if (f->unrest > 100) f->unrest = 100;
}

int _excessDeaths(Fleet_Entity* f) {
    int excessDeaths = 0;
    return excessDeaths;
}

void _consumeResources(Fleet_Entity* f) {
    f->recreation -= f->lowpop + (f->medpop * 2) + (f->highpop * 4) + (f->ultrapop * 8);
    if (f->recreation < 0) f->recreation = 0;
    f->education = f->medpop + (f->highpop * 2) + (f->ultrapop * 8);
    if (f->education < 0) f->education = 0;
    f->luxury = f->medpop + (f->highpop * 2) + (f->ultrapop * 8);
    if (f->luxury < 0) f->luxury = 0;
}

int _newPop(Fleet_Entity* f, float birthChance) {
    int popCurrent = FLEET_BASIC_NEEDS;
    return ceil(popCurrent/2 * birthChance * (100-f->unrest)/100);
}

void _changePop(Fleet_Entity* f, int excessDeaths) {
    // TODO: development level
    int popMax = f->development * 50 + 50;
    int popCurrent = FLEET_BASIC_NEEDS;
    int births = 0;
    if (popCurrent < popMax) {
        switch (f->development) {
            case 0:
                births = _newPop(f, 0.1) - excessDeaths;
                f->lowpop += births;
                break;
            case 1: 
                births = _newPop(f, 0.2) - excessDeaths;
                f->lowpop += ceil(births * 0.9);
                f->medpop += ceil(births * 0.1);
                break;
            case 2: 
                births = _newPop(f, 0.3) - excessDeaths;
                f->lowpop += ceil(births * 0.6);
                f->medpop = ceil(births * 0.35);
                f->highpop = ceil(births * 0.05);
                break;
            case 3: 
                births = _newPop(f, 0.2) - excessDeaths;
                f->lowpop += ceil(births * 0.2);
                f->medpop += ceil(births * 0.55);
                f->highpop += ceil(births * 0.2);
                f->ultrapop += ceil(births * 0.5);
                break;
            case 4: 
                births = _newPop(f, 0.1) - excessDeaths;
                f->lowpop += ceil(births * 0.1);
                f->medpop += ceil(births * 0.3);
                f->highpop += ceil(births * 0.4);
                f->ultrapop += ceil(births * 0.2);
                break;
            case 5:
                births = _newPop(f, 0.05) - excessDeaths;
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
    _unrestChange(f);
    int excessDeaths = _excessDeaths(f);
    _consumeResources(f);
    //TODO: Update development level
    f->development = 0;
    _changePop(f, excessDeaths);
    _generateResources(f);
    // Reset all unit turn state
    f->hasBuiltThisTurn = 0;
}

int Fleet_canMine(Fleet_Entity* f) {
    for (int i = 0; i < f->unitmax; i++) {
        Unit_Entity* u = Unit_getPointer(f->units[i]);
        if (u->mining > 0) {
            return 1;
        }
    }
    return 0;
}

int Fleet_canResearch(Fleet_Entity* f) {
    for (int i = 0; i < f->unitmax; i++) {
        Unit_Entity* u = Unit_getPointer(f->units[i]);
        if (u->research > 0) {
            return 1;
        }
    }
    return 0;
}

int Fleet_getUsedStorage(Fleet_Entity* f) {
    int total = 0;
    for (int i = 0; i < f->unitmax; i++) {
        Unit_Entity* u = Unit_getPointer(f->units[i]);
        total += u->totalStored;
    }
    return total;
}

int Fleet_getMaxStorage(Fleet_Entity* f) {
    int total = 0;
    for (int i = 0; i < f->unitmax; i++) {
        Unit_Entity* u = Unit_getPointer(f->units[i]);
        total += u->storage;
    }
    return total;
}

int Fleet_hasEnoughItems(Fleet_Entity* f, Sector_Resource r) {
    int totalStored = 0;
    for (int i = 0; i < f->unitmax; i++) {
        totalStored += Unit_getTotalStored(Unit_getPointer(f->units[i]), r);
    }
    return totalStored >= r.abundance;
}

void Fleet_consumeItems(Fleet_Entity* f, Sector_Resource r) {
    Unit_Entity* u;
    for (int i = 0; i < f->unitmax; i++) {
        u = Unit_getPointer(f->units[i]);
        int stored = Unit_getTotalStored(u, r);
        int amountToTake = r.abundance > stored ? stored : r.abundance;
        Unit_consumeItem(u, (Sector_Resource) {r.type, amountToTake});
        r.abundance -= amountToTake;
        if (r.abundance <= 0) {
            return;
        }
    }
    return;
}

int Fleet_getAssignedPop(Fleet_Entity* f) {
    int pop = 0;
    for (int i = 0; i < f->unitmax; i++) {
        pop += Unit_getPointer(f->units[i])->pop;
    }
    return pop;
}

