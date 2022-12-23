#include <math.h>
#include <string.h>
#include "raylib.h"

#include "event.h"
#include "order.h"
#include "unit.h"
#include "fleet.h"
#include "world.h"
#include "ui.h"

UI_Element nullElement = {0, 0, 0, 0, 0, 0, 0};
UI_Element elements[255];
int elementEnd = 0;

void UI_createElement(int x, int y, int width, int height, char* btnText, UI_Screen screen, void (*enableFunc), void (*drawFunc), void (*clickFunc), void (*targeterFunc)) {
    UI_Element* el = &elements[elementEnd];
    el->x = x;
    el->y = y;
    el->width = width;
    el->height = height;
   strcpy(el->btnText, btnText);
    el->screen = screen;
    el->enableFunc = enableFunc; 
    el->drawFunc = drawFunc;
    el->clickFunc = clickFunc;
    el->targeterFunc = targeterFunc;
    el->enabled = 1;
    elementEnd++;
}

UI_Element UI_getElementByPosition(int x, int y) {
    for (int e = 0; e < elementEnd; e++) {
        UI_Element* el = &elements[e];
        if (!el->enabled) {
            continue;
        }
        if (x >= el->x && y >= el->y && x <= el->x + el->width && y <= el->y + el->height) {
            return elements[e];
        }
    }
    return nullElement;
}

UI_Screen currentScreen;

void UI_isCurrentScreenEnable(UI_Element* el) {
    if (currentScreen == el->screen || el->screen == SCREEN_ALL) {
        el->enabled = 1;
    } else {
        el->enabled = 0;
    }
}

void UI_enableScreen(UI_Screen screen) {
    currentScreen = screen;
    for (int e = 0; e < elementEnd; e++) {
        UI_isCurrentScreenEnable(&elements[e]);
    }
}

void UI_updateEnabled() {
    for (int e = 0; e < elementEnd; e++) {
        if (elements[e].enableFunc) {
            elements[e].enableFunc(&elements[e]);
        }
    }
}

UI_Targeter targeter = {0, 0, 0, 0};

void UI_createTargeter(UI_Element* element, int x, int y, int range) {
    targeter.x = x;
    targeter.y = y;
    // For some reason, if we don't dereference this pointer it often points to other stuff when we get back to the targeter.
    // My guess is we have a pointer to a pointer going on somewhere.
    targeter.triggeredBy = *element;
    targeter.range = range;
}

void UI_handleTargeter(Sector_Entity* clickedSector) {
    targeter.range = 0;
    if (targeter.triggeredBy.targeterFunc) {
        targeter.triggeredBy.targeterFunc(&targeter.triggeredBy, clickedSector);
    }
}

Sector_Entity* currentSectorInfo = 0;
int lastFleet = 0;
int showEventPanel = 0;
Sector_Planet* currentPlanetInfo = 0;

// Click and draw handler functions

void UI_drawButton(UI_Element* el) {
    DrawRectangle(el->x, el->y, el->width, el->height, DARKBLUE);
    DrawRectangleLines(el->x, el->y, el->width, el->height, RAYWHITE);
    DrawText(el->btnText, el->x + 5, el->y + 5, 16, RAYWHITE);
}

void UI_fleetSelectedEnable(UI_Element* el) {
    if (currentScreen == SCREEN_MAP && currentSectorInfo && currentSectorInfo->fleet > -1) {
        el->enabled = 1;
    } else {
        el->enabled = 0;
    }
}

void UI_clickNextEvent(UI_Element* el, Vector2 mpos) {
    Event_remove(0);
}

void UI_clickSetDestination(UI_Element *el, Vector2 mpos) {
    // Get the max range from the fleet's supply
    int range = Fleet_getPointer(currentSectorInfo->fleet)->fuel;
    UI_createTargeter(el, currentSectorInfo->x, currentSectorInfo->y, range);
}

void UI_targeterSetDestination(UI_Element* el, Sector_Entity* clickedSector) {
    if (clickedSector->star == STAR_NONE) {
        return;
    }
    Fleet_Entity* f = Fleet_getPointer(currentSectorInfo->fleet);
    if (f->warpFactor == 0) {
        return;
    }
    Order_Entity* o = 0;
    // If this fleet already has an order, update it instead of making a new one
    if (f->orders > -1) {
        o = Order_getPointer(f->orders);
    } else {
        o = Order_create();
        f->orders = Order_count() - 1;
    }
    o->originX = currentSectorInfo->x;
    o->originY = currentSectorInfo->y;
    o->targetX = clickedSector->x;
    o->targetY = clickedSector->y;
    o->type = ORDER_MOVE_FLEET;
    o->fleet = currentSectorInfo->fleet;
    currentSectorInfo = clickedSector;
}

