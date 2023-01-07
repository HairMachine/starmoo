#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "fleet.h"
#include "unit.h"
#include "combat.h"
#include "sector.h"

Combat_Unit c_units[128];
int c_unitnum = 0;
int active = 0;
int startDesign = 0;
int endDesign = 0;
Fleet_Entity* playerFleet = 0;

void Combat_createEnemyDesigns() {
    startDesign = Unit_designCount();
    // Laser fighter
    Unit_Design* ud = Unit_createDesign();
    strcpy(ud->name, "Laser Fighter");
    ud->componentnum = 1;
    ud->components[0] = Unit_getComponent(7);
    ud->components[1] = Unit_getComponent(8);
    endDesign = Unit_designCount();
}

void Combat_addShipToCombat(Unit_Entity u, int side, int originalIndex, int fleetIndex) {
    c_units[c_unitnum] = (Combat_Unit) {u, side, originalIndex, fleetIndex};
    c_unitnum++;
}

void Combat_addFleetShipsToCombat(Fleet_Entity* f) {
    for (int i = 0; i < f->unitmax; i++) {
        Unit_Entity u = Unit_getCopy(f->units[i]);
        if (u.canFight) {
            Combat_addShipToCombat(u, PLAYER_SIDE, f->units[i], i);
        }
    }
    playerFleet = f;
}

void Combat_setupRandomEncounter(Sector_Entity* s) {
    c_unitnum = 0;
    int fleetSize = s->wealthLevel * 10;
    // TODO: A much more sophisticated design that creates enemies that fit into the given
    // fleet size; larger fleet size means more dangerous enemies can be spawned
    int fart = ceil(fleetSize / 50);
    int numShips = ceil(fleetSize / 25) + rand() % fart;
    if (numShips > 128) {
        numShips = 128;
    }
    for (int i = 0; i < numShips; i++) {
        Unit_Design* d = Unit_getDesignPointer(startDesign);
        Unit_Entity u = Unit_generate(d);
        Combat_addShipToCombat(u, ENEMY_SIDE, -1, -1);
    }
}

void _removeUnit(int index) {
    for (int i = index; i < c_unitnum; i++) {
        c_units[i] = c_units[i + 1];
    }
    c_unitnum--;
}

void _destroyc_units() {
    for (int i = c_unitnum - 1; i >= 0; i--) {
        if (c_units[i].entity.hp <= 0) {
            if (c_units[i].unitIndex > -1) {
                Unit_getPointer(c_units[i].unitIndex)->hp = c_units[i].entity.hp;
            }
            if (c_units[i].fleetIndex > -1) {
                Fleet_removeUnitAtIndex(playerFleet, c_units[i].fleetIndex);
            }
            _removeUnit(i);
        }
    }
}

/**
 * A brawl! At the minute, all ships act simultaneously and fire all weapons at random targets.
 * Later, this will get more complicated and sophisticated; just need a basic test of fleet strength now.
 */
void _processRound() {
    for (int i = 0; i < c_unitnum; i++) {
        Combat_Unit* acting = &c_units[i];
        Combat_Unit* target = 0;
        while (!target || target->side == acting->side) {
            target = &c_units[rand() % c_unitnum];
        }
        // This is an alpha strike: all weapons unleash at once at a poor unfortunate target
        for (int j = 0; j < WEAPON_MAX; j++) {
            Unit_Component* wpn = &acting->entity.weapons[j];
            if (target->entity.shields > 0) {
                target->entity.shields -= wpn->shotPenetration;
            } else {
                target->entity.hp -= wpn->shotPower;
            }
        }
        // Recharge shields
        acting->entity.shields += acting->entity.shieldRechargeRate;
    }
    // Find and remove any c_units that have been destroyed
    _destroyc_units();
}

int Combat_shipsAlive(int side) {
    for (int i = 0; i < c_unitnum; i++) {
        if (c_units[i].side == side) {
            return 1;
        }
    }
    return 0;
}

void Combat_start() {
    active = 1;
}

void Combat_run() {
    if (Combat_shipsAlive(PLAYER_SIDE) && Combat_shipsAlive(ENEMY_SIDE)) {
        _processRound();
    } else {
        active = 0;
    }
}

int Combat_unitCount() {
    return c_unitnum;
}

Combat_Unit Combat_getUnitCopy(int index) {
    return c_units[index];
}

int Combat_active() {
    return active;
}