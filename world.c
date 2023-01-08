#include <stdlib.h>
#include <string.h>
#include "raylib.h"

#include "sector.h"
#include "event.h"
#include "order.h"
#include "combat.h"
#include "unit.h"
#include "fleet.h"
#include "world.h"

Sector_Entity World_sectors[World_sizeY * World_sizeX];
int legitimacy = 1000;

/**
 * Algorithm that creates a universe. 
 */
void World_create() {
    for (int x = 0; x < World_sizeX; x+=3) {
        for (int y = 0; y < World_sizeY; y+=3) {
            Sector_Template st = {STAR_NONE, 0};
            int distx = abs(x - World_sizeX / 2);
            int disty = abs(y - World_sizeY / 2);
            int fudge = rand() % 3;
            st.distFromCentre = (distx > disty ? distx : disty) * 2 - fudge;
            for (int x1 = 0; x1 < 3; x1++) {
                for (int y1 = 0; y1 < 3; y1++) {
                    Sector_Entity newSector = Sector_create(st);
                    newSector.x = x + x1;
                    newSector.y = y + y1;
                    World_sectors[newSector.y * World_sizeX + newSector.x] = newSector;
                }
            }
            int ok = 0;
            while (!ok) {
                int xoffs = rand() % 3;
                int yoffs = rand() % 3;
                // Recalculate if there's a star directly adjacent; it looks weird
                if (y > 0) {
                    if (World_sectors[(y + yoffs - 1) * World_sizeX + (x + xoffs)].star > STAR_NONE) {
                        continue;
                    }
                }
                if (x > 0) {
                    if (World_sectors[(y + yoffs) * World_sizeX + (x + xoffs - 1)].star > STAR_NONE) {
                        continue;
                    }
                }
                // The player's starting position is an established colony
                if (x == 0 && y == 0) {
                    st.star = STAR_YELLOW;
                    st.specialLocation = 1;
                } else if (x / 3 == World_sizeX / 6 && y / 3 == World_sizeY / 6) {
                    st.star = STAR_YELLOW;
                    st.specialLocation = 2;
                } else {
                    Sector_StarType star = rand() % (STAR_END - 1) + 1;
                    st.star = star;
                    st.specialLocation = 0;
                }
                Sector_Entity newSector = Sector_create(st);
                newSector.x = x + xoffs;
                newSector.y = y + yoffs;
                World_sectors[newSector.y * World_sizeX + newSector.x] = newSector;
                ok = 1;
            }
        }
    }
}

/**
 * Called each turn; updates the universe. 
 */
void World_update() {
    // Process player orders
    for (int i = 0 ; i < Order_count(); i++) {
        Order_Entity* o = Order_getPointer(i);
        switch (o->type) {
            case ORDER_MOVE_FLEET: {
                int dirx, diry;
                Sector_Entity* se = &World_sectors[o->originY * World_sizeX + o->originX];
                Fleet_Entity* f = Fleet_getPointer(se->fleet);
                for (int m = 0; m < f->warpFactor; m++) {
                    // Calculate move
                    dirx = o->targetX - o->originX;
                    diry = o->targetY - o->originY;
                    o->originX = dirx <= -1 ? o->originX - 1 : (dirx >= 1 ? o->originX + 1 : o->originX);
                    o->originY = diry <= -1 ? o->originY - 1 : (diry >= 1 ? o->originY + 1 : o->originY);
                    // Move fleet
                    se->fleet = -1;
                    se = &World_sectors[o->originY * World_sizeX + o->originX];
                    se->fleet = o->fleet;
                    // When reaching the end, complete the order
                    if (o->originX == o->targetX && o->originY == o->targetY) {
                        if (!se->explored) {
                            Event_create("System Discovered", "Captain, we have discovered a new system!");
                        }
                        se->explored = 1;
                        f->orders = -1;
                        o->completed = 1;
                        // Check if this system is hostile and generate combat if it is
                        if (se->hostile) {
                            Combat_setupRandomEncounter(se);
                            // TODO: Do not tie this to only having a single fleet!
                            Fleet_Entity* f = Fleet_getPointer(0);
                            Combat_addFleetShipsToCombat(f);
                            Combat_run();
                            Event_create("Attack!", "Captain, an enemy force is approaching...");
                        }
                        break;
                    }
                }
            } break;
            case ORDER_BUILD_SHIP: {
                Unit_Entity* builder = Unit_getPointer(o->param2);
                if (builder->hasBuiltThisTurn) {
                    break;
                }
                Unit_Entity* building = Unit_getPointer(o->param1);
                building->costToBuild -= builder->production;
                if (building->costToBuild <= 0) {
                    Event_create("Build finished", "Finished building ship");
                    o->completed = 1;
                    Fleet_Entity* f = Fleet_getPointer(o->fleet);
                    Fleet_addUnit(f, o->param1);
                }
                builder->hasBuiltThisTurn = 1;
            } break;
            default:
                break;
        }
    }
    Order_completeAll();
    // Simulate all sectors
    for (int x = 0; x < World_sizeX; x++) {
        for (int y = 0; y < World_sizeY; y++) {
            Sector_Entity* s = &World_sectors[y * World_sizeX + x];
            Sector_simulate(s);
            // Perform any mining and research tasks on planets
            Sector_Planet* p = 0;
            Unit_Entity* u = 0;
            int amount = 0;
            for (int pi = 0; pi < s->planetnum; pi++) {
                p = &s->planets[pi];
                for (int ui = 0; ui < p->unitnum; ui++) {
                    u = Unit_getPointer(p->units[ui]);
                    if (u->resourceMining) {
                        for (int ri = 0; ri < p->resourcenum; ri++) {
                            amount = 0;
                            int chance = (u->mining * p->resources[ri].abundance) / 100;
                            while (chance > 100) {
                                amount++;
                                chance -= 100;
                            }
                            if (rand() % 100 <= chance) {
                                amount++;
                            }
                            Unit_Inventory inv = {p->resources[ri].type, amount};
                            Unit_storeItem(u, inv);
                        }
                    }
                }
            }
            if (s->fleet > -1) {
                Fleet_simulate(Fleet_getPointer(s->fleet));
            }
        }
    }
    // Your people become more annoyed over time
    World_changeLegitimacy(-1);
}

Sector_Entity World_getSectorCopy(int x, int y) {
    return World_sectors[y * World_sizeX + x];
}

Sector_Entity* World_getSectorPointer(int x, int y) {
    return &World_sectors[y * World_sizeX + x];
}

int World_getLegitimacy() {
    return legitimacy;
}

void World_changeLegitimacy(int amnt) {
    legitimacy += amnt;
    if (legitimacy > 1000) {
        legitimacy = 1000;
    }
}