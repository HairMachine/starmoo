#ifndef RESEARCH_H
#define RESEARCH_H

typedef enum {
    FIELD_PROPULSION, FIELD_CONSTRUCTION, FIELD_WEAPONS, FIELD_ALL
} Research_Field;

extern char Research_fieldStrings[FIELD_ALL][32];

typedef struct {
    char name[32];
    Research_Field field;
    int level;
    int pointsRequired;
    int points;
    int discovered;
    int developed;
} Research_Tech;

void Research_advance(Research_Field field, int points);

#endif

