#include <stdlib.h>
#include <math.h>

#include "sector.h"

char Sector_resourceStrings[RES_ALL][18] = {
    // Base resources
    "None", "Base Metals", "Fertile Soil", "Fabrics", "Deuterium", "Silicon",
    "Warp Seeds", "Cytronium", "Magnetrium", "Subfilaments", "Hyperalloys",
    "Stim Cells", "Fine Fruit", "Regenatrons", "Precious Ores",
    // Goods
    "Barathian Wine 1", "Stims 1", "Medicine 1", "Luxury Goods 1",
    "Barathian Wine 2", "Stims 2", "Medicine 2", "Luxury Goods 2",
    "Barathian Wine 3", "Stims 3", "Medicine 3", "Luxury Goods 3",
    "Barathian Wine 4", "Stims 4", "Medicine 4", "Luxury Goods 4",
    "Barathian Wine 5", "Stims 5", "Medicine 5", "Luxury Goods 5",
    "Barathian Wine 6", "Stims 6", "Medicine 6", "Luxury Goods 6",
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
            rt[3] = (Sector_Resource) {RES_REGENATRONS, 10};
            rt[4] = (Sector_Resource) {RES_STIM_CELLS, 25};
            rt[5] = (Sector_Resource) {RES_FINE_FRUIT, 15};
            rt[6] = (Sector_Resource) {RES_PRECIOUS_ORES, 5};
            rt[7] = (Sector_Resource) {RES_SILICON, 10};
            break;
        case PLANET_TERRAN:
            rt[0] = (Sector_Resource) {RES_FERTILE_SOIL, 80};
            rt[1] = (Sector_Resource) {RES_BASE_METALS, 15};
            rt[2] = (Sector_Resource) {RES_FABRICS, 40};
            rt[3] = (Sector_Resource) {RES_REGENATRONS, 5};
            rt[4] = (Sector_Resource) {RES_STIM_CELLS, 15};
            rt[5] = (Sector_Resource) {RES_FINE_FRUIT, 10};
            rt[6] = (Sector_Resource) {RES_PRECIOUS_ORES, 5};
            rt[7] = (Sector_Resource) {RES_SILICON, 10};
            break;
        case PLANET_DESERT:
        case PLANET_OCEAN:
            rt[0] = (Sector_Resource) {RES_FERTILE_SOIL, 15};
            rt[1] = (Sector_Resource) {RES_BASE_METALS, 30};
            rt[2] = (Sector_Resource) {RES_FABRICS, 20};
            rt[3] = (Sector_Resource) {RES_REGENATRONS, 5};
            rt[4] = (Sector_Resource) {RES_STIM_CELLS, 5};
            rt[5] = (Sector_Resource) {RES_FINE_FRUIT, 5};
            break;
        case PLANET_TUNDRA:
        case PLANET_VOLCANIC:
            rt[0] = (Sector_Resource) {RES_BASE_METALS, 30};
            rt[1] = (Sector_Resource) {RES_SILICON, 15};
            rt[2] = (Sector_Resource) {RES_MAGNETRIUM, 25};
            rt[3] = (Sector_Resource) {RES_CYTRONIUM, 15};
            rt[4] = (Sector_Resource) {RES_HYPERALLOYS, 20};
            rt[5] = (Sector_Resource) {RES_PRECIOUS_ORES, 15};
            break;
        case PLANET_BARREN:
            rt[0] = (Sector_Resource) {RES_BASE_METALS, 80};
            rt[1] = (Sector_Resource) {RES_SILICON, 50};
            rt[2] = (Sector_Resource) {RES_MAGNETRIUM, 25};
            rt[3] = (Sector_Resource) {RES_CYTRONIUM, 15};
            rt[4] = (Sector_Resource) {RES_HYPERALLOYS, 10};
            rt[5] = (Sector_Resource) {RES_PRECIOUS_ORES, 35};
            break;
        case PLANET_GAS_GIANT:
            rt[0] = (Sector_Resource) {RES_DEUTERIUM, 50};
            rt[1] = (Sector_Resource) {RES_SUBFILAMENTS, 10};
            break;
        default:
            break;
    }
}

