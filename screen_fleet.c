#include <string.h>
#include <math.h>

#include "event.h"
#include "order.h"
#include "unit.h"
#include "research.h"
#include "fleet.h"
#include "ui.h"
#include "screen_manager.h"

int buildShip = 0;
int selectedShip = -1;
int newDesign = 0;
Unit_Component newDesignComponents[UNIT_COMPONENT_MAX] = {};
int newDesignComponentNum = 0;
int newDesignBuildCosts[RES_ALL];
int upgradeShip = 0;

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
    int designSize = 0;
    for (int i = 0; i < d->componentnum; i++) {
        designSize += d->components[i].size;
        if (FLEET_BASIC_NEEDS - Fleet_getAssignedPop(f) - d->componentnum < 0) {
            Event_create("No workers", "Not enough people to work this ship.\nBuild farms!");
            return;
        }
        if (designSize > f->largestAllowedShip) {
            Event_create("Ship too big", "This ship is too large to fit into your available\nship bays.");
            return;
        }
        for (int j = 0; j < 4; j++) {
            if (!Fleet_hasEnoughItems(f, d->components[i].buildCosts[j])) {
                Event_create("Could not upgrade", "Not enough stuff to build ship.");
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
        if (Research_techIsDeveloped(uc.techRequired) && (!uc.obsoletedBy || !Research_techIsDeveloped(uc.obsoletedBy))) {
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
        if (Research_techIsDeveloped(uc.techRequired) && (!uc.obsoletedBy || !Research_techIsDeveloped(uc.obsoletedBy))) {
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
    ud->componentnum = newDesignComponentNum;
    ud->playerCanBuild = 1;
    for (int i = 0; i < newDesignComponentNum; i++) {
        ud->components[i] = newDesignComponents[i];
        if (strlen(ud->name) < 29) {
            strcpy(ud->name, TextFormat(
                "%s %c%c%c-%c",
                ud->name,
                ud->components[i].name[0],
                ud->components[i].name[1],
                ud->components[i].name[2],
                ud->components[i].name[strlen(ud->components[i].name) - 1]
            ));
        }
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

void _clickUpgradeButton(UI_Element* el, Vector2 mpos) {
    upgradeShip = 1;
}

void _enableUpgradeMenu(UI_Element* el) {
    el->visible = upgradeShip;
}

void _drawUpgradeMenu(UI_Element* el) {
    UI_drawPanel(el);
    int line = 0;
    for (int i = 0; i < MAXUPGRADES; i++) {
        Fleet_Upgrade upgrade = Fleet_getUpgrade(i);
        if (Research_techIsDeveloped(upgrade.techRequired) && !Research_techIsDeveloped(upgrade.obsoletedBy) && !upgrade.complete) {
            DrawText(TextFormat("%s %d", upgrade.name, upgrade.complete), el->x, el->y + line * 32, 16, RAYWHITE);
            // Show cost of upgrade
            int col = 0;
            for (int j = 0; j <  4; j++) {
                if (upgrade.buildCosts[j].type) {
                    DrawText(
                        TextFormat("%s: %d", Sector_resourceStrings[upgrade.buildCosts[j].type], upgrade.buildCosts[j].abundance),
                        el->x + col, el->y + line * 32  + 16, 16, RAYWHITE 
                    );
                    int slen = strlen(Sector_resourceStrings[i]);
                    int log = (int) log10(newDesignBuildCosts[i]);
                    line += (slen + log + 4) * 8;
                }
            }
            line++;
        }
    }
    if (line == 0) {
        DrawText("No upgrades available", el->x, el->y, 16, RAYWHITE);
    }
}

void _clickUpgradeMenu(UI_Element* el, Vector2 mpos) {
    int clicked = floor((mpos.y - el->y) / 32);
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    for (int i = 0; i < MAXUPGRADES; i++) {
        Fleet_Upgrade upgrade = Fleet_getUpgrade(i);
        if (Research_techIsDeveloped(upgrade.techRequired) && !Research_techIsDeveloped(upgrade.obsoletedBy) && !upgrade.complete) {
            clicked--;
            if (clicked == -1) {
                for (int j = 0; j < 4; j++) {
                    if (!Fleet_hasEnoughItems(f, upgrade.buildCosts[j])) {
                        Event_create("Could not build", "Not enough stuff to upgrade ship.");
                        return;
                    }
                }
                Order_Entity* o = Order_create();
                o->type = ORDER_UPGRADE;
                o->fleet = ScreenManager_currentSector()->fleet;
                o->param1 = i;
                for (int j = 0; j < 4; j++) {
                    Fleet_consumeItems(f, upgrade.buildCosts[j]);
                    o->param2 += upgrade.buildCosts[j].abundance * 100;
                }
                upgradeShip = 0;
                return;
            }
        }
    }
}

void _clickUpgradeDoneButton(UI_Element* el, Vector2 mpos) {
    upgradeShip = 0;
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
    // Show orders queue
    int line = 0;
    Order_Entity* o = 0;
    Fleet_Upgrade upgrade = (Fleet_Upgrade) {};
    for (int i = 0; i < Order_count(); i++) {
        o = Order_getPointer(i);
        if ((o->type == ORDER_BUILD_SHIP || o->type == ORDER_UPGRADE) && o->fleet == ScreenManager_currentSector()->fleet) {
            if (o->type == ORDER_BUILD_SHIP) {
                u = Unit_getPointer(o->param1);
                DrawText(TextFormat("%s (%d)", u->name, u->costToBuild), el->x + 300, el->y + 32 + 16*line, 16, RAYWHITE);
            } else {
                upgrade = Fleet_getUpgrade(o->param1);
                DrawText(TextFormat("%s (%d)", upgrade.name, o->param2), el->x + 300, el->y + 32 + 16*line, 16, RAYWHITE);
            }
            line++;
        }
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

    UI_createElement(364, 464, 100, 32, "Done", SCREEN_FLEET, _enableUpgradeMenu, UI_drawButton, _clickUpgradeDoneButton, NOFUNC);
    UI_createElement(100, 100, 600, 400, "Upgrade display", SCREEN_FLEET, _enableUpgradeMenu, _drawUpgradeMenu, _clickUpgradeMenu, NOFUNC);

    UI_createElement(500, 100, 132, 32, "Upgrade mothership", SCREEN_FLEET, NOFUNC, UI_drawButton, _clickUpgradeButton, NOFUNC);
    UI_createElement(500, 132, 132, 32, "Build ship", SCREEN_FLEET, NOFUNC, UI_drawButton, _clickBuildShipButton, NOFUNC);
    
    UI_createElement(0, 64, 400, SCREENY, "Fleet screen", SCREEN_FLEET, NOFUNC, _drawFleetScreen, _clickFleetScreen, NOFUNC);
}