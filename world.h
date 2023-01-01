#include "sector.h"

#define World_sizeX 36
#define World_sizeY 30

void World_create();
void World_update();
Sector_Entity World_getSectorCopy(int x, int y);
Sector_Entity* World_getSectorPointer(int x, int y);

