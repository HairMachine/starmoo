#include "inventory.h"
#include "ui.h"

void _drawInventoryList(UI_Element *el) {
    Inventory_Entity *inv = 0;
    for (int i = 0; i < Inventory_count(); i++) {
        inv = Inventory_getPointer(i);
        DrawText(TextFormat("%s - %d tons", inv->name, inv->quantity), el->x, el->y + 16*i, 16, RAYWHITE);
    }
}

void ScreenCargo_init() {
    UI_createElement(0, 64, SCREENX, SCREENY, "Inventory list", SCREEN_CARGO, NOFUNC, _drawInventoryList, NOFUNC, NOFUNC);
}