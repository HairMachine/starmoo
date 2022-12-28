#include <math.h>

#include "event.h"
#include "fleet.h"
#include "unit.h"
#include "ui.h"
#include "inventory.h"
#include "screen_manager.h"

int chooseResourcesForMining = 0;
int chooseShipForMining = 0;
int chooseShipForRetrieval = 0;
Sector_Planet* currentPlanetInfo = 0;

void _resourceSelectEnable(UI_Element* el) {
    el->visible = chooseResourcesForMining && chooseShipForMining;
}

void _miningShipSelectEnable(UI_Element *el) {
    el->visible = chooseShipForMining && !chooseResourcesForMining;
}

void _drawMiningShipSelect(UI_Element *el) {
    UI_drawPanel(el);
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    Unit_Entity* u = 0;
    for (int i = 0; i < f->unitmax; i++) {
        u = Unit_getPointer(f->units[i]);
        if (u->mining > 0) {
            UI_drawSelectListItem(el, i, 16, u->name, chooseShipForMining - 2 >= 0 && chooseShipForMining - 2 == i);    
        }
    }
}

void _clickMiningShipSelection(UI_Element *el, Vector2 mpos) {
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    chooseShipForMining = UI_handleSelectList(el, mpos, f->unitmax, 16) + 2;
}

void _clickConfirmMiningShipSelect(UI_Element *el, Vector2 mpos) {
    chooseResourcesForMining = 1;
}

void _clickConfirmResourceSelection(UI_Element* el, Vector2 mpos) {
    if (chooseResourcesForMining < 2) {
        chooseResourcesForMining = 0;
        chooseShipForMining = 0;
        return;
    }
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    int chosenShip = f->units[chooseShipForMining - 2];
    Unit_Entity* u = Unit_getPointer(chosenShip);
    Sector_deployUnitToPlanet(currentPlanetInfo, chosenShip);
    u->resourceMining = currentPlanetInfo->resources[chooseResourcesForMining - 2];
    Fleet_removeUnitAtIndex(f, chooseShipForMining - 2);
    chooseResourcesForMining = 0;
    chooseShipForMining = 0;
}

void _drawResourceSelect(UI_Element* el) {
    UI_drawPanel(el);
    for (int i = 0; i < currentPlanetInfo->resourcenum; i++) {
        UI_drawSelectListItem(
            el, i, 16, 
            Sector_resourceStrings[currentPlanetInfo->resources[i].type], 
            chooseResourcesForMining >= 2 && chooseResourcesForMining - 2 == i
        );
    }
}


void _clickResourceSelect(UI_Element* el, Vector2 mpos) {
    chooseResourcesForMining = UI_handleSelectList(el, mpos, currentPlanetInfo->resourcenum, 16) + 2;
}

void _deployMineEnable(UI_Element *el, Vector2 mpos) {
    if (UI_getScreen() != SCREEN_SYSTEM || !currentPlanetInfo) {
        el->visible = 0;
        return;
    }
    if (currentPlanetInfo->resourcenum <= 0) {
        el->visible = 0;
    } else {
        el->visible = 1;
        if (Fleet_canMine(Fleet_getPointer(ScreenManager_currentSector()->fleet))) {
            el->enabled = 1;
        } else {
            el->enabled = 0;
        }
    }
}

void _clickMineEnable(UI_Element *el, Vector2 mpos) {
    chooseShipForMining = 1;
}

void _collectResourceEnable(UI_Element *el) {
    if (UI_getScreen() == SCREEN_SYSTEM &&  currentPlanetInfo) {
        el->visible = 1;
        if (currentPlanetInfo->unitnum > 0) {
            for (int i = 0; i < currentPlanetInfo->unitnum; i++) {
                Unit_Entity* uc = Unit_getPointer(currentPlanetInfo->units[i]);
                if (uc->storednum > 0) {
                    el->enabled = 1;
                    return;
                }
            }
        }
        el->enabled = 0;
    } else {
        el->visible = 0;
    }
}

