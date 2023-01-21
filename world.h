#include "sector.h"

#define World_sizeX 30
#define World_sizeY 30

void World_create();
void World_update();
void World_explore(int startX, int startY, int range);
Sector_Entity World_getSectorCopy(int x, int y);
Sector_Entity* World_getSectorPointer(int x, int y);
int World_getLegitimacy();
void World_changeLegitimacy(int amnt);