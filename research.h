#ifndef RESEARCH_H
#define RESEARCH_H

typedef enum {
    FIELD_PROPULSION, FIELD_CONSTRUCTION, FIELD_WEAPONS, FIELD_ESPIONAGE, FIELD_LUXURIES,
    FIELD_FORCEFIELDS, FIELD_ALL
} Research_Field;

extern char Research_fieldStrings[FIELD_ALL][32];

typedef enum {
    RT_NONE, 
    // Propulsion techs
    RT_WARP_2, RT_WARP_3, RT_WARP_4, RT_WARP_5, RT_WARP_6, RT_WARP_7, RT_WARP_8,
    RT_WARP_9, RT_WARP_10,
    // Construction techs
    RT_ARMOUR_2, RT_ARMOUR_3, RT_ARMOUR_4, RT_ARMOUR_5, RT_ARMOUR_6,
    RT_MINING_2, RT_MINING_3, RT_MINING_4, RT_MINING_5, RT_MINING_6,
    RT_CARRIER_1, RT_CARRIER_2, RT_CARRIER_3, RT_CARRIER_4, RT_CARRIER_5,
    RT_CONSTRUCT_1, RT_CONSTRUCT_2, RT_CONSTRUCT_3, RT_CONSTRUCT_4, RT_CONSTRUCT_5,
    // Weapons techs
    RT_BEAM_2, RT_BEAM_3, RT_BEAM_4, RT_BEAM_5, RT_BEAM_6, RT_BEAM_7, RT_BEAM_8,
    RT_GUN_2, RT_GUN_3, RT_GUN_4, RT_GUN_5, RT_GUN_6,
    RT_TORPEDO_1, RT_TORPEDO_2, RT_TORPEDO_3, RT_TORPEDO_4,
    // Espionage techs
    RT_ANTENNA_1, RT_ANTENNA_2, RT_ANTENNA_3, RT_ANTENNA_4, RT_ANTENNA_5,
    RT_ENCRYPTOR_1, RT_ENCRYPTOR_2, RT_ENCRYPTOR_3, RT_ENCRYPTOR_4, RT_ENCRYPTOR_5,
    RT_OVERRIDE_1, RT_OVERRIDE_2, RT_OVERRIDE_3, RT_OVERRIDE_4, RT_OVERRIDE_5,
    // Luxury techs
    RT_GOODS_1, RT_GOODS_2, RT_GOODS_3, RT_GOODS_4, RT_GOODS_5,
    RT_WINE_1, RT_WINE_2, RT_WINE_3, RT_WINE_4, RT_WINE_5,
    RT_MEDICAL_1, RT_MEDICAL_2, RT_MEDICAL_3, RT_MEDICAL_4, RT_MEDICAL_5,
    RT_STIMS_1, RT_STIMS_2, RT_STIMS_3, RT_STIMS_4, RT_STIMS_5,
    // Forcefield techs
    RT_SHIELD_2, RT_SHIELD_3, RT_SHIELD_4, RT_SHIELD_5, RT_SHIELD_6,
    RT_REFLECTOR_1, RT_REFLECTOR_2, RT_REFLECTOR_3, RT_REFLECTOR_4, RT_REFLECTOR_5,
    RT_CHARGER_1, RT_CHARGER_2, RT_CHARGER_3, RT_CHARGER_4, RT_CHARGER_5,
    RT_DRAINER_1, RT_DRAINER_2, RT_DRAINER_3, RT_DRAINER_4, RT_DRAINER_5,
    // End
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
Research_Tech* Research_getTechPointer(int r);
Research_Tech* Research_getDeveloping(int field);
void Research_startDeveloping(int r);

#endif