Sector_Planet _generatePlanet(Sector_StarType star, int distFromStar, int wealthLevel) {
    Sector_Planet p = (Sector_Planet) {};
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
    // Create some resources
    p.resourcenum = 0;
    Sector_Resource resourceTemplate[RESOURCE_MAX] = {};
    _getPossibleResourcesByPlanet(p, resourceTemplate);
    for (int i = 0; i < RESOURCE_MAX; i++) {
        if (resourceTemplate[i].type != RES_NONE && (p.pop == 0 || wealthLevel <= Sector_resourceQuality(resourceTemplate[i].type))) {
            int rq = resourceTemplate[i].abundance + (rand() % 25 + 1) - (rand() % 50 + 1);
            Sector_planetAddResource(&p, resourceTemplate[i].type, rq);
        }
    }
    // Create goods production on wealthier planets with more deveoped industries
    if (p.pop > 0 && wealthLevel >= 10) {
        Sector_ResourceType baseGood = wealthLevel / 20 * GOOD_TYPES + RES_PRECIOUS_ORES + 1;
        Sector_ResourceType startGood = baseGood - GOOD_TYPES;
        Sector_ResourceType endGood = baseGood + GOOD_TYPES;
        int possibleGoods = RESOURCE_MAX - p.resourcenum;
        if (possibleGoods > endGood - startGood) {
            possibleGoods = endGood - startGood;
        }
        int goods = rand() % possibleGoods + 1;
        Sector_ResourceType goodsList[5] = {};
        int i = 0;
        for (Sector_ResourceType good = startGood; good < endGood; good++) {
            goodsList[i] = good;
            i++;
        }
        for (int j = 0; j < goods; j++) {
            int roll = rand() % i;
            Sector_ResourceType good = goodsList[roll];
            int abundance = 101 + rand() % 75;
            Sector_planetAddResource(&p, good, abundance);
            for (int k = roll; k < i; k++) {
                goodsList[k] = goodsList[k + 1];
            }
            i--;
        }
    }
    // TODO: Phenomena
    // TODO: Make adjustments up / down by star type
    return p;
}

Sector_Entity Sector_create(Sector_Template st) {
    Sector_Entity s = (Sector_Entity) {};
    s.star = st.star;
    for (int i = 0; i < PLANET_MAX; i++) {
        Sector_Planet p = (Sector_Planet) {};
        s.planets[i] = p;
    }
    if (st.specialLocation == 1) {
        s.explored = 1;
        Sector_Planet homeworld = {PLANET_TERRAN, 300, TEMP_GOLDILOCKS, 0, 0, 1000, 2000, {} ,0};
        Sector_planetAddResource(&homeworld, RES_BASE_METALS, 50);
        Sector_planetAddResource(&homeworld, RES_SILICON, 25);
        Sector_planetAddResource(&homeworld, RES_FABRICS, 75);
        Sector_planetAddResource(&homeworld, RES_FERTILE_SOIL, 80);
        homeworld.pop = 1000;
        s.planets[3] = homeworld;
        s.planetnum = 6;
        s.wealthLevel = 0;
    } else if (st.specialLocation == 2) {
        s.hostile = 1;
        Sector_Planet enemyworld = {PLANET_GAIA, 300, TEMP_GOLDILOCKS, 0, 0, 1000, 2000, {} ,0};
        enemyworld.pop = 5000;
        s.planets[3] = enemyworld;
        s.planetnum = 6;
        s.wealthLevel = 100;
        s.fleet = -1;
    } else {
        s.fleet = -1;
        if (rand() % 100 < 80 - st.distFromCentre*2) {
            s.hostile = 1;
        }
        s.wealthLevel = 100 - (st.distFromCentre / 3) * 10;
        if (s.wealthLevel < 0) {
            s.wealthLevel = 0;
        }
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
            s.planets[i] = _generatePlanet(s.star, i, s.wealthLevel);
        }
        // Generate some funds based on pop and wealth level
        s.planets[i].funds = Sector_planetBaseFunds(&s, &s.planets[i]);
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
    if (abundance > 100) {
        abundance -= 100;
    } else {
        if (rand() % 100 > abundance*2) {
            return;
        }
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

int Sector_resourceBasePrice(Sector_Planet* p, Sector_ResourceType r, int wealthLevel) {
    int bp = 0;
    if (r <= RES_FABRICS) {
        bp = 100;
    } else if (r <= RES_HYPERALLOYS) {
        bp = 150;
    } else if (r <= RES_PRECIOUS_ORES) {
        bp = 200;
    } else {
        bp = 400 * (1 + (r - floor(RES_PRECIOUS_ORES / 5)));
    }
    // Alter by quality compared to wealth level
    if (wealthLevel > 0) {
        int quality = Sector_resourceQuality(r) + 10;
        bp = (bp * ((10*quality) / (wealthLevel+10))) / 10;
    }
    if (bp == 0) bp = 1;
    for (int i = 0; i < p->resourcenum; i++) {
        if (p->resources[i].type == r) {
            return ((100 - p->resources[i].abundance) * bp) / 100;
        }
    }
    return bp;
}

int Sector_resourceQuality(Sector_ResourceType r) {
    if (r <= RES_HYPERALLOYS) {
        return 0;
    }
    if (r <= RES_PRECIOUS_ORES) {
        return 10;
    }
    return 10 + ceil((r - RES_PRECIOUS_ORES) / 4) * 10;
}

int Sector_planetBaseFunds(Sector_Entity* s, Sector_Planet* p) {
    return p->pop * (s->wealthLevel+1);
}

/**
 * Simulates an individual sector of the galaxy.
 * Less detail than fleet simulation; random events, create resources and goods, etc.
 */
void Sector_simulate(Sector_Entity* s) {
    for (int i = 0; i < s->planetnum; s++) {
        Sector_Planet* p = &s->planets[i];
        int baseFunds = Sector_planetBaseFunds(s, p);
        if (p->funds < baseFunds) {
            p->funds += ceil((p->pop * (s->wealthLevel+1)) / 100);
        }
    }
}

