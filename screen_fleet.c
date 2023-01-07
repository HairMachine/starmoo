#include <string.h>
#include <math.h>

#include "event.h"
#include "order.h"
#include "unit.h"
#include "fleet.h"
#include "ui.h"
#include "screen_manager.h"

int buildShip = 0;
int selectedShip = -1;
int newDesign = 0;
Unit_Component newDesignComponents[UNIT_COMPONENT_MAX] = {};
int newDesignComponentNum = 0;
int newDesignBuildCosts[RES_ALL];

void _enableBuildShipMenu(UI_Element *el) {
    el->visible = buildShip;
}

void _drawBuildShipMenu(UI_Element* el) {
    UI_drawPanel(el);
    int line = 0;
    for (int i = 0; i < DESIGN_MAX; i++) {
        if (i < Unit_designCount()) {
            Unit_Design d = Unit_getDesignCopy(i);
            if (d.playerCanBuild) {
                int num = Fleet_getUnitCountByDesign(Fleet_getPointer(ScreenManager_currentSector()->fleet), i);
                int cost = Unit_designProductionCost(&d);
                if (strcmp(d.name, "")) {
                    DrawText(TextFormat("Build %s (%d owned) - %d production", d.name, num, cost), el->x, el->y + line*32, 16, RAYWHITE);
                }
                line++;
            }
        } else {
            DrawText("+ New design", el->x, el->y + line*32, 16, RAYWHITE);
            break;
        }
    }
    // Show orders queue
    line = 0;
    Order_Entity* o = 0;
    Unit_Entity* u = 0;
    for (int i = 0; i < Order_count(); i++) {
        o = Order_getPointer(i);
        if (o->type == ORDER_BUILD_SHIP && o->param2 == selectedShip) {
            u = Unit_getPointer(o->param1);
            DrawText(TextFormat("%s (%d)", u->name, u->costToBuild), el->x + 400, el->y + 32 + 16*line, 16, RAYWHITE);
            line++;
        }
    }
}

