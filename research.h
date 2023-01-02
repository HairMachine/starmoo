#ifndef RESEARCH_H
#define RESEARCH_H

typedef enum {
    FIELD_PROPULSION, FIELD_CONSTRUCTION, FIELD_WEAPONS, FIELD_ALL
} Research_Field;

extern char Research_fieldStrings[FIELD_ALL][32];

typedef enum {
    RT_NONE, 
    RT_WARP_2, RT_CONSTRUCTION_2, RT_LASER_2,
    RT_ALL
} Research_TechType;

typedef struct {
    char name[32];
    Research_Field field;
    int level;
    int pointsRequired;
    int discovered;
    int developed;
    int points;
} Research_Tech;

void Research_advance(Research_Field field, int points);
int Research_techIsDeveloped(Research_TechType rt);

#endif

