typedef struct {
    int power; // Raw damage output
    int penetration; // How much it can bypass energy shields
    int projectileSpeed; // How quickly the projectile moves; 0 is a direct damage weapon
    int cooldown; // Cooldown between each shot
} Unit_Weapon;

typedef struct {
    int id;
    char name[32];
    Unit_Weapon weapons[8];
    int shipSize;
    int warpDriveLevel;
    int armourLevel;
    int shieldLevel;
    int passengerBays;
    int fuelTanks;
    int researchLabs;
    int constructionBays;
    int storageBays;
    int miningRigs;
    int farmingBays;
} Unit_Design;

Unit_Design* Unit_createDesign();
int Unit_designCount();
Unit_Design Unit_getDesignCopy(int id);
Unit_Design* Unit_getDesignPointer(int id);

typedef struct {
    int hp;
    int hpMax;
    Unit_Weapon weapons[8];
    int shields;
    int shieldMax;
    int fuelMax;
    int passengerMax;
    int warpDriveLevel; // Able to carry other ships in the fleet through a space warp if > 0
    int design;
    int research;
    int production;
    int storage;
    int mining;
    int farming;
} Unit_Entity;

Unit_Entity* Unit_create(Unit_Design* design);
Unit_Entity Unit_getCopy(int uid);
int Unit_count();