void _clickCollectResource(UI_Element* el) {
    Unit_Entity* uc = 0;
    Unit_Entity* ul = 0;
    Inventory_Entity* inv = 0;
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    int collected = 0;
    for (int i = 0; i < currentPlanetInfo->unitnum; i++) {
        uc = Unit_getPointer(currentPlanetInfo->units[i]);
        for (int j = 0; j < uc->storednum; j++) {
            inv = Inventory_getPointer(uc->stored[j]);
            // Find the closes ship in the fleet with space
            for (int k = 0; k < f->unitmax; k++) {
                ul = Unit_getPointer(f->units[k]);
                if (ul->storage - ul->totalStored > inv->quantity) {
                    Unit_removeItem(uc, j);
                    uc->totalStored -= inv->quantity;
                    Unit_storeItem(ul, j);
                    ul->totalStored += inv->quantity;
                    Event_create("Got stuff!", "You have collected resources.");
                    collected = 1;
                }
            }
        }
        if (!collected) {
            Event_create("Didn't got stuff!", "No room to collect resources.");
        }
        collected = 0;
    }
}

void _retrieveMineEnable(UI_Element *el) {
    if (UI_getScreen() == SCREEN_SYSTEM && currentPlanetInfo && currentPlanetInfo->unitnum > 0) {
        el->visible = 1;
    } else {
        el->visible = 0;
    }
}

void _clickRetrieveMine(UI_Element *el, Vector2 mpos) {
    chooseShipForRetrieval = 1;
}

void _deployedShipSelectEnable(UI_Element *el) {
    if (chooseShipForRetrieval) {
        el->visible = 1;
    } else {
        el->visible = 0;
    }
}

void _drawDeployedShipSelect(UI_Element *el) {
    UI_drawPanel(el);
    for (int i = 0; i < currentPlanetInfo->unitnum; i++) {
        Unit_Entity* u = Unit_getPointer(currentPlanetInfo->units[i]);
        UI_drawSelectListItem(el, i, 16, u->name, chooseShipForRetrieval - 2 == i);
    }
}

void _clickDeployedShipSelect(UI_Element *el, Vector2 mpos) {
    chooseShipForRetrieval = UI_handleSelectList(el, mpos, currentPlanetInfo->unitnum, 16) + 2;
}

void _clickConfirmDeployedShipSelect(UI_Element *el) {
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    int uid = currentPlanetInfo->units[chooseShipForRetrieval - 2];
    Fleet_addUnit(f, uid);
    Sector_removeUnitFromPlanetByIndex(currentPlanetInfo, chooseShipForRetrieval - 2);
    chooseShipForRetrieval = 0;
}

int _calcRingChange(Sector_Planet* p) {
    return (p->size / 20 > 15) ? p->size / 20 + 5: 15;
}

void _drawSystem(UI_Element* el) {
    int starPosX = el->x;
    int starPosY = el->y + el->height / 2;
    switch (ScreenManager_currentSector()->star) {
        case STAR_BLUE:
            DrawCircle(starPosX, starPosY, 80, BLUE);
            break;
        case STAR_RED:
            DrawCircle(starPosX, starPosY, 80, RED);
            break;
        case STAR_WHITE:
            DrawCircle(starPosX, starPosY, 80, RAYWHITE);
            break;
        case STAR_YELLOW:
            DrawCircle(starPosX, starPosY, 80, YELLOW);
            break;
        default:
            break;
    }
    int ring = 80;
    Sector_Planet* p = 0;
    for (int i = 0; i < ScreenManager_currentSector()->planetnum; i++) {
        p = &ScreenManager_currentSector()->planets[i];
        ring += _calcRingChange(p);
        DrawCircleLines(starPosX, starPosY, ring, DARKGRAY);
        int psize = floor(p->size / 20);
        int hpsize = floor(p->size / 10);
        switch (p->type) {
            case PLANET_GAIA:
            case PLANET_TERRAN:
                DrawCircle(starPosX + ring, starPosY, psize, GREEN);
                break;
            case PLANET_OCEAN:
                DrawCircle(starPosX + ring, starPosY, psize, BLUE);
                break;
            case PLANET_BARREN:
                DrawCircle(starPosX + ring, starPosY, psize, GRAY);
                break;
            case PLANET_TUNDRA:
                DrawCircle(starPosX + ring, starPosY, psize, WHITE);
                break;
            case PLANET_DESERT:
                DrawCircle(starPosX + ring, starPosY, psize, BROWN);
                break;
            case PLANET_GAS_GIANT:
                DrawCircle(starPosX + ring, starPosY, psize, VIOLET);
                break;
            case PLANET_VOLCANIC:
                DrawCircle(starPosX + ring, starPosY, psize, ORANGE);
                break;
            default:
                break;
        }
        // Show units deployed here
        for (int i = 0; i < p->unitnum; i++) {
            DrawText("X", starPosX + ring + i*8, starPosY, 8, BLACK);
        }
        // Highlight selected planet
        if (currentPlanetInfo && currentPlanetInfo == p) {
            DrawRectangleLines(starPosX + ring - psize, starPosY - psize, hpsize, hpsize, RAYWHITE);
            // Draw a panel of info
            DrawText(TextFormat(
                "%s World\nPopulation: %dM\nTemperature: %s",
                Sector_planetStrings[p->type], p->pop, Sector_tempStrings[p->temperature], p->temperature
            ), 500, 0, 16, RAYWHITE);
            Unit_Entity* u = 0;
            for (int i = 0; i < p->resourcenum; i++) {
                int isBeingMined = 0;
                if (p->resources[i].type != RES_NONE) {
                    for (int j = 0; j < p->unitnum; j++) {
                        u = Unit_getPointer(p->units[j]);
                        if (u->resourceMining.type == p->resources[i].type) {
                            isBeingMined = 1;
                            break;
                        }
                    }
                    if (isBeingMined) {
                        DrawText(TextFormat("%s: %d%% (being mined)", Sector_resourceStrings[p->resources[i].type], p->resources[i].abundance), 500, 100 + i * 24, 16, RAYWHITE);
                    } else {
                        DrawText(TextFormat("%s: %d%%", Sector_resourceStrings[p->resources[i].type], p->resources[i].abundance), 500, 100 + i * 24, 16, RAYWHITE);
                    }
                }
            }
        }
        ring += _calcRingChange(p);
    }
}

