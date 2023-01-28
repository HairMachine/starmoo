
#ifndef FLEET_H
#define FLEET_H

#include "sector.h"
#include "research.h"

#define FLEET_BASIC_NEEDS f->lowpop + f->medpop + f->highpop + f->ultrapop
#define FLEET_STANDARD_NEEDS f->medpop + f->highpop + f->ultrapop
#define FLEET_HIGH_NEEDS f->highpop + f->ultrapop
#define FLEET_ULTRAHIGH_NEEDS f->ultrapop

typedef struct {
    int orders;
    int units[64];
    int unitmax;
    int warpFactor;
    int food;
    int recreation;
    int luxury;
    int education;
    int lowpop;
    int medpop;
    int highpop;
    int ultrapop;
    int scientists;
    int unrest;
    int development;
    int credits;
    int size;
    int maxSize;
    int hasBuiltThisTurn;
} Fleet_Entity;

#define MAXUPGRADES 20

typedef struct {
    char name[32];
    Sector_Resource buildCosts[4];
    Research_TechType techRequired;
    Research_TechType obsoletedBy;
    int development;
    int warpFactor;
    int sizeIncrease;
    int complete;
} Fleet_Upgrade;

Fleet_Entity* Fleet_create();
void Fleet_addUnit(Fleet_Entity* f, int uid);
void Fleet_removeUnitAtIndex(Fleet_Entity* f, int index);
Fleet_Entity* Fleet_getPointer(int index);
void Fleet_destroy(int index);
int Fleet_count();
int Fleet_getUnitCountByDesign(Fleet_Entity* f, int designId);
void Fleet_simulate(Fleet_Entity* f);
int Fleet_canMine(Fleet_Entity *f);
int Fleet_canResearch(Fleet_Entity* f);
int Fleet_hasEnoughItems(Fleet_Entity* f, Sector_Resource r);
void Fleet_collectItems(Fleet_Entity* f, Sector_Resource r);
void Fleet_consumeItems(Fleet_Entity* f, Sector_Resource r);
int Fleet_getAssignedPop(Fleet_Entity* f);
Fleet_Upgrade Fleet_getUpgrade(int upgrade);
void Fleet_applyUpgrade(Fleet_Entity* f, int upgrade);

#endif

