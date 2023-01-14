#ifndef UNIT_H
#define UNIT_H

#include "sector.h"
#include "research.h"

#define UNIT_COMPONENT_MAX 16
#define COMPONENTS_ALL 128
#define WEAPON_MAX 8
#define DESIGN_MAX 64
#define DESIGN_USER 8

typedef struct {
    char name[32];
    int shotPower; // Raw damage output
    int shotPenetration; // How much it can bypass energy shields
    int armourStrength;
    int shieldStrength;
    int sheildRechargeRate;
    int thrusterSpeed;
    int storageCapacity;
    int habitationSpace;
    int goodProductionVolume;
    int goodProduced;
    int miningVolume;
    int unitProductionVolume;
    int foodProduction;
    int size;
    int productionCost;
    Sector_Resource buildCosts[4];
    int researchVolume;
    int totalShipSize;
    Research_TechType techRequired;
    Research_TechType obsoletedBy;
} Unit_Component;

typedef struct {
    int id;
    char name[32];
    int shipSize;
    Unit_Component components[UNIT_COMPONENT_MAX];
    int componentnum;
    int playerCanBuild;
} Unit_Design;

Unit_Component Unit_getComponent(int index);
Unit_Design* Unit_createDesign();
int Unit_designCount();
Unit_Design Unit_getDesignCopy(int id);
Unit_Design* Unit_getDesignPointer(int id);
int Unit_designProductionCost(Unit_Design* d);

typedef struct {
    int storedResourceId;
    int quantity;
} Unit_Inventory;

typedef struct {
    char name[32];
    int hp;
    int hpMax;
    Unit_Component weapons[WEAPON_MAX];
    int shields;
    int shieldMax;
    int shieldRechargeRate;
    int popMax;
    int design;
    int research;
    int production;
    int storage;
    int mining;
    int resourceMining;
    int farming;
    int totalShipStorage;
    int size;
    int pop;
    Unit_Inventory stored[32];
    int storednum;
    int totalStored;
    int canFight;
    int costToBuild;
} Unit_Entity;

Unit_Entity Unit_generate(Unit_Design* design);
Unit_Entity* Unit_create(Unit_Design* design);
Unit_Entity Unit_getCopy(int uid);
Unit_Entity* Unit_getPointer(int uid);
int Unit_count();
int Unit_lastAddedIndex();
void Unit_storeItem(Unit_Entity* u, Unit_Inventory inv);
int Unit_consumeItem(Unit_Entity* u, Sector_Resource r);
int Unit_getTotalStored(Unit_Entity* u, Sector_Resource r);
void Unit_inventoryTransfer(Unit_Entity* from, int invIndex, Unit_Entity* to);
void Unit_removeItemByIndex(Unit_Entity*, int index);

#endif