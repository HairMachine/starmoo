#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "raylib.h"

#include "unit.h"
#include "fleet.h"
#include "world.h"
#include "ui.h"
#include "screen_manager.h"

int main() 
{
    InitWindow(SCREENX, SCREENY, "4x COOL");
    SetTargetFPS(120);
    World_create();
    Unit_initComponents();
    ScreenManager_init();

    // Create initial warp ship design available to player
    Fleet_create();

    Unit_Design* ud = Unit_createDesign();
    strcpy(ud->name, "Mothership");
    ud->componentnum = 4;
    ud->components[0] = Unit_getComponent(0);
    ud->components[1] = Unit_getComponent(1);
    ud->components[2] = Unit_getComponent(3);
    ud->components[3] = Unit_getComponent(5);
    // Build one to start them off with
    Unit_create(ud);
    
    Fleet_addUnit(Fleet_getPointer(0), Unit_count() - 1);
    Fleet_getPointer(0)->lowpop = 25;
    Fleet_getPointer(0)->fuel = 100;

    // Some crappy base designs for testing
    ud = Unit_createDesign();
    strcpy(ud->name, "Mining Vessel");
    ud->componentnum = 2;
    ud->components[0] = Unit_getComponent(4);
    ud->components[1] = Unit_getComponent(2);

    ud = Unit_createDesign();
    strcpy(ud->name, "Cargo Ship");
    ud->componentnum = 2;
    ud->components[0] = Unit_getComponent(2);
    ud->components[1] = Unit_getComponent(2);

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            UI_drawElements();
            if (IsMouseButtonPressed(0)) {
                UI_handleMouse();
            }
        EndDrawing();
    }

    CloseWindow();                 
    return 0;
}