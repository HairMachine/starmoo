#include <math.h>

#include "ui.h"
#include "research.h"
#include "screen_manager.h"

void _drawResearchOptions(UI_Element* el) {
    int line = 0;
    Research_Tech* rt = 0;
    for (int i = 0; i < RT_ALL; i++) {
        rt = Research_getTechPointer(i);
        if (rt->discovered && !rt->developed) {
            DrawText(TextFormat("%s (%s)", rt->name, Research_fieldStrings[rt->field]), el->x, el->y + line*16, 16, RAYWHITE);
            line++;
        }
    }
    DrawText("Currently developing:", el->x + 400, el->y, 32, RAYWHITE);
    for (int f = 0; f < FIELD_ALL; f++) {
        DrawText(TextFormat("%s: %s", Research_fieldStrings[f], Research_getDeveloping(f)->name), el->x + 400, el->y + 48 + f*16, 16, RAYWHITE);
    }
}

void _clickResearchOptions(UI_Element* el, Vector2 mpos) {
    int clicked = floor((mpos.y - el->y) / 16);
    Research_Tech* rt = 0;
    for (int i = 0; i < RT_ALL; i++) {
        rt = Research_getTechPointer(i);
        if (rt->discovered && !rt->developed) {
            clicked--;
            if (clicked == -1) {
                Research_startDeveloping(i);
            }
        }
    }
}

void ScreenResearch_init() {
    UI_createElement(32, 32, SCREENX, SCREENY, "Research Options", SCREEN_RESEARCH, NOFUNC, _drawResearchOptions, _clickResearchOptions, NOFUNC);
}