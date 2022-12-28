#include "sector.h"

#define UNIT_COMPONENT_MAX 32

typedef struct {
    char name[32];
    int shotPower; // Raw damage output
    int shotPenetration; // How much it can bypass energy shields
    int shotProjectileSpeed; // How quickly the projectile moves; 0 is a direct damage weapon
    int shotCooldown; // Cooldown between each shot
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
} Unit_Component;

typedef struct {
    int id;
    char name[32];
    int shipSize;
    Unit_Component components[UNIT_COMPONENT_MAX];
    int componentnum;
} Unit_Design;

void Unit_initComponents();
Unit_Component Unit_getComponent(int index);
Unit_Design* Unit_createDesign();
int Unit_designCount();
Unit_Design Unit_getDesignCopy(int id);
Unit_Design* Unit_getDesignPointer(int id);
int Unit_designProductionCost(Unit_Design* d);

typedef struct {
    int hp;
    int hpMax;
    Unit_Component components[8];
    int shields;
    int shieldMax;
    int fuelMax;
    int popMax;
    int warpDriveLevel; // Able to carry other ships in the fleet through a space warp if > 0
    int design;
    int research;
    int production;
    int storage;
    int mining;
    Sector_Resource resourceMining;
    int farming;
    int stored[32];
    int storednum;
    int totalStored;
} Unit_Entity;

Unit_Entity* Unit_create(Unit_Design* design);
Unit_Entity Unit_getCopy(int uid);
Unit_Entity* Unit_getPointer(int uid);
int Unit_count();
void Unit_storeItem(Unit_Entity*, int invId);
void Unit_removeItem(Unit_Entity*, int invId);