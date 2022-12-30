#include "unit.h"

#define PLAYER_SIDE 0
#define ENEMY_SIDE 1

typedef struct {
    Unit_Entity entity;
    int side;
    int originalIndex;
} Combat_Unit;

void Combat_createEnemyDesigns();
void Combat_addShipToCombat(Unit_Entity u, int side, int originalIndex);
void Combat_setupRandomEncounter();
void Combat_run();
int Combat_unitCount();
Combat_Unit Combat_getUnitCopy(int index);
int Combat_active();
