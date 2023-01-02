#ifndef SECTOR_H
#define SECTOR_H

typedef enum {
    STAR_NONE, STAR_RED, STAR_YELLOW, STAR_BLUE, STAR_WHITE, STAR_END
} Sector_StarType;

typedef enum {
    PLANET_NONE, PLANET_GAIA, PLANET_TERRAN, PLANET_OCEAN, PLANET_DESERT,
    PLANET_TUNDRA, PLANET_BARREN, PLANET_VOLCANIC, PLANET_GAS_GIANT, PLANET_ALL
} Sector_PlanetType;

extern char Sector_planetStrings[PLANET_ALL][16];

typedef enum {
    TEMP_X_HOT, TEMP_HOT, TEMP_Q_HOT, TEMP_GOLDILOCKS, TEMP_Q_COLD, TEMP_COLD, TEMP_X_COLD
} Sector_PlanetTemperature;

extern char Sector_tempStrings[TEMP_X_COLD+1][16];

typedef enum {
    RES_NONE, RES_BASE_METALS, RES_FERTILE_SOIL, RES_FABRICS, RES_ANTIMATTER, RES_WARP_SEED, RES_ALL
} Sector_ResourceType;

extern char Sector_resourceStrings[RES_ALL][16];

typedef struct {
    Sector_ResourceType type;
    int abundance;
} Sector_Resource;

#define RESOURCE_MAX 8

typedef struct {
    Sector_PlanetType type;
    int size;
    Sector_PlanetTemperature temperature;    
    int pop;
    int popMax;
    Sector_Resource resources[RESOURCE_MAX];
    int resourcenum;
    int units[8]; // Units deployed at this planet for mining, research etc.
    int unitnum;
    int hostile;
} Sector_Planet;

typedef enum {
    PH_NONE, PH_GAS_CLOUD, PH_ASTEROID_FIELD, PH_ABANDONED_STATION, PH_PRECURSOR_REMNANTS
} Sector_Phenomena;

#define PLANET_MAX 12
#define PHENOMENA_MAX 8

typedef struct {
    Sector_StarType star;
    int pop;
    int popMax;
    int explored;
    int fleet;
    int x;
    int y;
    Sector_Planet planets[PLANET_MAX];
    int planetnum;
    Sector_Phenomena phenomena[PHENOMENA_MAX];
    int phenomenanum;
    int hostile;
} Sector_Entity;

typedef struct {
    Sector_StarType star;
    int startingLocation;
} Sector_Template;

Sector_Entity Sector_create(Sector_Template st);
void Sector_simulate(Sector_Entity* s);
void Sector_planetAddResource(Sector_Planet* p, Sector_ResourceType t, int abundance);
Sector_Planet Sector_getPlanet(Sector_Entity* s, int index);
void Sector_deployUnitToPlanet(Sector_Planet* p, int uid);
void Sector_removeUnitFromPlanetByIndex(Sector_Planet* p, int index);
int Sector_resourceBasePrice(Sector_Planet* p, Sector_ResourceType r);

#endif