#include <string.h>

#include "sector.h"
#include "unit.h"

Unit_Design designs[8];
int designnum = 0;

Unit_Component Unit_allComponents[6] = {};

void Unit_initComponents() {
    // Warp Device I
    strcpy(Unit_allComponents[0].name, "Warp Device I");
    Unit_allComponents[0].warpDriveLevel = 1;
    // Basic Habitation
    strcpy(Unit_allComponents[1].name, "Basic Habitation");
    Unit_allComponents[1].habitationSpace = 50;
    Unit_allComponents[1].buildCosts[0] = (Sector_Resource) {RES_BASE_METALS, 25};
    Unit_allComponents[1].buildCosts[1] = (Sector_Resource) {RES_FABRICS, 10};
    // Basic storage
    strcpy(Unit_allComponents[2].name, "Basic Storage");
    Unit_allComponents[2].storageCapacity = 250;
    Unit_allComponents[2].buildCosts[0] = (Sector_Resource) {RES_BASE_METALS, 50};
    // Basic construction
    strcpy(Unit_allComponents[3].name, "Basic Fabricator");
    Unit_allComponents[3].unitProductionVolume = 50;
    Unit_allComponents[3].buildCosts[0] = (Sector_Resource) {RES_BASE_METALS, 50};
    // Basic mining
    strcpy(Unit_allComponents[4].name, "Basic Miner");
    Unit_allComponents[4].miningVolume = 25;
    Unit_allComponents[4].buildCosts[0] = (Sector_Resource) {RES_BASE_METALS, 50};
    // Basic farm
    strcpy(Unit_allComponents[5].name, "Basic Farm");
    Unit_allComponents[5].foodProduction = 25;
    Unit_allComponents[5].buildCosts[0] = (Sector_Resource) {RES_BASE_METALS, 10};
    Unit_allComponents[5].buildCosts[1] = (Sector_Resource) {RES_FERTILE_SOIL, 100};
}

Unit_Component Unit_getComponent(int index) {
    return Unit_allComponents[index];
}

Unit_Design* Unit_createDesign() {
    Unit_Design* d = &designs[designnum];
    d->id = designnum;
    d->shipSize = 1;
    for (int i = 0; i < UNIT_COMPONENT_MAX; i++) {
        d->components[i] = (Unit_Component) {};
    }
    d->componentnum = 0;
    designnum++;
    return d;
}

int Unit_designProductionCost(Unit_Design* d) {
    int pc = 0;
    for (int i = 0; i < d->componentnum; i++) {
        pc += d->components[i].productionCost;
    }
    return pc;
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
    Unit_Entity* u = &units[unitnum];
    u->hpMax = 0;
    u->shieldMax = 0;
    u->warpDriveLevel = 0;
    u->storage = 0;
    u->popMax = 0;
    u->mining = 0;
    u->resourceMining = RES_NONE;
    Unit_Component* c = 0;
    for (int i = 0; i < design->componentnum; i++) {
        c = &design->components[i];
        u->hpMax += c->armourStrength;
        u->shieldMax += c->shieldStrength;
        if (c->warpDriveLevel > u->warpDriveLevel) {
            u->warpDriveLevel = c->warpDriveLevel;
        }
        u->storage += c->storageCapacity;
        u->popMax += c->habitationSpace;
        u->mining += c->miningVolume;
        u->farming += c->foodProduction;
    }
    u->hp = u->hpMax;
    u->shields = u->shieldMax;
    // Not sure what we're doing about this
    u->fuelMax = 100;
    u->design = design->id;
    unitnum++;
    return u;
}

Unit_Entity Unit_getCopy(int uid) {
    return units[uid];
}

Unit_Entity* Unit_getPointer(int uid) {
    return &units[uid];
}

int Unit_count() {
    return unitnum;
}
