#include <stdlib.h>
#include <math.h>

#include "sector.h"

char Sector_resourceStrings[RES_ALL][16] = {
    "None", "Base Metals", "Fertile Soil", "Fabrics", "Antimatter", "Warp Seed"
};

char Sector_tempStrings[TEMP_X_COLD+1][16] = {
    "Extremely Hot", "Very Hot", "Hot", "Temperate", "Cold", "Very Cold", "Extremely Cold" 
};

char Sector_planetStrings[PLANET_ALL][16] = {
    "None", "Gaia", "Terran", "Ocean", "Desert", "Tundra", "Barren", "Volcanic", "Gas Giant"
};

void _getPossibleResourcesByPlanet(Sector_Planet p, Sector_Resource rt[]) {
    // Reset the template
    for (int i = 0; i < RESOURCE_MAX; i++) {
        rt[i] = (Sector_Resource) {RES_NONE, 0};
    }
    switch (p.type) {
        case PLANET_GAIA:
            rt[0] = (Sector_Resource) {RES_FERTILE_SOIL, 100};
            rt[1] = (Sector_Resource) {RES_BASE_METALS, 15};
            rt[2] = (Sector_Resource) {RES_FABRICS, 50};
            if (rand() % 100 <= 5) {
                rt[3] = (Sector_Resource) {RES_ANTIMATTER, 5};
            } 
            break;
        case PLANET_TERRAN:
            rt[0] = (Sector_Resource) {RES_FERTILE_SOIL, 80};
            rt[1] = (Sector_Resource) {RES_BASE_METALS, 15};
            rt[2] = (Sector_Resource) {RES_FABRICS, 40};
            if (rand() % 100 <= 5) {
                rt[3] = (Sector_Resource) {RES_ANTIMATTER, 5};
            } 
            break;
        case PLANET_DESERT:
        case PLANET_OCEAN:
            rt[0] = (Sector_Resource) {RES_FERTILE_SOIL, 15};
            rt[1] = (Sector_Resource) {RES_BASE_METALS, 30};
            rt[2] = (Sector_Resource) {RES_FABRICS, 20};
            if (rand() % 100 <= 5) {
                rt[3] = (Sector_Resource) {RES_ANTIMATTER, 5};
            } 
            break;
        case PLANET_TUNDRA:
        case PLANET_VOLCANIC:
            rt[0] = (Sector_Resource) {RES_BASE_METALS, 30};
            if (rand() % 100 <= 15) {
                rt[1] = (Sector_Resource) {RES_ANTIMATTER, 10};
            } 
            break;
        case PLANET_BARREN:
            rt[0] = (Sector_Resource) {RES_BASE_METALS, 80};
            if (rand() % 100 <= 25) {
                rt[1] = (Sector_Resource) {RES_ANTIMATTER, 25};
            }
            break;
        case PLANET_GAS_GIANT:
            if (rand() % 100 <= 50) {
                rt[0] = (Sector_Resource) {RES_ANTIMATTER, 45};
            }
            break;
        default:
            break;
    }
}

Sector_Planet _generatePlanet(Sector_StarType star, int distFromStar) {
    Sector_Planet p;
    p.unitnum = 0;
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
    Sector_Resource resourceTemplate[RESOURCE_MAX] = {};
    _getPossibleResourcesByPlanet(p, resourceTemplate);
    for (int i = 0; i < RESOURCE_MAX; i++) {
        if (resourceTemplate[i].type != RES_NONE) {
            int rq = resourceTemplate[i].abundance + (rand() % 50 + 1) - (rand() % 50 + 1);
            Sector_planetAddResource(&p, resourceTemplate[i].type, rq);
        }
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
        Sector_Planet p = {PLANET_NONE, 0, TEMP_X_COLD, 0, 0, 0, 0, {}, 0, {}, 0};
        s.planets[i] = p;
    }
    if (st.startingLocation) {
        s.explored = 1;
        s.fleet = 0;
        Sector_Planet homeworld = {PLANET_TERRAN, 300, TEMP_GOLDILOCKS, 0, 0, 1000, 2000, {} ,0};
        Sector_planetAddResource(&homeworld, RES_BASE_METALS, 50);
        Sector_planetAddResource(&homeworld, RES_ANTIMATTER, 25);
        Sector_planetAddResource(&homeworld, RES_FABRICS, 75);
        Sector_planetAddResource(&homeworld, RES_FERTILE_SOIL, 80);
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
    if (abundance <= 0) {
        return;
    }
    Sector_Resource newResource = {t, abundance};
    p->resources[p->resourcenum] = newResource;
    p->resourcenum++;
}

Sector_Planet Sector_getPlanet(Sector_Entity* s, int index) {
    return s->planets[index];
}

void Sector_deployUnitToPlanet(Sector_Planet* p, int uid) {
    p->units[p->unitnum] = uid;
    p->unitnum++;
}

void Sector_removeUnitFromPlanetByIndex(Sector_Planet* p, int index) {
    for (int i = index; i < p->unitnum; i++) {
        p->units[i] = p->units[i + 1];
    }
    p->unitnum--;
}

/**
 * Simulates an individual sector of the galaxy.
 * Less detail than fleet simulation; random events, create resources and goods, etc.
 */
void Sector_simulate(Sector_Entity* s) {
    
}

