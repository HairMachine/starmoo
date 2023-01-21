#include <stdlib.h>

#include "event.h"
#include "ui.h"
#include "world.h"
#include "sector.h"
#include "combat.h"
#include "fleet.h"
#include "screen_manager.h"

Sector_Entity* currentSectorInfo = 0;
int paused = 1;

void _galaxySelectedEnable(UI_Element* el) {
    el->enabled = !Combat_active();
}

void _clickGalaxyTab(UI_Element *el, Vector2 mpos) {
    UI_enableScreen(SCREEN_MAP);
}

void _systemSelectedEnable(UI_Element* el) {
    if (currentSectorInfo && currentSectorInfo->explored) {
        el->enabled = 1;
    } else {
        el->enabled = 0;
    }
}

void _clickSystemTab(UI_Element *el, Vector2 mpos) {
    if (currentSectorInfo) {
        UI_enableScreen(SCREEN_SYSTEM);
    }
}

void _fleetSelectedEnable(UI_Element* el) {
    if (currentSectorInfo && currentSectorInfo->fleet > -1 && !Combat_active()) {
        el->enabled = 1;
    } else {
        el->enabled = 0;
    }
}

void _clickFleetTab(UI_Element *el, Vector2 mpos) {
    UI_enableScreen(SCREEN_FLEET);
}

void _cargoSelectedEnable(UI_Element *el) {
    if (currentSectorInfo && currentSectorInfo->fleet > -1 && !Combat_active()) {
        el->enabled = 1;
    } else {
        el->enabled = 0;
    }
}

void _clickCargoTab(UI_Element *el, Vector2 mpos) {
    UI_enableScreen(SCREEN_CARGO);
}

void _clickResearchTab(UI_Element* el, Vector2 mpos) {
    UI_enableScreen(SCREEN_RESEARCH);
}

void _drawLegitimacy(UI_Element* el) {
    int lpcnt = (100*World_getLegitimacy()) / 1000;
    Color lcol = GREEN;
    if (lpcnt < 25) {
        lcol = RED;
    } else if (lpcnt < 75) {
        lcol = YELLOW;
    }
    DrawText(TextFormat("Legitimacy: %d%%", lpcnt), el->x, el->y, 16, lcol);
}

void _eventEnable(UI_Element* el) {
    if (Event_count() == 0) {
        el->visible = 0;
    } else {
        el->visible = 1;
    }
}

void _clickNextEvent(UI_Element* el, Vector2 mpos) {
    Event_remove(0);
}

void _drawEventPanel(UI_Element* el) {
    if (Event_count() == 0) {
        return;
    }
    UI_drawPanel(el);
    Event_Entity* e = 0;
    e = Event_getPointer(0);
    DrawText(e->title, el->x + 32, el->y + 16, 32, RAYWHITE);
    DrawText(e->body, el->x + 32, el->y + 64, 16, RAYWHITE);
}

void _pauseEnable(UI_Element *el) {
    el->visible = !paused;
}

void _clickPause(UI_Element *el, Vector2 mpos) {
    paused = 1;
}

void _unPauseEnable(UI_Element *el) {
    el->visible = paused;
}

void _clickUnPause(UI_Element *el, Vector2 mpos) {
    paused = 0;
}

Sector_Entity* ScreenManager_currentSector() {
    return currentSectorInfo;
}

void ScreenManager_setCurrentSector(Sector_Entity* s) {
    currentSectorInfo = s;
}

void ScreenManager_togglePause() {
    paused = !paused;
}

int ScreenManager_isPaused() {
    return paused;
}

void ScreenManager_init() {
    UI_createElement(0, 0, 100, 32, "Galaxy", SCREEN_ALL, _galaxySelectedEnable, UI_drawButton, _clickGalaxyTab, NOFUNC);
    UI_createElement(100, 0, 100, 32, "System", SCREEN_ALL, _systemSelectedEnable, UI_drawButton, _clickSystemTab, NOFUNC);
    UI_createElement(200, 0, 100, 32, "Ships", SCREEN_ALL, _fleetSelectedEnable, UI_drawButton, _clickFleetTab, NOFUNC);
    UI_createElement(300, 0, 100, 32, "Cargo", SCREEN_ALL, _cargoSelectedEnable, UI_drawButton, _clickCargoTab, NOFUNC);
    UI_createElement(400, 0, 100, 32, "Research", SCREEN_ALL, NOFUNC, UI_drawButton, _clickResearchTab, NOFUNC);
    UI_createElement(500, 0, 100, 32, "Legitimacy", SCREEN_ALL, NOFUNC, _drawLegitimacy, NOFUNC, NOFUNC);

    UI_createElement(350, 350, 100, 32, "Next", SCREEN_ALL, _eventEnable, UI_drawButton, _clickNextEvent, NOFUNC);
    UI_createElement(350, 350, 100, 32, "Done", SCREEN_ALL, _eventEnable, UI_drawButton, _clickNextEvent, NOFUNC);
    UI_createElement(100, 200, 400, 200, "Event Panel", SCREEN_ALL, _eventEnable, _drawEventPanel, NOFUNC, NOFUNC);

    UI_createElement(600, 500, 100, 32, "Pause", SCREEN_ALL, _pauseEnable, UI_drawButton, _clickPause, NOFUNC);
    UI_createElement(600, 500, 100, 32, "Unpause", SCREEN_ALL, _unPauseEnable, UI_drawButton, _clickUnPause, NOFUNC);

    ScreenMap_init();
    ScreenSystem_init();
    ScreenFleet_init();
    ScreenCargo_init();
    ScreenCombat_init();
    ScreenResearch_init();
    
    UI_enableScreen(SCREEN_MAP);
    UI_updateEnabled();
}

