#include <stdlib.h>

#include "sector.h"
#include "event.h"
#include "order.h"
#include "unit.h"
#include "fleet.h"
#include "world.h"

Sector_Entity World_sectors[World_sizeY * World_sizeX];

/**
 * Algorithm that creates a universe. 
 */
void World_create() {
    for (int x = 0; x < World_sizeX; x+=3) {
        for (int y = 0; y < World_sizeY; y+=3) {
            Sector_Template st = {STAR_NONE, 0};
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
                    st.startingLocation = 1;
                } else {
                    Sector_StarType star = rand() % (STAR_END - 1) + 1;
                    st.star = star;
                    st.startingLocation = 0;
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

void World_resupplyFleet(Sector_Entity* s) {
    if (s->fleet == -1) {
        return;
    }
    Fleet_Entity* f = Fleet_getPointer(s->fleet);
    if (f->fuel >= f->fuelMax) {
        return;
    }
    int refuelChance = 0;
    if (s->pop > 0) {
        // TODO: More complex resupply logic, consuming resources, taking into account infrastructure etc.
        refuelChance = 100;
    } else if (s->star > STAR_NONE) {
        refuelChance = 10;
    }
    if (refuelChance <= 0) {
        return;
    }
    // Supply ships have extra fuel generators on board
    for (int i = 0; i < f->unitmax; i++) {
        if (Unit_getCopy(f->units[i]).fuelMax > 0) {
            refuelChance += 5;
        }
    }
    // % chance of refuelling; values > 100 give extra chances to gain a fuel unit
    while (refuelChance > 0) {
        if (rand() % 100 < refuelChance) {
            f->fuel++;
        }
        refuelChance -= 100;
    }
    // Send an event if the fleet has been fully refuelled
    if (f->fuel >= f->fuelMax) {
        f->fuel = f->fuelMax;
    }
}

/**
 * Called each turn; updates the universe. 
 */
void World_update() {
    // Process player orders
    for (int i = Order_count() - 1; i >= 0; i--) {
        Order_Entity* o = Order_getPointer(i);
        switch (o->type) {
            case ORDER_MOVE_FLEET:
                int dirx, diry;
                Sector_Entity* se = &World_sectors[o->originY * World_sizeX + o->originX];
                Fleet_Entity* f = Fleet_getPointer(se->fleet);
                for (int m = 0; m < f->warpFactor; m++) {
                    // Consume supplies - if the fleet has no supplies, it can't move until it generates more
                    if (f->fuel <= 0) {
                        break;
                    }
                    f->fuel--;
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
                        Order_complete(i);
                        break;
                    }
                }
                break;
            default:
                break;
        }
    }
    // Simulate all sectors
    for (int x = 0; x < World_sizeX; x++) {
        for (int y = 0; y < World_sizeY; y++) {
            Sector_Entity* s = &World_sectors[y * World_sizeX + x];
            Sector_simulate(s);
            World_resupplyFleet(s);
            if (s->fleet > -1) {
                Fleet_simulate(Fleet_getPointer(s->fleet));
            }
        }
    }
}

Sector_Entity World_getSectorCopy(int x, int y) {
    return World_sectors[y * World_sizeX + x];
}

Sector_Entity* World_getSectorPointer(int x, int y) {
    return &World_sectors[y * World_sizeX + x];
}