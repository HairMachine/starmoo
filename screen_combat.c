#include "ui.h"
#include "combat.h"
#include "world.h"

int pline = 0;
int eline = 0;
int begun = 0;

void _enableOkBtn(UI_Element* el) {
    el->visible = begun && !Combat_active();
}

void _clickOkBtn(UI_Element* el, Vector2 mpos) {
    if (Combat_shipsAlive(PLAYER_SIDE)) {
        World_changeLegitimacy(50);
    } else {
        World_changeLegitimacy(-50);
    }
    UI_enableScreen(SCREEN_MAP);
    begun = 0;
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

void _enableCombatOptions(UI_Element* el) {
    el->visible = !begun;
}

void _drawCombatOptions(UI_Element* el) {
    UI_drawPanel(el);
    DrawText("What will you do?", el->x, el->y, 16, RAYWHITE);
}

void _clickFightButton(UI_Element* el, Vector2 mpos) {
    begun = 1;
    Combat_start();
}

void _clickRunButton(UI_Element* el, Vector2 mpos) {
    World_changeLegitimacy(-25);
    UI_enableScreen(SCREEN_MAP);
}

void ScreenCombat_init() {
    UI_createElement(200, 532, 100, 32, "Run", SCREEN_COMBAT, _enableCombatOptions, UI_drawButton, _clickRunButton, NOFUNC);
    UI_createElement(100, 532, 100, 32, "Fight", SCREEN_COMBAT, _enableCombatOptions, UI_drawButton, _clickFightButton, NOFUNC);
    UI_createElement(100, 500, 600, 100, "Combat options", SCREEN_COMBAT, _enableCombatOptions, _drawCombatOptions, NOFUNC, NOFUNC);
    UI_createElement(500, 400, 100, 32, "OK", SCREEN_COMBAT, _enableOkBtn, UI_drawButton, _clickOkBtn, NOFUNC);
    UI_createElement(0, 64, SCREENX, SCREENY, "Combat", SCREEN_COMBAT, NOFUNC, _drawCombat, NOFUNC, NOFUNC);
}