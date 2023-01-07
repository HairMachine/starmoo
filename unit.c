#include <string.h>
#include "raylib.h"

#include "research.h"
#include "sector.h"
#include "unit.h"

Unit_Design designs[DESIGN_MAX];
int designnum = 0;

Unit_Component Unit_allComponents[COMPONENTS_ALL] = {
    (Unit_Component) { // 0
        .name = "Warp Device I",
        .warpDriveLevel = 1,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) { // 1
        .name = "Basic Habitation",
        .habitationSpace = 50,
        .size = 500,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_FABRICS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) { // 2
        .name = "Basic Storage",
        .storageCapacity = 1000,
        .size = 10,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) {
        .name = "Basic Fabricator",
        .unitProductionVolume = 50,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) {
        .name="Basic Miner",
        .miningVolume = 50,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) {
        .name="Basic Farm",
        .foodProduction = 25,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 1},
            {RES_FERTILE_SOIL, 10},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) {
        .name="Research Lab",
        .researchVolume = {
            25, 25, 25, 25, 25, 25
        },
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) {
        .name="Laser Cannon",
        .shotPower = 10,
        .shotPenetration = 25,
        .size = 25,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) {
        .name="Shield I",
        .shieldStrength = 10,
        .sheildRechargeRate = 1,
        .size = 50,
        .buildCosts = {
            {RES_BASE_METALS, 25},
            {RES_MAGNETRIUM, 25},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) {
        .name="Gun Battery",
        .shotPower = 25,
        .shotPenetration = 10,
        .size = 25,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) {
        .name="Titanium Armour",
        .armourStrength = 100,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    (Unit_Component) {
        .name="Small Ship Bay",
        .maxAllowedShipSize = 500,
        .totalShipSize = 3000,
        .buildCosts = {
            {RES_BASE_METALS, 5},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        }
    },
    // Warp components
    (Unit_Component) {
        .name = "Warp Device II",
        .warpDriveLevel = 2,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_2
    },
    (Unit_Component) {
        .name = "Warp Device III",
        .warpDriveLevel = 3,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_3
    },
    (Unit_Component) {
        .name = "Warp Device IV",
        .warpDriveLevel = 4,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_4
    },
    (Unit_Component) {
        .name = "Warp Device V",
        .warpDriveLevel = 5,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_5
    },
    (Unit_Component) {
        .name = "Warp Device VI",
        .warpDriveLevel = 6,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_6
    },
    (Unit_Component) {
        .name = "Warp Device VII",
        .warpDriveLevel = 7,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_7
    },
    (Unit_Component) {
        .name = "Warp Device VIII",
        .warpDriveLevel = 8,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_8
    },
    (Unit_Component) {
        .name = "Warp Device IX",
        .warpDriveLevel = 9,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_9
    },
    (Unit_Component) {
        .name = "Warp Device X",
        .warpDriveLevel = 10,
        .size = 1000,
        .buildCosts = {
            {RES_WARP_SEEDS, 1},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_WARP_10
    },
    // Construction components
    (Unit_Component) {
        .name="Armour 2",
        .armourStrength = 200,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 75},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_ARMOUR_2
    },
    (Unit_Component) {
        .name="Armour 3",
        .armourStrength = 400,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 120},
            {RES_HYPERALLOYS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_ARMOUR_3
    },
    (Unit_Component) {
        .name="Armour 4",
        .armourStrength = 800,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 120},
            {RES_HYPERALLOYS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_ARMOUR_4
    },
    (Unit_Component) {
        .name="Armour 5",
        .armourStrength = 1600,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 120},
            {RES_HYPERALLOYS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_ARMOUR_5
    },
    (Unit_Component) {
        .name="Armour 6",
        .armourStrength = 3200,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 120},
            {RES_HYPERALLOYS, 10},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_ARMOUR_6
    },
    (Unit_Component) {
        .name="Miner 2",
        .miningVolume = 50,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_MINING_2
    },
    (Unit_Component) {
        .name="Miner 3",
        .miningVolume = 100,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_MINING_3
    },
    (Unit_Component) {
        .name="Miner 4",
        .miningVolume = 200,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_MINING_4
    },
    (Unit_Component) {
        .name="Miner 5",
        .miningVolume = 400,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_MINING_5
    },
    (Unit_Component) {
        .name="Miner 6",
        .miningVolume = 800,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_MINING_6
    },
    (Unit_Component) {
        .name="Moderate Ship Bay",
        .maxAllowedShipSize = 1000,
        .totalShipSize = 6000,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CARRIER_1
    },
    (Unit_Component) {
        .name="Medium Ship Bay",
        .maxAllowedShipSize = 1000,
        .totalShipSize = 6000,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CARRIER_2
    },
    (Unit_Component) {
        .name="Large Ship Bay",
        .maxAllowedShipSize = 2000,
        .totalShipSize = 12000,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CARRIER_3
    },
    (Unit_Component) {
        .name="Huge Ship Bay",
        .maxAllowedShipSize = 4000,
        .totalShipSize = 24000,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CARRIER_4
    },
    (Unit_Component) {
        .name="Massive Ship Bay",
        .maxAllowedShipSize = 8000,
        .totalShipSize = 48000,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CARRIER_5
    },
    (Unit_Component) {
        .name = "Improved Fabricator",
        .unitProductionVolume = 100,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CONSTRUCT_1
    },
    (Unit_Component) {
        .name = "Advanced Fabricator",
        .unitProductionVolume = 200,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CONSTRUCT_2
    },
    (Unit_Component) {
        .name = "Molecular Replicator",
        .unitProductionVolume = 400,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CONSTRUCT_3
    },
    (Unit_Component) {
        .name = "Advanced Replication",
        .unitProductionVolume = 1600,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CONSTRUCT_4
    },
    (Unit_Component) {
        .name = "Quantum Constructor",
        .unitProductionVolume = 3200,
        .size = 100,
        .buildCosts = {
            {RES_BASE_METALS, 50},
            {RES_NONE, 0},
            {RES_NONE, 0},
            {RES_NONE, 0}
        },
        .techRequired = RT_CONSTRUCT_5
    },
};

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
            u.costToBuild += c->buildCosts[j].abundance * 100;
        }
        u.size += c->size;
        u.totalShipStorage += c->totalShipSize;
        if (c->maxAllowedShipSize > u.allowsShipSize) {
            u.allowsShipSize = c->maxAllowedShipSize;
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
