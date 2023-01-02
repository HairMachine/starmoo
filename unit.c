#include <string.h>
#include "raylib.h"

#include "research.h"
#include "sector.h"
#include "unit.h"

Unit_Design designs[DESIGN_MAX];
int designnum = 0;

Unit_Component Unit_allComponents[COMPONENTS_ALL] = {};

void Unit_initComponents() {
    // Warp Device I
    strcpy(Unit_allComponents[0].name, "Warp Device I");
    Unit_allComponents[0].warpDriveLevel = 1;
    Unit_allComponents[0].techRequired = RT_WARP_2;
    // Basic Habitation
    strcpy(Unit_allComponents[1].name, "Basic Habitation");
    Unit_allComponents[1].habitationSpace = 50;
    Unit_allComponents[1].buildCosts[0] = (Sector_Resource) {RES_BASE_METALS, 25};
    Unit_allComponents[1].buildCosts[1] = (Sector_Resource) {RES_FABRICS, 10};
    Unit_allComponents[1].techRequired = RT_NONE;
    // Basic storage
    strcpy(Unit_allComponents[2].name, "Basic Storage");
    Unit_allComponents[2].storageCapacity = 250;
    Unit_allComponents[2].buildCosts[0] = (Sector_Resource) {RES_BASE_METALS, 50};
    Unit_allComponents[2].techRequired = RT_NONE;
    // Basic construction
    strcpy(Unit_allComponents[3].name, "Basic Fabricator");
    Unit_allComponents[3].unitProductionVolume = 50;
    Unit_allComponents[3].buildCosts[0] = (Sector_Resource) {RES_BASE_METALS, 50};
    Unit_allComponents[3].techRequired = RT_NONE;
    // Basic mining
    strcpy(Unit_allComponents[4].name, "Basic Miner");
    Unit_allComponents[4].miningVolume = 25;
    Unit_allComponents[4].buildCosts[0] = (Sector_Resource) {RES_BASE_METALS, 50};
    Unit_allComponents[4].techRequired = RT_NONE;
    // Basic farm
    strcpy(Unit_allComponents[5].name, "Basic Farm");
    Unit_allComponents[5].foodProduction = 25;
    Unit_allComponents[5].buildCosts[0] = (Sector_Resource) {RES_BASE_METALS, 10};
    Unit_allComponents[5].buildCosts[1] = (Sector_Resource) {RES_FERTILE_SOIL, 100};
    Unit_allComponents[5].techRequired = RT_NONE;
    // Research lab
    strcpy(Unit_allComponents[6].name, "Research Lab");
    for (int i = 0; i < FIELD_ALL; i++) {
        Unit_allComponents[6].researchVolume[i] = 25;
    }
    Unit_allComponents[6].buildCosts[0] = (Sector_Resource) {RES_BASE_METALS, 100};
    Unit_allComponents[6].techRequired = RT_NONE;
    // Laser cannon
    strcpy(Unit_allComponents[7].name, "Laser Cannon");
    Unit_allComponents[7].shotPower = 10;
    Unit_allComponents[7].shotPenetration = 25;
    Unit_allComponents[7].buildCosts[0] = (Sector_Resource) {RES_BASE_METALS, 25};
    Unit_allComponents[7].techRequired = RT_NONE;
    // Shield I
    strcpy(Unit_allComponents[8].name, "Sheild I");
    Unit_allComponents[8].shieldStrength = 10;
    Unit_allComponents[8].sheildRechargeRate = 1;
    Unit_allComponents[8].buildCosts[0] = (Sector_Resource) {RES_MAGNETRIUM, 50};
    Unit_allComponents[8].buildCosts[1] = (Sector_Resource) {RES_BASE_METALS, 50};
    Unit_allComponents[8].techRequired = RT_NONE;
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
    return designnum;
}

Unit_Design Unit_getDesignCopy(int id) {
    return designs[id];
}

Unit_Design* Unit_getDesignPointer(int id) {
    return &designs[id];
}

Unit_Entity units[1024];
int unitnum = 0;
int addIndex = 0;

Unit_Entity Unit_generate(Unit_Design* design) {
    Unit_Entity u = (Unit_Entity) {};
    strcpy(u.name, TextFormat("%s %d", design->name, addIndex));
    u.hpMax = 100;
    Unit_Component* c = 0;
    int numweapons = 0;
    for (int i = 0; i < design->componentnum; i++) {
        c = &design->components[i];
        u.hpMax += c->armourStrength;
        u.shieldMax += c->shieldStrength;
        if (c->warpDriveLevel > u.warpDriveLevel) {
            u.warpDriveLevel = c->warpDriveLevel;
        }
        u.storage += c->storageCapacity;
        u.popMax += c->habitationSpace;
        u.mining += c->miningVolume;
        u.farming += c->foodProduction;
        u.production += c->unitProductionVolume;
        for (int j = 0; j < FIELD_ALL; j++) {
            u.research[j] += c->researchVolume[j];
        }
        // if it's a weapon put the component into the weapons slot
        if (c->shotPower || c->shotPenetration) {
            u.weapons[numweapons] = *c;
            numweapons++;
            u.canFight = 1;
        }
        // Calculate production cost
        for (int j = 0; j < 4; j++) {
            u.costToBuild += c->buildCosts[j].abundance;
        }
    }
    u.hp = u.hpMax;
    u.shields = u.shieldMax;
    // Not sure what we're doing about this
    u.design = design->id;
    return u;
}

Unit_Entity* Unit_create(Unit_Design* design) {
    addIndex = unitnum;
    for (int i = 0; i < unitnum; i++) {
        if (units[i].hp <= 0) {
            addIndex = i;
            unitnum--;
            break;
        }
    }
    units[addIndex] = Unit_generate(design);
    Unit_Entity* u = &units[addIndex];
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

int Unit_lastAddedIndex() {
    return addIndex;
}

void Unit_storeItem(Unit_Entity* u, Unit_Inventory inv) {
    u->totalStored += inv.quantity;
    for (int i = 0; i < u->storednum; i++) {
        Unit_Inventory* existingInv = &u->stored[i];
        if (existingInv->storedResourceId == inv.storedResourceId) {
            existingInv->quantity += inv.quantity;
            return;
        }
    }
    u->stored[u->storednum] = inv;
    u->storednum++;
}

int Unit_consumeItem(Unit_Entity* u, Sector_Resource r) {
    if (u->storednum == 0) {
        return 0;
    }
    for (int i = 0; i < u->storednum; i++) {
        Unit_Inventory* existingInv = &u->stored[i];
        if (existingInv->storedResourceId == r.type) {
            if (existingInv->quantity >= r.abundance) {
                existingInv->quantity -= r.abundance;
            } else {
                return 0;
            }
        }
    }
    return 1;
}

void Unit_inventoryTransfer(Unit_Entity* from, int invIndex, Unit_Entity* to) {
    Unit_storeItem(to, from->stored[invIndex]);
    Unit_removeItemByIndex(from, invIndex);
}

void Unit_removeItemByIndex(Unit_Entity* u, int index) {
    u->totalStored -= u->stored[index].quantity;
    for (int i = index; i < u->storednum; i++) {
        u->stored[i] = u->stored[i + 1];
    }
    u->storednum--;
}
