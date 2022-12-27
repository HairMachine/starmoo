#include <math.h>

#include "fleet.h"
#include "unit.h"
#include "ui.h"
#include "screen_manager.h"

int chooseResourcesForMining = 0;
Sector_Planet* currentPlanetInfo = 0;

void _resourceSelectEnable(UI_Element* el) {
    el->enabled = chooseResourcesForMining;
}

void _clickConfirmResourceSelection(UI_Element* el, Vector2 mpos) {
    if (chooseResourcesForMining < 2) {
        chooseResourcesForMining = 0;
        return;
    }
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    Unit_Entity* u = 0;
    for (int i = 0; i < f->unitmax; i++) {
        u = Unit_getPointer(f->units[i]);
        if (u->mining > 0) {
            Sector_deployUnitToPlanet(currentPlanetInfo, f->units[i]);
            u->resourceMining = currentPlanetInfo->resources[chooseResourcesForMining - 2].type;
            Fleet_removeUnit(f, i);
        }
    }
    chooseResourcesForMining = 0;
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
        el->enabled = 0;
        return;
    }
    if (currentPlanetInfo->resourcenum > 0 && Fleet_canMine(Fleet_getPointer(ScreenManager_currentSector()->fleet))) {
        el->enabled = 1;
    } else {
        el->enabled = 0;
    }
}

void _clickMineEnable(UI_Element *el, Vector2 mpos) {
    // TODO: Be able to choose a mining ship.
    chooseResourcesForMining = 1;
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
            for (int i = 0; i < p->resourcenum; i++) {
                if (p->resources[i].type != RES_NONE) {
                    DrawText(TextFormat("%s: %d%%", Sector_resourceStrings[p->resources[i].type], p->resources[i].abundance), 500, 100 + i * 24, 16, RAYWHITE);
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
    UI_createElement(350, 350, 100, 32, "Done", SCREEN_ALL, _resourceSelectEnable, UI_drawButton, _clickConfirmResourceSelection, NOFUNC);
    UI_createElement(100, 200, 400, 200, "Resource select", SCREEN_SYSTEM, _resourceSelectEnable, _drawResourceSelect, _clickResourceSelect, NOFUNC);
    UI_createElement(500, 400, 200, 32, "Deploy mining ship", SCREEN_SYSTEM, _deployMineEnable, UI_drawButton, _clickMineEnable, NOFUNC);
    UI_createElement(0, 40, SCREENX, SCREENY, "System display", SCREEN_SYSTEM, NOFUNC, _drawSystem, _clickSystem, NOFUNC);
}