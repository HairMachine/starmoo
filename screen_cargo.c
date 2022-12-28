#include "unit.h"
#include "fleet.h"
#include "inventory.h"
#include "ui.h"
#include "screen_manager.h"

void _drawInventoryList(UI_Element *el) {
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    Unit_Entity* u = 0;
    Inventory_Entity* inv = 0;
    int line = 0;
    for (int i = 0; i < f->unitmax; i++) {
        u = Unit_getPointer(f->units[i]);
        for (int j = 0; j < u->storednum; j++) {
            inv = Inventory_getPointer(u->stored[j]);
            DrawText(TextFormat("%s - %d tons", inv->name, inv->quantity), el->x, el->y + 16*line, 16, RAYWHITE);
            line++;
        }
    }
}

void ScreenCargo_init() {
    UI_createElement(0, 64, SCREENX, SCREENY, "Inventory list", SCREEN_CARGO, NOFUNC, _drawInventoryList, NOFUNC, NOFUNC);
}