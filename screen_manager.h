#include "sector.h"

void ScreenManager_init();
void ScreenCombat_init();
void ScreenCargo_init();
void ScreenFleet_init();
void ScreenMap_init();
void ScreenSystem_init();
void ScreenResearch_init();
Sector_Entity* ScreenManager_currentSector();
void ScreenManager_setCurrentSector(Sector_Entity* s);
