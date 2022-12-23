#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "raylib.h"

#include "unit.h"
#include "fleet.h"
#include "world.h"
#include "ui.h"

int main() 
{
    InitWindow(SCREENX, SCREENY, "4x COOL");
    SetTargetFPS(60);
    World_create();
    UI_initialise();

    // Create initial warp ship design available to player
    Fleet_create();

    Unit_Design* ud = Unit_createDesign();
    strcpy(ud->name, "Mothership");
    ud->passengerBays = 5;
    ud->shipSize = 4;
    ud->fuelTanks = 5;
    ud->warpDriveLevel = 1;
    ud->researchLabs = 5;
    ud->constructionBays = 1;
    ud->storageBays = 5;
    ud->farmingBays = 2;
    ud->miningRigs = 1;
    // Build one to start them off with
    Unit_create(ud);
    
    Fleet_addUnit(Fleet_getPointer(0), Unit_count() - 1);
    Fleet_getPointer(0)->lowpop = 25;
    Fleet_getPointer(0)->food = 250;
    Fleet_getPointer(0)->drink = 250;
    Fleet_getPointer(0)->oxygen = 750;
    Fleet_getPointer(0)->fuel = 5;

    // Some other basic designs for transport and supply ships
    
    ud = Unit_createDesign();
    strcpy(ud->name, "Habitat Pod");
    ud->passengerBays = 1;
    ud->farmingBays = 1;

    ud = Unit_createDesign();
    strcpy(ud->name, "Research Vessel");
    ud->researchLabs = 1;

    ud = Unit_createDesign();
    strcpy(ud->name, "Hauler");
    ud->storageBays = 1;
    ud->fuelTanks = 1;

    ud = Unit_createDesign();
    strcpy(ud->name, "Constructor");
    ud->constructionBays = 1;

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