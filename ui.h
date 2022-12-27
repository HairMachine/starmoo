#include "raylib.h"
#include "sector.h"

#define SCREENX 800
#define SCREENY 600
#define NOFUNC 0
#define UI_tileSize 16

typedef enum {
    SCREEN_ALL, SCREEN_MAP, SCREEN_SYSTEM, SCREEN_FLEET, SCREEN_RESEARCH,
    SCREEN_ESPIONAGE, SCREEN_DIPLOMACY, SCREEN_INFRASTRUCTURE, SCREEN_BUILD_MILITARY
} UI_Screen;

typedef struct UI_Element UI_Element;

struct UI_Element {
    int x;
    int y;
    int width;
    int height;
    char btnText[32];
    UI_Screen screen;
    int enabled;
    void (*enableFunc)(UI_Element* el);
    void (*drawFunc)(UI_Element* el);
    void (*clickFunc)(UI_Element* el, Vector2 mpos);
    void (*targeterFunc)(UI_Element* el, Sector_Entity* clickedSector);
};

typedef struct {
    int range;
    int x;
    int y;
    UI_Element triggeredBy;
} UI_Targeter;

void UI_createElement(int x, int y, int width, int height, char* btnText, UI_Screen screen, void (*enableFunc), void (*drawFunc), void (*clickFunc), void (*targeterFunc));
void UI_drawElements();
void UI_handleMouse();
void UI_drawButton(UI_Element* el);
UI_Screen UI_getScreen();
void UI_enableScreen(UI_Screen s);
void UI_updateEnabled();
void UI_drawPanel(UI_Element* el);
int UI_handleSelectList(UI_Element* el, Vector2 mpos, int numelements, int listsize);
void UI_drawSelectListItem(UI_Element* el, int offset, int listsize, char text[32], int isSelected);
