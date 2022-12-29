#include "unit.h"
#include "fleet.h"
#include "ui.h"
#include "screen_manager.h"

void _drawInventoryList(UI_Element *el) {
    Fleet_Entity* f = Fleet_getPointer(ScreenManager_currentSector()->fleet);
    Unit_Entity* u = 0;
    int line = 0;
    for (int i = 0; i < f->unitmax; i++) {
        u = Unit_getPointer(f->units[i]);
        for (int j = 0; j < u->storednum; j++) {
            DrawText(
                TextFormat(
                    "%s - %d tons",
                    Sector_resourceStrings[u->stored[j].storedResourceId],
                    u->stored[j].quantity
                ),
                el->x, el->y + 16*line, 16, RAYWHITE
            );
            line++;
        }
    }
}

void ScreenCargo_init() {
    UI_createElement(0, 64, SCREENX, SCREENY, "Inventory list", SCREEN_CARGO, NOFUNC, _drawInventoryList, NOFUNC, NOFUNC);
}