#include "ui.h"
#include "combat.h"

int pline = 0;
int eline = 0;

void _enableOkBtn(UI_Element* el) {
    el->visible = !Combat_active();
}

void _clickOkBtn(UI_Element* el, Vector2 mpos) {
    UI_enableScreen(SCREEN_MAP);
}

void _drawCombat(UI_Element* el) {
    pline = 0;
    eline = 0;
    for (int i = 0; i < Combat_unitCount(); i++) {
        Combat_Unit c = Combat_getUnitCopy(i);
        if (c.side == PLAYER_SIDE) {
            DrawText(TextFormat("%s %d", c.entity.name, c.entity.hp), el->x, el->y + pline*16, 16, RAYWHITE);
            pline++;
        } else if (c.side == ENEMY_SIDE) {
            DrawText(TextFormat("%s %d", c.entity.name, c.entity.hp), el->x + 400, el->y + eline*16, 16, RAYWHITE);
            eline++;
        }
    }
}

void ScreenCombat_init() {
    UI_createElement(500, 100, 100, 32, "OK", SCREEN_COMBAT, _enableOkBtn, UI_drawButton, _clickOkBtn, NOFUNC);
    UI_createElement(0, 64, SCREENX, SCREENY, "Combat", SCREEN_COMBAT, NOFUNC, _drawCombat, NOFUNC, NOFUNC);
}