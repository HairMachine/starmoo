#include <stdlib.h>
#include <math.h>

#include "sector.h"

char Sector_resourceStrings[RES_ALL][8] = {
    "None", "Oxygen", "Food", "Water", "Fuel"
};

char Sector_tempStrings[TEMP_X_COLD+1][14] = {
    "Extremely Hot", "Very Hot", "Hot", "Temperate", "Cold", "Very Cold", "Extremely Cold" 
};

char Sector_planetStrings[PLANET_ALL][10] = {
    "None", "Gaia", "Terran", "Ocean", "Desert", "Tundra", "Barren", "Volcanic", "Gas Giant"
};

Sector_Planet _generatePlanet(Sector_StarType star, int distFromStar) {
    Sector_Planet p;
    p.temperature = distFromStar <= TEMP_X_COLD ? distFromStar : TEMP_X_COLD;
    // Generate appropriate type, some types don't work with particular temperatures
    int typeOk = 0;
    while (!typeOk) {
        p.type = rand() % PLANET_ALL;
        switch (p.type) {
            case PLANET_GAIA:
            case PLANET_TERRAN:
                typeOk = p.temperature == TEMP_GOLDILOCKS;
                break;
            case PLANET_OCEAN:
            case PLANET_DESERT:
                typeOk = p.temperature < TEMP_COLD && p.temperature > TEMP_HOT;
                break;
            case PLANET_TUNDRA:
                typeOk = p.temperature >= TEMP_COLD;
                break;
            case PLANET_VOLCANIC:
                typeOk = p.temperature <= TEMP_HOT;
                break;
            case PLANET_NONE:
                p.size = 0;
                p.pop = 0;
                p.popMax = 0;
                p.artefacts = 0;
                p.abandoned = 0;
                p.resourcenum = 0;
                return p;
            default:
                typeOk = 1;
                break;
        }
    }
    // Generate a size based on type
    if (p.type == PLANET_GAS_GIANT) {
        p.size = (rand() % 500) + 500;
    } else {
        p.size = (rand() % 250) + 50;
    }
    // Create some resources
    p.resourcenum = 0;
    int rnum = rand() % RESOURCE_MAX;
    for (int i = 0; i < rnum; i++) {
        int rtype = (rand() % RES_ALL - 1) + 1;
        int rq = rand() % 100 + 1;
        Sector_planetAddResource(&p, rtype, rq);
    }
    // Population based on size and other factors
    if (p.type == PLANET_GAS_GIANT) {
        p.popMax = 0;
    } else {
        int typemod = 0;
        switch (p.type) {
            case PLANET_TERRAN:
                typemod = 10;
                break;
            case PLANET_DESERT:
            case PLANET_OCEAN:
            case PLANET_TUNDRA:
                typemod = 50;
                break;
            case PLANET_VOLCANIC:
            case PLANET_BARREN:
                typemod = 200;
                break;
            default:
                break;
        }
        p.popMax = p.size - (abs(p.temperature - 3) * abs(p.temperature - 3) * 25) - typemod + rand() % 50;
        if (p.popMax < 0) {
            p.popMax = 0;
        } else {
            p.popMax *= 10;
        }
    }
    if (p.popMax > 0) {
        p.pop = round(p.popMax * (rand() % 25 + 50) / 100);
    } else {
        p.pop = 0;
    }
    p.artefacts = 0;
    p.abandoned = 0;
    
    // TODO: Phenomena
    // TODO: Make adjustments up / down by star type
    return p;
}

Sector_Entity Sector_create(Sector_Template st) {
    Sector_Entity s;
    s.star = st.star;
    s.pop = 0;
    s.popMax = 0;
    for (int i = 0; i < PLANET_MAX; i++) {
        Sector_Planet p = {PLANET_NONE, 0, TEMP_X_COLD, 0, 0, 0, 0, {}, 0};
        s.planets[i] = p;
    }
    if (st.startingLocation) {
        s.explored = 1;
        s.fleet = 0;
        Sector_Planet homeworld = {PLANET_TERRAN, 300, TEMP_GOLDILOCKS, 0, 0, 1000, 2000, {} ,0};
        Sector_planetAddResource(&homeworld, RES_OXYGEN, 100);
        Sector_planetAddResource(&homeworld, RES_FOOD, 80);
        Sector_planetAddResource(&homeworld, RES_WATER, 75);
        Sector_planetAddResource(&homeworld, RES_FUEL, 50);
        s.planets[3] = homeworld;
        s.planetnum = 6;
    } else {
        s.explored = 0;
        s.fleet = -1;
        s.planetnum = 0;
    }
    if (s.star == STAR_NONE) {
        return s;
    }
    // Create some random planets
    if (!s.planetnum) {
        s.planetnum = rand() % PLANET_MAX;
    }
    for (int i = 0; i < s.planetnum; i++) {
        if (s.planets[i].type == PLANET_NONE) {
            s.planets[i] = _generatePlanet(s.star, i);
        }
    }
    // Add up population
    for (int i = 0; i < s.planetnum; i++) {
        s.pop += s.planets[i].pop;
        s.popMax += s.planets[i].popMax;
    }
    return s;
}

void Sector_planetAddResource(Sector_Planet* p, Sector_ResourceType t, int abundance) {
    Sector_Resource newResource = {t, abundance};
    p->resources[p->resourcenum] = newResource;
    p->resourcenum++;
}

Sector_Planet Sector_getPlanet(Sector_Entity* s, int index) {
    return s->planets[index];
}

/**
 * Simulates an individual sector of the galaxy.
 * Less detail than fleet simulation; random events, create resources and goods, etc.
 */
void Sector_simulate(Sector_Entity* s) {

}