void _clickSystem(UI_Element* el, Vector2 mpos) {
    int relx = mpos.x - el->x;
    //int rely = mpos.y - el->y;
    int leftRing = 80;
    int rightRing = 80;
    Sector_Planet* p = 0;
    for (int i = 0; i < ScreenManager_currentSector()->planetnum; i++) {
        p = &ScreenManager_currentSector()->planets[i];
        rightRing += _calcRingChange(p) * 2;
        if (relx <= rightRing && relx >= leftRing) {
            currentPlanetInfo = p;
            return;
        }
        leftRing = rightRing;
    }
}


void ScreenSystem_init() {
    UI_createElement(350, 350, 100, 32, "Done", SCREEN_SYSTEM, _miningShipSelectEnable, UI_drawButton, _clickConfirmMiningShipSelect, NOFUNC);
    UI_createElement(100, 200, 400, 200, "Mining ship select", SCREEN_SYSTEM, _miningShipSelectEnable, _drawMiningShipSelect, _clickMiningShipSelection, NOFUNC);
    UI_createElement(350, 350, 100, 32, "Done", SCREEN_SYSTEM, _resourceSelectEnable, UI_drawButton, _clickConfirmResourceSelection, NOFUNC);
    UI_createElement(100, 200, 400, 200, "Resource select", SCREEN_SYSTEM, _resourceSelectEnable, _drawResourceSelect, _clickResourceSelect, NOFUNC);
    UI_createElement(500, 400, 200, 32, "Deploy mining ship", SCREEN_SYSTEM, _deployMineEnable, UI_drawButton, _clickMineEnable, NOFUNC);
    
    UI_createElement(500, 432, 200, 32, "Collect resources", SCREEN_SYSTEM, _collectResourceEnable, UI_drawButton, _clickCollectResource, NOFUNC);
    
    UI_createElement(350, 350, 100, 32, "Done", SCREEN_SYSTEM, _deployedShipSelectEnable, UI_drawButton, _clickConfirmDeployedShipSelect, NOFUNC);
    UI_createElement(100, 200, 400, 200, "Deployed ship select", SCREEN_SYSTEM, _deployedShipSelectEnable, _drawDeployedShipSelect, _clickDeployedShipSelect, NOFUNC);
    UI_createElement(500, 464, 200, 32, "Retrieve ship", SCREEN_SYSTEM, _retrieveMineEnable, UI_drawButton, _clickRetrieveMine, NOFUNC);
    
    UI_createElement(0, 40, SCREENX, SCREENY, "System display", SCREEN_SYSTEM, NOFUNC, _drawSystem, _clickSystem, NOFUNC);
}