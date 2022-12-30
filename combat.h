#include "unit.h"
#include "fleet.h"

#define PLAYER_SIDE 0
#define ENEMY_SIDE 1

typedef struct {
    Unit_Entity entity;
    int side;
    int unitIndex;
    int fleetIndex;
} Combat_Unit;

void Combat_createEnemyDesigns();
void Combat_addShipToCombat(Unit_Entity u, int side, int originalIndex, int fleetIndex);
void Combat_addFleetShipsToCombat(Fleet_Entity* f);
void Combat_setupRandomEncounter();
void Combat_run();
int Combat_unitCount();
Combat_Unit Combat_getUnitCopy(int index);
int Combat_active();