void _clickBuildShipMenu(UI_Element* el, Vector2 mpos) {
    int designToBuild = 0;
    int clicked = floor((mpos.y - el->y) / 32);
    for (int i = 0; i < DESIGN_MAX; i++) {
        Unit_Design* d = Unit_getDesignPointer(i);
        if (d->playerCanBuild) {
            clicked--;
        }
        if (clicked == -1) {
            designToBuild = i;
            break;
        }
        if (i >= Unit_designCount()) {
            newDesign = 1;
            return;
        }
    }
    Unit_Design* d = Unit_getDesignPointer(designToBuild);
    // Check resources are available
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    int allowedSize = f->maxSize - f->size;
    int designSize = 0;
    for (int i = 0; i < d->componentnum; i++) {
        designSize += d->components[i].size;
        if (FLEET_BASIC_NEEDS - Fleet_getAssignedPop(f) - d->componentnum < 0) {
            Event_create("No workers", "Not enough people to work this ship.\nBuild farms!");
            return;
        }
        if (designSize > allowedSize) {
            Event_create("No space", "Not enough ship bay space available\nto build this ship.");
            return;
        }
        if (designSize > f->largestAllowedShip) {
            Event_create("Ship too big", "This ship is too large to fit into your available\nship bays.");
            return;
        }
        for (int j = 0; j < 4; j++) {
            if (!Fleet_hasEnoughItems(f, d->components[i].buildCosts[j])) {
                Event_create("Could not build", "Not enough stuff to build ship.");
                return;
            }
        }
        for (int j = 0; j < 4; j++) {
            Fleet_consumeItems(f, d->components[i].buildCosts[j]);
        }
    }
    Unit_create(Unit_getDesignPointer(designToBuild));
    Order_Entity* o = Order_create();
    o->type = ORDER_BUILD_SHIP;
    o->fleet = ScreenManager_currentSector()->fleet;
    o->param1 = Unit_lastAddedIndex();
    o->param2 = selectedShip;
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

void _enableDesignShip(UI_Element* el) {
    el->visible = newDesign;
}

void _drawDesignShip(UI_Element* el) {
    UI_drawPanel(el);
    DrawText("Add components", el->x, el->y, 32, RAYWHITE);
    int line = 0;
    for (int i = 0; i < COMPONENTS_ALL; i++) {
        Unit_Component uc = Unit_getComponent(i);
        if (Research_techIsDeveloped(uc.techRequired)) {
            DrawText(uc.name, el->x, 32 + el->y + line*16, 16, RAYWHITE);
            line++;
        }
    }
    int secondWindow = el->x + el->width / 2;
    DrawText("New Ship", secondWindow, el->y, 32, RAYWHITE);
    for (int i = 0; i < newDesignComponentNum; i++) {
        DrawText(newDesignComponents[i].name, secondWindow, 32 + el->y + i*16, 16, RAYWHITE);
    }
    // Show cost of ship
    line = 0;
    for (int i = 0; i < RES_ALL; i++) {
        if (newDesignBuildCosts[i]) {
            DrawText(
                TextFormat("%s: %d", Sector_resourceStrings[i], newDesignBuildCosts[i]),
                el->x + line, el->y + el->height, 16, RAYWHITE 
            );
            int slen = strlen(Sector_resourceStrings[i]);
            int log = (int) log10(newDesignBuildCosts[i]);
            line += (slen + log + 4) * 8;
        }
    }
}

void _clickDesignShip(UI_Element* el, Vector2 mpos) {
    if (mpos.x - el->x > el->width / 2) {
        return;
    }
    int clicked = floor((mpos.y - (el->y + 32)) / 16);
    int build = 0;
    for (int i = 0; i < COMPONENTS_ALL; i++) {
        Unit_Component uc = Unit_getComponent(i);
        if (Research_techIsDeveloped(uc.techRequired)) {
            clicked--;
            if (clicked == -1) {
                build = i;
            }
        }
    }
    Unit_Component c = Unit_getComponent(build);
    newDesignComponents[newDesignComponentNum] = c;
    newDesignComponentNum++;
    // Calculate ship costs
    for (int i = 0; i < RES_ALL; i++) {
        newDesignBuildCosts[i] = 0;
    }
    for (int i = 0; i < newDesignComponentNum; i++) {
        for (int j = 0; j < 4; j++) {
            Sector_Resource r = newDesignComponents[i].buildCosts[j];
            if (r.abundance) {
                newDesignBuildCosts[r.type] += r.abundance;
            }
        }
    }
}

void _clickDesignShipBuildButton(UI_Element* el, Vector2 mpos) {
    Unit_Design* ud = Unit_createDesign();
    strcpy(ud->name, TextFormat("User Ship %d", Unit_designCount()));
    ud->componentnum = newDesignComponentNum;
    ud->playerCanBuild = 1;
    for (int i = 0; i < newDesignComponentNum; i++) {
        ud->components[i] = newDesignComponents[i];
    }
    newDesign = 0;
}

void _clickDesignShipClearButton(UI_Element* el, Vector2 mpos) {
    newDesignComponentNum = 0;
    for (int i = 0; i < RES_ALL; i++) {
        newDesignBuildCosts[i] = 0;
    }
}

void _clickDesignShipCancelButton(UI_Element* el, Vector2 mpos) {
    newDesign = 0;
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

void ScreenFleet_init() {
    UI_createElement(364, 464, 100, 32, "Build", SCREEN_FLEET, _enableDesignShip, UI_drawButton, _clickDesignShipBuildButton, NOFUNC);
    UI_createElement(464, 464, 100, 32, "Clear", SCREEN_FLEET, _enableDesignShip, UI_drawButton, _clickDesignShipClearButton, NOFUNC);
    UI_createElement(564, 464, 100, 32, "Cancel", SCREEN_FLEET, _enableDesignShip, UI_drawButton, _clickDesignShipCancelButton, NOFUNC);
    UI_createElement(100, 100, 600, 400, "Design ship display", SCREEN_FLEET, _enableDesignShip, _drawDesignShip, _clickDesignShip, NOFUNC);

    UI_createElement(364, 464, 100, 32, "Done", SCREEN_FLEET, _enableBuildShipMenu, UI_drawButton, _clickBuildShipDoneButton, NOFUNC);
    UI_createElement(100, 100, 600, 400, "Build ship display", SCREEN_FLEET, _enableBuildShipMenu, _drawBuildShipMenu, _clickBuildShipMenu, NOFUNC);

    UI_createElement(500, 100, 132, 32, "Build ship", SCREEN_FLEET, _enableBuildShipButton, UI_drawButton, _clickBuildShipButton, NOFUNC);
    UI_createElement(0, 64, 400, SCREENY, "Fleet screen", SCREEN_FLEET, NOFUNC, _drawFleetScreen, _clickFleetScreen, NOFUNC);
}