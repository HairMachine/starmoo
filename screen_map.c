#include <math.h>

#include "order.h"
#include "event.h"
#include "sector.h"
#include "world.h"
#include "fleet.h"
#include "ui.h"
#include "screen_manager.h"

int lastFleet = 0;

UI_Targeter targeter = {0, 0, 0, 0};

void _createTargeter(UI_Element* element, int x, int y, int range) {
    targeter.x = x;
    targeter.y = y;
    // For some reason, if we don't dereference this pointer it often points to other stuff when we get back to the targeter.
    // My guess is we have a pointer to a pointer going on somewhere.
    targeter.triggeredBy = *element;
    targeter.range = range;
}

void _handleTargeter(Sector_Entity* clickedSector) {
    targeter.range = 0;
    if (targeter.triggeredBy.targeterFunc) {
        targeter.triggeredBy.targeterFunc(&targeter.triggeredBy, clickedSector);
    }
}

void _targeterSetDestination(UI_Element* el, Sector_Entity* clickedSector) {
    if (clickedSector->star == STAR_NONE) {
        return;
    }
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
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
    o->originX = ScreenManager_currentSector()->x;
    o->originY = ScreenManager_currentSector()->y;
    o->targetX = clickedSector->x;
    o->targetY = clickedSector->y;
    o->type = ORDER_MOVE_FLEET;
    o->fleet = ScreenManager_currentSector()->fleet;
    ScreenManager_setCurrentSector(clickedSector);
}


void _drawMap(UI_Element* el) {
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
            if (ScreenManager_currentSector() == s) {
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
            } else {
                Sector_Planet* p = 0;
                for (int i = 0; i < s->planetnum; i++) {
                    p = &s->planets[i];
                    if (p->unitnum > 0) {
                        DrawText("X", el->x + x * UI_tileSize + UI_tileSize - 4, el->y + y * UI_tileSize + 1, 4, BLACK);
                        DrawText("X", el->x + x * UI_tileSize + UI_tileSize - 5, el->y + y * UI_tileSize, 4, WHITE);
                        break;
                    }
                }
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

void _clickMap(UI_Element* el, Vector2 mpos) {
    int xtil = floor(((int)mpos.x - el->x) / UI_tileSize);
    int ytil = floor(((int)mpos.y - el->y) / UI_tileSize);
    Sector_Entity* se = World_getSectorPointer(xtil, ytil);
    if (targeter.range == 0) {
        if (se->star > STAR_NONE) {
            ScreenManager_setCurrentSector(se);
            ScreenManager_currentSector()->x = xtil;
            ScreenManager_currentSector()->y = ytil;
            if (ScreenManager_currentSector()->fleet > -1) {
                lastFleet = ScreenManager_currentSector()->fleet;
            }
        }
    } else {
        _handleTargeter(se);
    }
}

void _setDestinationAvailable(UI_Element *el) {
    if (UI_getScreen() == SCREEN_MAP && ScreenManager_currentSector() && ScreenManager_currentSector()->fleet > -1) {
        el->visible = 1;
    } else {
        el->visible = 0;
    }
}

void _clickSetDestination(UI_Element *el, Vector2 mpos) {
    // Get the max range from the fleet's supply
    int range = Fleet_getPointer(ScreenManager_currentSector()->fleet)->fuel;
    _createTargeter(el, ScreenManager_currentSector()->x, ScreenManager_currentSector()->y, range);
}

void _drawSectorInfo(UI_Element* el) {
    if (!ScreenManager_currentSector()) {
        return;
    }
    if (ScreenManager_currentSector()->explored) {
        DrawText(TextFormat("Population: %iM", ScreenManager_currentSector()->pop), el->x, el->y, 16, RAYWHITE);
        int line = 2;
        for (int i = 0; i < ScreenManager_currentSector()->planetnum; i++) {
            Sector_Planet p = Sector_getPlanet(ScreenManager_currentSector(), i);
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
                "Total ships: %d\nPopulation: %d\nFuel: %d/%d\nFood: %d\nStorage: %d/%d",
                f->unitmax, FLEET_BASIC_NEEDS, f->fuel, f->fuelMax, f->food,
                Fleet_getUsedStorage(f), Fleet_getMaxStorage(f)),
            el->x, el->y + 128, 8, RAYWHITE
        );
    }
}


void ScreenMap_init() {  
    UI_createElement(0, 40, World_sizeX*UI_tileSize, World_sizeY*UI_tileSize, "Map Display", SCREEN_MAP, NOFUNC, _drawMap, _clickMap, NOFUNC);
    UI_createElement(600, 40, 300, World_sizeY, "Sector Info", SCREEN_MAP, NOFUNC, _drawSectorInfo, NOFUNC, NOFUNC);
    UI_createElement(600, 300, 132, 32, "Set Destination", SCREEN_MAP, _setDestinationAvailable, UI_drawButton, _clickSetDestination, _targeterSetDestination);
}