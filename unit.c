#include "unit.h"

Unit_Design designs[8];
int designnum = 0;

Unit_Design* Unit_createDesign() {
    Unit_Design* d = &designs[designnum];
    d->id = designnum;
    d->shipSize = 1;
    d->warpDriveLevel = 0;
    d->armourLevel = 1;
    d->shieldLevel = 0;
    d->passengerBays = 0;
    d->fuelTanks = 0;
    d->researchLabs = 0;
    d->constructionBays = 0;
    d->storageBays = 0;
    d->miningRigs = 0;
    d->farmingBays = 0;
    designnum++;
    return d;
}

int Unit_designCount() {
    return 8;
}

Unit_Design Unit_getDesignCopy(int id) {
    return designs[id];
}

Unit_Design* Unit_getDesignPointer(int id) {
    return &designs[id];
}

Unit_Entity units[1024];
int unitnum = 0;

Unit_Entity* Unit_create(Unit_Design* design) {
    // TODO: These values are modified by technological advances.
    Unit_Entity* u = &units[unitnum];
    u->hp = design->armourLevel * 100;
    u->hpMax = u->hp;
    u->warpDriveLevel = design->warpDriveLevel;
    u->passengerMax = design->passengerBays * 10;
    u->shields = design->shieldLevel * 50;
    u->fuelMax = design->fuelTanks;
    u->design = design->id;
    u->research = design->researchLabs * 10;
    u->production = design->constructionBays * 10;
    u->storage = design->storageBays * 100;
    u->mining = design->miningRigs * 10;
    u->farming = design->farmingBays * 10;
    u->storage = design->storageBays * 250;
    unitnum++;
    return u;
}

Unit_Entity Unit_getCopy(int uid) {
    return units[uid];
}

int Unit_count() {
    return unitnum;
}
