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
    // Basic resources
    RES_NONE, RES_BASE_METALS, RES_FERTILE_SOIL, RES_FABRICS, RES_DEUTERIUM, RES_SILICON, 
    RES_WARP_SEEDS, RES_CYTRONIUM, RES_MAGNETRIUM, RES_SUBFILAMENTS, RES_HYPERALLOYS,
    // Luxury resources
    RES_STIM_CELLS, RES_FINE_FRUIT, RES_REGENATRONS, RES_PRECIOUS_ORES,
    // Goods
    RES_WINE_1, RES_STIM_1, RES_MEDICAL_1, RES_LUXURY_1,
    RES_WINE_2, RES_STIM_2, RES_MEDICAL_2, RES_LUXURY_2,
    RES_WINE_3, RES_STIM_3, RES_MEDICAL_3, RES_LUXURY_3,
    RES_WINE_4, RES_STIM_4, RES_MEDICAL_4, RES_LUXURY_4,
    RES_WINE_5, RES_STIM_5, RES_MEDICAL_5, RES_LUXURY_5,
    RES_WINE_6, RES_STIM_6, RES_MEDICAL_6, RES_LUXURY_6,
    RES_ALL
} Sector_ResourceType;

#define GOOD_TYPES 4

extern char Sector_resourceStrings[RES_ALL][18];

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
    int funds;
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
    int wealthLevel;
} Sector_Entity;

typedef struct {
    Sector_StarType star;
    int specialLocation;
    int distFromCentre;
} Sector_Template;

Sector_Entity Sector_create(Sector_Template st);
void Sector_simulate(Sector_Entity* s);
void Sector_planetAddResource(Sector_Planet* p, Sector_ResourceType t, int abundance);
Sector_Planet Sector_getPlanet(Sector_Entity* s, int index);
void Sector_deployUnitToPlanet(Sector_Planet* p, int uid);
void Sector_removeUnitFromPlanetByIndex(Sector_Planet* p, int index);
int Sector_resourceBasePrice(Sector_Planet* p, Sector_ResourceType r, int wealthLevel);
int Sector_resourceQuality(Sector_ResourceType r);
int Sector_planetBaseFunds(Sector_Entity* s, Sector_Planet* p);

#endif