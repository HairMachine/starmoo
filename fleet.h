
#define FLEET_BASIC_NEEDS f->lowpop + f->medpop + f->highpop + f->ultrapop
#define FLEET_STANDARD_NEEDS f->medpop + f->highpop + f->ultrapop
#define FLEET_HIGH_NEEDS f->highpop + f->ultrapop
#define FLEET_ULTRAHIGH_NEEDS f->ultrapop

typedef struct {
    int fuel;
    int fuelMax;
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
} Fleet_Entity;

Fleet_Entity* Fleet_create();
void Fleet_addUnit(Fleet_Entity* f, int uid);
void Fleet_removeUnitAtIndex(Fleet_Entity* f, int index);
Fleet_Entity* Fleet_getPointer(int index);
void Fleet_destroy(int index);
int Fleet_count();
int Fleet_getUnitCountByDesign(Fleet_Entity* f, int designId);
void Fleet_generateResources(Fleet_Entity* f);
int Fleet_createUnrest(Fleet_Entity* f, int need, int notBasic);
void Fleet_unrestChange(Fleet_Entity* f);
int Fleet_excessDeaths(Fleet_Entity* f);
void Fleet_consumeResources(Fleet_Entity* f);
int Fleet_newPop(Fleet_Entity* f, float birthChance);
void Fleet_changePop(Fleet_Entity* f, int excessDeaths);
void Fleet_simulate(Fleet_Entity* f);
int Fleet_canMine(Fleet_Entity *f);
int Fleet_getUsedStorage(Fleet_Entity *f);
int Fleet_getMaxStorage(Fleet_Entity *f);

