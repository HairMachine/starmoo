#include <string.h>
#include <math.h>

#include "unit.h"
#include "fleet.h"
#include "ui.h"
#include "screen_manager.h"

void _drawBuildShipMenu(UI_Element* el) {
    for (int i = 0; i < Unit_designCount(); i++) {
        Unit_Design d = Unit_getDesignCopy(i);
        int num = Fleet_getUnitCountByDesign(Fleet_getPointer(ScreenManager_currentSector()->fleet), i);
        int cost = Unit_designProductionCost(&d);
        DrawRectangleLines(el->x, el->y + i * 32, el->width, 32, RAYWHITE);
        if (strcmp(d.name, "")) {
            DrawText(TextFormat("Build %s (%d owned) - %d production", d.name, num, cost), el->x, el->y + i * 32, 16, RAYWHITE);
        } else {
            DrawText("+ New design", el->x, el->y + i *32, 16, RAYWHITE);
        }
    }
}

void _clickBuildShipMenu(UI_Element* el, Vector2 mpos) {
    int designToBuild = floor((mpos.y - el->y) / 32);
    Unit_create(Unit_getDesignPointer(designToBuild));
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    Fleet_addUnit(f, Unit_count() - 1);
}

void ScreenFleet_init() {
    UI_createElement(0, 64, 400, SCREENY, "Build ship display", SCREEN_BUILD_MILITARY, NOFUNC, _drawBuildShipMenu, _clickBuildShipMenu, NOFUNC);
}