void UI_clickBuildShips(UI_Element *el, Vector2 mpos) {
    UI_enableScreen(SCREEN_BUILD_MILITARY);
}

void UI_clickGalaxyTab(UI_Element *el, Vector2 mpos) {
    UI_enableScreen(SCREEN_MAP);
}

void UI_clickSystemTab(UI_Element *el, Vector2 mpos) {
    if (currentSectorInfo->fleet > -1) {
        UI_enableScreen(SCREEN_SYSTEM);
    }
}

void UI_clickFleetTab(UI_Element *el, Vector2 mpos) {
    UI_enableScreen(SCREEN_FLEET);
}

void UI_endTurnEnable(UI_Element *el, Vector2 mpos) {
    if (Event_count() == 0) {
        el->enabled = 1;
    } else {
        el->enabled = 0;
    }
}

void UI_clickEndTurn(UI_Element *el, Vector2 mpos) {
    World_update();
}

void UI_drawMap(UI_Element* el) {
    int circleRad = UI_tileSize / 2;
    for (int x = 0; x < World_sizeX; x++) {
        for (int y = 0; y < World_sizeY; y++) {
            Sector_Entity* s = World_getSectorPointer(x, y);
            switch (s->star) {
                case STAR_RED:
                    DrawCircle(el->x + x*UI_tileSize + circleRad, el->y + y*UI_tileSize + circleRad, circleRad - 4, MAROON);
                    break;
                case STAR_YELLOW:
                    DrawCircle(el->x + x*UI_tileSize + circleRad, el->y + y*UI_tileSize + circleRad, circleRad - 4, ORANGE);
                    break;
                case STAR_BLUE:
                    DrawCircle(el->x + x*UI_tileSize + circleRad, el->y + y*UI_tileSize + circleRad, circleRad - 4, DARKBLUE);
                    break;
                case STAR_WHITE:
                    DrawCircle(el->x + x*UI_tileSize + circleRad, el->y + y*UI_tileSize + circleRad, circleRad - 4, GRAY);
                    break;
                default:
                    break;
            }
            if (currentSectorInfo == s) {
                DrawRectangleLines(el->x + x*UI_tileSize - 1, el->y + y*UI_tileSize - 1, UI_tileSize + 2, UI_tileSize + 2, WHITE);
            }
            if (s->pop > 0 && s->star == STAR_NONE) {
                s->pop++;
            }
            if (s->pop > 0 && s->explored == 1) {
                DrawText("C", el->x + x * UI_tileSize + 1, el->y + y * UI_tileSize + 1, 4, BLACK);
                DrawText("C", el->x + x * UI_tileSize, el->y + y * UI_tileSize, 4, WHITE);
            } else if (s->explored) {
                DrawText("E", el->x + x * UI_tileSize + 1, el->y + y * UI_tileSize + 1, 4, BLACK);
                DrawText("E", el->x + x * UI_tileSize, el->y + y * UI_tileSize, 4, WHITE);
            }
            if (s->fleet > -1) {
                DrawText("F", el->x + x * UI_tileSize + UI_tileSize - 4, el->y + y * UI_tileSize + 1, 4, BLACK);
                DrawText("F", el->x + x * UI_tileSize + UI_tileSize - 5, el->y + y * UI_tileSize, 4, WHITE);
            }
        }
    }
    // Show targeter
    if (targeter.range > 0) {
        for (int x = targeter.x - targeter.range; x <= targeter.x + targeter.range; x++) {
            for (int y = targeter.y - targeter.range; y <= targeter.y + targeter.range; y++) {
                if (x >= 0 && y >= 0 && x < World_sizeX - 1 && y < World_sizeY - 1) {
                    DrawRectangleLines(el->x + x*UI_tileSize - 1, el->y + y*UI_tileSize - 1, UI_tileSize + 2, UI_tileSize + 2, GREEN);
                }
            }
        }
    }
    // Show orders (where applicable)
    for (int i = 0; i < Order_count(); i++) {
        Order_Entity o = Order_getCopy(i);
        switch (o.type) {
            case ORDER_MOVE_FLEET:
                DrawLine(
                    el->x + circleRad + o.originX * UI_tileSize,
                    el->y + circleRad + o.originY * UI_tileSize,
                    el->x + circleRad + o.targetX * UI_tileSize,
                    el->y + circleRad + o.targetY * UI_tileSize,
                    GREEN
                );
                break;
            default:
                break;
        }
    }
}

