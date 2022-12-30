#include <string.h>
#include <math.h>

#include "unit.h"
#include "fleet.h"
#include "ui.h"
#include "screen_manager.h"

int buildShip = 0;
int selectedShip = -1;

void _enableBuildShipMenu(UI_Element *el) {
    el->visible = buildShip;
}

void _drawBuildShipMenu(UI_Element* el) {
    UI_drawPanel(el);
    for (int i = 0; i < Unit_designCount(); i++) {
        Unit_Design d = Unit_getDesignCopy(i);
        if (d.playerCanBuild) {
            int num = Fleet_getUnitCountByDesign(Fleet_getPointer(ScreenManager_currentSector()->fleet), i);
            int cost = Unit_designProductionCost(&d);
            DrawRectangleLines(el->x, el->y + i * 32, el->width, 32, RAYWHITE);
            if (strcmp(d.name, "")) {
                DrawText(TextFormat("Build %s (%d owned) - %d production", d.name, num, cost), el->x, el->y + i * 32, 16, RAYWHITE);
            }
        }
    }
}

void _clickBuildShipMenu(UI_Element* el, Vector2 mpos) {
    int designToBuild = floor((mpos.y - el->y) / 32);
    Unit_create(Unit_getDesignPointer(designToBuild));
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    Fleet_addUnit(f, Unit_lastAddedIndex());
}

void _drawFleetScreen(UI_Element *el) {
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    Unit_Entity* u = 0;
    for (int i = 0; i < f->unitmax; i++) {
        int storage = 100;
        u = Unit_getPointer(f->units[i]);
        if (u->storage > 0) {
            storage = (u->totalStored * 100) / u->storage;
        }
        UI_drawSelectListItem(el, i, 16, TextFormat("%s %d%%", u->name, storage), selectedShip == i);
    }
}

void _clickFleetScreen(UI_Element *el, Vector2 mpos) {
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    selectedShip = UI_handleSelectList(el, mpos, f->unitmax, 16);
    if (selectedShip >= f->unitmax) {
        selectedShip = -1;
    }
}

void _enableBuildShipButton(UI_Element *el) {
    if (selectedShip == -1) {
        el->visible = 0;
    } else {
        el->visible = 1;
        Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
        Unit_Entity* u = Unit_getPointer(f->units[selectedShip]);
        if (u->production > 0) {
            el->enabled = 1;
        } else {
            el->enabled = 0;
        }
    }
}

void _clickBuildShipButton(UI_Element *el, Vector2 mpos) {
    buildShip = 1;
}

void _clickBuildShipDoneButton(UI_Element *el, Vector2 mpos) {
    buildShip = 0;
}

void ScreenFleet_init() {
    UI_createElement(364, 464, 100, 32, "Done", SCREEN_FLEET, _enableBuildShipMenu, UI_drawButton, _clickBuildShipDoneButton, NOFUNC);
    UI_createElement(100, 100, 400, 400, "Build ship display", SCREEN_FLEET, _enableBuildShipMenu, _drawBuildShipMenu, _clickBuildShipMenu, NOFUNC);

    UI_createElement(500, 100, 132, 32, "Build ship", SCREEN_FLEET, _enableBuildShipButton, UI_drawButton, _clickBuildShipButton, NOFUNC);
    UI_createElement(0, 64, 400, SCREENY, "Fleet screen", SCREEN_FLEET, NOFUNC, _drawFleetScreen, _clickFleetScreen, NOFUNC);
}