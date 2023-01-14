#include <math.h>
#include "raylib.h"

#include "event.h"
#include "unit.h"
#include "research.h"
#include "fleet.h"

Fleet_Entity fleets[8];
int fleetnum;

Fleet_Upgrade upgrades[MAXUPGRADES] = {
    (Fleet_Upgrade) {
        .name = "Development Level 1",
        .buildCosts = {
            {RES_BASE_METALS, 500},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .development = 1,
        .obsoletedBy = RT_ALL
    },
    (Fleet_Upgrade) {
        .name = "Development Level 2",
        .buildCosts = {
            {RES_BASE_METALS, 1000},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .development = 2,
        .obsoletedBy = RT_ALL
    },
    (Fleet_Upgrade) {
        .name = "Development Level 3",
        .buildCosts = {
            {RES_BASE_METALS, 2000},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .development = 3,
        .obsoletedBy = RT_ALL
    },
    (Fleet_Upgrade) {
        .name = "Development Level 4",
        .buildCosts = {
            {RES_BASE_METALS, 4000},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .development = 4,
        .obsoletedBy = RT_ALL
    },
    (Fleet_Upgrade) {
        .name = "Development Level 5",
        .buildCosts = {
            {RES_BASE_METALS, 8000},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .development = 5,
        .obsoletedBy = RT_ALL
    },
    (Fleet_Upgrade) {
        .name = "Warp Factor 2",
        .buildCosts = {
            {RES_BASE_METALS, 100},
            {RES_DEUTERIUM, 100},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .warpFactor = 2,
        .techRequired = RT_WARP_2,
        .obsoletedBy = RT_WARP_3
    },
    (Fleet_Upgrade) {
        .name = "Warp Factor 3",
        .buildCosts = {
            {RES_BASE_METALS, 200},
            {RES_DEUTERIUM, 200},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .warpFactor = 3,
        .techRequired = RT_WARP_3,
        .obsoletedBy = RT_WARP_4
    },
    (Fleet_Upgrade) {
        .name = "Warp Factor 4",
        .buildCosts = {
            {RES_BASE_METALS, 400},
            {RES_DEUTERIUM, 400},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .warpFactor = 4,
        .techRequired = RT_WARP_4,
        .obsoletedBy = RT_WARP_5
    },
    (Fleet_Upgrade) {
        .name = "Warp Factor 5",
        .buildCosts = {
            {RES_BASE_METALS, 800},
            {RES_DEUTERIUM, 800},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .warpFactor = 5,
        .techRequired = RT_WARP_5,
        .obsoletedBy = RT_WARP_6
    },
    (Fleet_Upgrade) {
        .name = "Warp Factor 6",
        .buildCosts = {
            {RES_BASE_METALS, 1600},
            {RES_DEUTERIUM, 1600},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .warpFactor = 6,
        .techRequired = RT_WARP_6,
        .obsoletedBy = RT_WARP_7
    },
    (Fleet_Upgrade) {
        .name = "Warp Factor 7",
        .buildCosts = {
            {RES_BASE_METALS, 3200},
            {RES_DEUTERIUM, 3200},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .warpFactor = 7,
        .techRequired = RT_WARP_7,
        .obsoletedBy = RT_WARP_8
    },
    (Fleet_Upgrade) {
        .name = "Warp Factor 8",
        .buildCosts = {
            {RES_BASE_METALS, 6400},
            {RES_DEUTERIUM, 6400},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .warpFactor = 8,
        .techRequired = RT_WARP_8,
        .obsoletedBy = RT_WARP_9
    },
    (Fleet_Upgrade) {
        .name = "Warp Factor 9",
        .buildCosts = {
            {RES_BASE_METALS, 12800},
            {RES_DEUTERIUM, 12800},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .warpFactor = 9,
        .techRequired = RT_WARP_9,
        .obsoletedBy = RT_WARP_10
    },
    (Fleet_Upgrade) {
        .name = "Warp Factor 10",
        .buildCosts = {
            {RES_BASE_METALS, 25600},
            {RES_DEUTERIUM, 25600},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .warpFactor = 10,
        .techRequired = RT_WARP_10,
        .obsoletedBy = RT_ALL
    },
    (Fleet_Upgrade) {
        .name = "Ship Bays 1",
        .buildCosts = {
            {RES_BASE_METALS, 100},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .sizeIncrease = 10000,
        .techRequired = RT_CARRIER_1,
        .obsoletedBy = RT_CARRIER_2
    },
    (Fleet_Upgrade) {
        .name = "Ship Bays 2",
        .buildCosts = {
            {RES_BASE_METALS, 200},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .sizeIncrease = 20000,
        .techRequired = RT_CARRIER_2,
        .obsoletedBy = RT_CARRIER_3
    },
    (Fleet_Upgrade) {
        .name = "Ship Bays 3",
        .buildCosts = {
            {RES_BASE_METALS, 400},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .sizeIncrease = 40000,
        .techRequired = RT_CARRIER_3,
        .obsoletedBy = RT_CARRIER_4
    },
    (Fleet_Upgrade) {
        .name = "Ship Bays 4",
        .buildCosts = {
            {RES_BASE_METALS, 800},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .sizeIncrease = 80000,
        .techRequired = RT_CARRIER_4,
        .obsoletedBy = RT_CARRIER_5
    },
    (Fleet_Upgrade) {
        .name = "Ship Bays 5",
        .buildCosts = {
            {RES_BASE_METALS, 1600},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .sizeIncrease = 160000,
        .techRequired = RT_CARRIER_5,
        .obsoletedBy = RT_ALL
    },
};

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
    f->food += newUnit.farming;
    f->size += newUnit.size;
    f->maxSize += newUnit.totalShipStorage;
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

void Fleet_applyUpgrade(Fleet_Entity* f, int upgrade) {
    Fleet_Upgrade* u = &upgrades[upgrade];
    f->development = u->development;
    f->warpFactor = u->warpFactor;
    f->maxSize = u->sizeIncrease;
    u->complete = 1; 
}

Fleet_Upgrade Fleet_getUpgrade(int upgrade) {
    return upgrades[upgrade];
}

