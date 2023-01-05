#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "raylib.h"

#include "unit.h"
#include "fleet.h"
#include "world.h"
#include "ui.h"
#include "screen_manager.h"

#include "combat.h"

int framecounter = 0;

int main() 
{
    InitWindow(SCREENX, SCREENY, "4x COOL");
    SetTargetFPS(120);
    World_create();
    ScreenManager_init();

    // Create initial warp ship design available to player
    Fleet_create();

    Unit_Design* ud = Unit_createDesign();
    strcpy(ud->name, "Mothership");
    ud->componentnum = 6;
    ud->components[0] = Unit_getComponent(0);
    ud->components[1] = Unit_getComponent(1);
    ud->components[2] = Unit_getComponent(3);
    ud->components[3] = Unit_getComponent(5);
    ud->components[4] = Unit_getComponent(2);
    ud->components[5] = Unit_getComponent(11);
    // Build one to start them off with
    Unit_create(ud);
    
    Fleet_addUnit(Fleet_getPointer(0), Unit_lastAddedIndex());
    Fleet_getPointer(0)->lowpop = 25;
    Fleet_getPointer(0)->credits = 10000;

    Combat_createEnemyDesigns();

    // Main game loop
    while (!WindowShouldClose()) {
        if (framecounter == 0 && Combat_active()) {
            Combat_run();
            UI_updateEnabled();
        }
        BeginDrawing();
            ClearBackground(BLACK);
            UI_drawElements();
            if (IsMouseButtonPressed(0)) {
                UI_handleMouse();
            }
        EndDrawing();
        framecounter++;
        if (framecounter == 60) {
            framecounter = 0;
        }
    }

    CloseWindow();                 
    return 0;
}