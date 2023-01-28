#include <stdlib.h>
#include <math.h>

#include "sector.h"

char Sector_resourceStrings[RES_ALL][32] = {
    // Base resources
    "None", "Base Metals", "Fertile Soil", "Fabrics", "Deuterium", "Silicon",
    "Warp Seeds", "Cytronium", "Magnetrium", "Subfilaments", "Hyperalloys",
    "Stim Cells", "Fine Fruit", "Regenatrons", "Precious Ores",
    // Goods
    "Basic Wine", "Basic Stims", "Basic Medicine", "Basic Goods",
    "Standard Wine", "Standard Stims", "Standard Medicine", "Standard Goods",
    "Decent Wine", "Decent Stims", "Decent Medicine", "Decent Goods",
    "Fine Wine", "Fine Stims", "Fine Medicine", "Fine Goods",
    "Excellent Wine", "Excellent Stims", "Excellent Medicine", "Excellent Goods",
    "Masterpiece Wine", "Masterpiece Stims", "Masterpiece Medicine", "Masterpiece Goods",
};

char Sector_tempStrings[TEMP_X_COLD+1][16] = {
    "Extremely Hot", "Very Hot", "Hot", "Temperate", "Cold", "Very Cold", "Extremely Cold" 
};

char Sector_planetStrings[PLANET_ALL][16] = {
    "None", "Gaia", "Terran", "Ocean", "Desert", "Tundra", "Barren", "Volcanic", "Gas Giant"
};

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
        Sector_planetAddResource(&homeworld, RES_BASE_METALS, 150);
        Sector_planetAddResource(&homeworld, RES_FERTILE_SOIL, 180);
        Sector_planetAddResource(&homeworld, RES_SILICON, 125);
        Sector_planetAddResource(&homeworld, RES_DEUTERIUM, 115);
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
        s.finalBoss = 1;
    } else {
        s.fleet = -1;
        if (rand() % 100 < 80 - st.distFromCentre*2) {
            s.hostile = 1;
        }
        s.wealthLevel = 100 - (st.distFromCentre / 3) * 10;
        if (s.wealthLevel < 0) {
            s.wealthLevel = 0;
        }
        // Randomly generate a cache 10% of the time
        if (rand() % 100 < 10) {
            s.cache = (Sector_Resource) {rand() % RES_SILICON + 1, rand() % 25 + 50};
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

void Sector_planetAddResearchBonus(Sector_Planet* p, Research_Field rf, int amount) {
    Sector_ResearchBonus newBonus = {rf, amount};
    p->researchBonuses[p->researchBonusNum] = newBonus;
    p->researchBonusNum++;
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
        bp = 10;
    } else if (r <= RES_HYPERALLOYS) {
        bp = 20;
    } else if (r <= RES_PRECIOUS_ORES) {
        bp = 40;
    } else {
        bp = 100 * (1 + (floor((r - RES_PRECIOUS_ORES - 1) / 4)));
    }
    if (bp == 0) bp = 1;
    int maxReduction = bp / 5;
    for (int i = 0; i < p->resourcenum; i++) {
        if (p->resources[i].type == r) {
            return (bp - ((100 - p->resources[i].abundance) * maxReduction) / 100) * 10;
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

