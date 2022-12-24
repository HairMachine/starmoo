#include "event.h"
#include "ui.h"
#include "screen_map.h"
#include "screen_system.h"
#include "screen_fleet.h"
#include "world.h"
#include "sector.h"

Sector_Entity* currentSectorInfo = 0;

void _clickGalaxyTab(UI_Element *el, Vector2 mpos) {
    UI_enableScreen(SCREEN_MAP);
}

void _fleetSelectedEnable(UI_Element* el) {
    if (currentSectorInfo && currentSectorInfo->fleet > -1) {
        el->enabled = 1;
    } else {
        el->enabled = 0;
    }
}

void _clickSystemTab(UI_Element *el, Vector2 mpos) {
    if (currentSectorInfo->fleet > -1) {
        UI_enableScreen(SCREEN_SYSTEM);
    }
}

void _clickBuildShips(UI_Element *el, Vector2 mpos) {
    UI_enableScreen(SCREEN_BUILD_MILITARY);
}

void _eventEnable(UI_Element* el) {
    if (Event_count() == 0) {
        el->enabled = 0;
    } else {
        el->enabled = 1;
    }
}

void _clickNextEvent(UI_Element* el, Vector2 mpos) {
    Event_remove(0);
}

void _drawEventPanel(UI_Element* el) {
    if (Event_count() == 0) {
        return;
    }
    Event_Entity* e = 0;
    e = Event_getPointer(0);
    DrawRectangle(el->x, el->y, el->width, el->height, BLACK);
    DrawRectangleLines(el->x, el->y, el->width, el->height, RAYWHITE);
    DrawText(e->title, el->x + 32, el->y + 16, 32, RAYWHITE);
    DrawText(e->body, el->x + 32, el->y + 64, 16, RAYWHITE);
}

Sector_Entity* ScreenManager_currentSector() {
    return currentSectorInfo;
}

void ScreenManager_setCurrentSector(Sector_Entity* s) {
    currentSectorInfo = s;
}

void _endTurnEnable(UI_Element *el, Vector2 mpos) {
    if (Event_count() == 0) {
        el->enabled = 1;
    } else {
        el->enabled = 0;
    }
}

void _clickEndTurn(UI_Element *el, Vector2 mpos) {
    World_update();
}

void ScreenManager_init() {
    UI_createElement(0, 0, 100, 32, "Galaxy", SCREEN_ALL, NOFUNC, UI_drawButton, _clickGalaxyTab, NOFUNC);
    UI_createElement(100, 0, 100, 32, "System", SCREEN_ALL, _fleetSelectedEnable, UI_drawButton, _clickSystemTab, NOFUNC);
    UI_createElement(200, 0, 100, 32, "Fleet", SCREEN_ALL, _fleetSelectedEnable, UI_drawButton, _clickBuildShips, NOFUNC);
    
    UI_createElement(350, 350, 100, 32, "Next", SCREEN_ALL, _eventEnable, UI_drawButton, _clickNextEvent, NOFUNC);
    UI_createElement(350, 350, 100, 32, "Done", SCREEN_ALL, _eventEnable, UI_drawButton, _clickNextEvent, NOFUNC);
    UI_createElement(100, 200, 400, 200, "Event Panel", SCREEN_ALL, _eventEnable, _drawEventPanel, NOFUNC, NOFUNC);

    UI_createElement(600, 500, 100, 32, "End Turn", SCREEN_MAP, _endTurnEnable, UI_drawButton, _clickEndTurn, NOFUNC);

    ScreenMap_init();
    ScreenSystem_init();
    ScreenFleet_init();
    
    UI_enableScreen(SCREEN_MAP);
    UI_updateEnabled();
}

