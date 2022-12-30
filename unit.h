#ifndef UNIT_H
#define UNIT_H

#include "sector.h"
#include "research.h"

#define UNIT_COMPONENT_MAX 32
#define WEAPON_MAX 8

typedef struct {
    char name[32];
    int shotPower; // Raw damage output
    int shotPenetration; // How much it can bypass energy shields
    int armourStrength;
    int shieldStrength;
    int sheildRechargeRate;
    int warpDriveLevel;
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
    int researchVolume[FIELD_ALL];
} Unit_Component;

typedef struct {
    int id;
    char name[32];
    int shipSize;
    Unit_Component components[UNIT_COMPONENT_MAX];
    int componentnum;
    int playerCanBuild;
} Unit_Design;

void Unit_initComponents();
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
    int fuelMax;
    int popMax;
    int warpDriveLevel; // Able to carry other ships in the fleet through a space warp if > 0
    int design;
    int research[FIELD_ALL];
    int production;
    int storage;
    int mining;
    Sector_Resource resourceMining;
    int farming;
    Unit_Inventory stored[32];
    int storednum;
    int totalStored;
    int canFight;
} Unit_Entity;

Unit_Entity Unit_generate(Unit_Design* design);
Unit_Entity* Unit_create(Unit_Design* design);
Unit_Entity Unit_getCopy(int uid);
Unit_Entity* Unit_getPointer(int uid);
int Unit_count();
void Unit_storeItem(Unit_Entity* u, Unit_Inventory inv);
void Unit_inventoryTransfer(Unit_Entity* from, int invIndex, Unit_Entity* to);
void Unit_removeItemByIndex(Unit_Entity*, int index);

#endif