void UI_clickMap(UI_Element* el, Vector2 mpos) {
    int xtil = floor(((int)mpos.x - el->x) / UI_tileSize);
    int ytil = floor(((int)mpos.y - el->y) / UI_tileSize);
    Sector_Entity* se = World_getSectorPointer(xtil, ytil);
    if (targeter.range == 0) {
        if (se->star > STAR_NONE) {
            currentSectorInfo = se;
            currentSectorInfo->x = xtil;
            currentSectorInfo->y = ytil;
            if (currentSectorInfo->fleet > -1) {
                lastFleet = currentSectorInfo->fleet;
            }
        }
    } else {
        UI_handleTargeter(se);
    }
}

void UI_drawEventPanel(UI_Element* el) {
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

void UI_eventEnable(UI_Element* el) {
    if (Event_count() == 0) {
        el->enabled = 0;
    } else {
        el->enabled = 1;
    }
}

void UI_drawSectorInfo(UI_Element* el) {
    if (!currentSectorInfo) {
        return;
    }
    if (currentSectorInfo->explored) {
        DrawText(TextFormat("Population: %iM", currentSectorInfo->pop), el->x, el->y, 16, RAYWHITE);
        int line = 2;
        for (int i = 0; i < currentSectorInfo->planetnum; i++) {
            Sector_Planet p = Sector_getPlanet(currentSectorInfo, i);
            DrawText(TextFormat("- %s %dM", Sector_planetStrings[p.type], p.pop), el->x, el->y + 8*line, 8, RAYWHITE);
            line++;
        }
    } else {
        DrawText("System unexplored", el->x, el->y, 16, RAYWHITE);
    }
    if (lastFleet > -1) {
        Fleet_Entity* f = Fleet_getPointer(lastFleet);
        DrawText(
            TextFormat(
                "Total ships: %d\nPopulation: %d\nFuel: %d/%d\nFood: %d\nDrink: %d\nOxygen: %d\nStorage: %d/%d",
                f->unitmax, FLEET_BASIC_NEEDS, f->fuel, f->fuelMax, f->food, f->drink, f->oxygen, f->food+f->drink+f->oxygen, f->storageMax),
            el->x, el->y + 128, 8, RAYWHITE
        );
    }
}

void UI_drawBuildShipMenu(UI_Element* el) {
    for (int i = 0; i < Unit_designCount(); i++) {
        Unit_Design d = Unit_getDesignCopy(i);
        int num = Fleet_getUnitCountByDesign(Fleet_getPointer(currentSectorInfo->fleet), i);
        DrawRectangleLines(el->x, el->y + i * 32, el->width, 32, RAYWHITE);
        if (strcmp(d.name, "")) {
            if (d.warpDriveLevel > 0 && num > 0) {
                DrawText(TextFormat("Cannot build %s (%d already in fleet)", d.name, num), el->x, el->y + i * 32, 16, RED);
            } else {
                DrawText(TextFormat("Build %s (%d owned)", d.name, num), el->x, el->y + i * 32, 16, RAYWHITE);
            }
        } else {
            DrawText("+ New design", el->x, el->y + i *32, 16, RAYWHITE);
        }
    }
}

void UI_clickBuildShipMenu(UI_Element* el, Vector2 mpos) {
    int designToBuild = floor((mpos.y - el->y) / 32);
    Unit_create(Unit_getDesignPointer(designToBuild));
    Fleet_Entity* f = Fleet_getPointer(currentSectorInfo->fleet);
    Fleet_addUnit(f, Unit_count() - 1);
}

int _calcRingChange(Sector_Planet* p) {
    return (p->size / 20 > 15) ? p->size / 20 + 5: 15;
}

void UI_drawSystem(UI_Element* el) {
    int starPosX = el->x;
    int starPosY = el->y + el->height / 2;
    switch (currentSectorInfo->star) {
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
    for (int i = 0; i < currentSectorInfo->planetnum; i++) {
        p = &currentSectorInfo->planets[i];
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

void UI_clickSystem(UI_Element* el, Vector2 mpos) {
    int relx = mpos.x - el->x;
    //int rely = mpos.y - el->y;
    int leftRing = 80;
    int rightRing = 80;
    Sector_Planet* p = 0;
    for (int i = 0; i < currentSectorInfo->planetnum; i++) {
        p = &currentSectorInfo->planets[i];
        rightRing += _calcRingChange(p) * 2;
        if (relx <= rightRing && relx >= leftRing) {
            currentPlanetInfo = p;
            return;
        }
        leftRing = rightRing;
    }
}

void UI_initialise() {
    UI_createElement(0, 0, 100, 32, "Galaxy", SCREEN_ALL, NOFUNC, UI_drawButton, UI_clickGalaxyTab, NOFUNC);
    UI_createElement(100, 0, 100, 32, "System", SCREEN_ALL, NOFUNC, UI_drawButton, UI_clickSystemTab, NOFUNC);
    UI_createElement(200, 0, 100, 32, "Fleet", SCREEN_ALL, NOFUNC, UI_drawButton, UI_clickFleetTab, NOFUNC);
    UI_createElement(350, 350, 100, 32, "Next", SCREEN_ALL, UI_eventEnable, UI_drawButton, UI_clickNextEvent, NOFUNC);
    UI_createElement(350, 350, 100, 32, "Done", SCREEN_ALL, UI_eventEnable, UI_drawButton, UI_clickNextEvent, NOFUNC);
    UI_createElement(100, 200, 400, 200, "", SCREEN_ALL, UI_eventEnable, UI_drawEventPanel, NOFUNC, NOFUNC);
        
    UI_createElement(0, 40, World_sizeX*UI_tileSize, World_sizeY*UI_tileSize, "", SCREEN_MAP, NOFUNC, UI_drawMap, UI_clickMap, NOFUNC);
    UI_createElement(600, 40, 300, World_sizeY, "", SCREEN_MAP, NOFUNC, UI_drawSectorInfo, NOFUNC, NOFUNC);
    UI_createElement(600, 300, 132, 32, "Set Destination", SCREEN_MAP, UI_fleetSelectedEnable, UI_drawButton, UI_clickSetDestination, UI_targeterSetDestination);
    UI_createElement(600, 364, 132, 32, "Build Ships", SCREEN_MAP, UI_fleetSelectedEnable, UI_drawButton, UI_clickBuildShips, NOFUNC);
    UI_createElement(600, 500, 100, 32, "End Turn", SCREEN_MAP, UI_endTurnEnable, UI_drawButton, UI_clickEndTurn, NOFUNC);

    UI_createElement(0, 40, SCREENX, SCREENY, "", SCREEN_SYSTEM, NOFUNC, UI_drawSystem, UI_clickSystem, NOFUNC);

    UI_createElement(0, 64, 400, SCREENY, "", SCREEN_BUILD_MILITARY, NOFUNC, UI_drawBuildShipMenu, UI_clickBuildShipMenu, NOFUNC);
    UI_createElement(600, 0, 132, 32, "Done", SCREEN_BUILD_MILITARY, NOFUNC, UI_drawButton, UI_clickGalaxyTab, NOFUNC);

    UI_enableScreen(SCREEN_MAP);
    UI_updateEnabled();
}

void UI_drawElements() {
    // Draw in reverse priority order
    for (int e = elementEnd - 1; e >= 0; e--) {
        if (elements[e].enabled) {
            elements[e].drawFunc(&elements[e]);
        }
    }
}

void UI_checkSpecialEvents() {
    if (Event_count() == 0) {
        return;
    }
    Event_Entity* e = Event_getPointer(0);
    if (strcmp(e->title, "System Discovered") == 0) {
        UI_enableScreen(SCREEN_SYSTEM);
        return;
    }
}

void UI_handleMouse() {
    Vector2 mpos = GetMousePosition();
    UI_Element el = UI_getElementByPosition((int)mpos.x, (int)mpos.y);
    if (el.clickFunc) {
        el.clickFunc(&el, mpos);
    }
    UI_checkSpecialEvents();
    UI_updateEnabled();
}
