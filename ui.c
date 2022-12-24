#include <math.h>
#include <string.h>
#include "raylib.h"

#include "event.h"
#include "order.h"
#include "unit.h"
#include "fleet.h"
#include "world.h"
#include "ui.h"

UI_Element nullElement = {0, 0, 0, 0, 0, 0, 0};
UI_Element elements[255];
int elementEnd = 0;

void UI_createElement(int x, int y, int width, int height, char* btnText, UI_Screen screen, void (*enableFunc), void (*drawFunc), void (*clickFunc), void (*targeterFunc)) {
    UI_Element* el = &elements[elementEnd];
    el->x = x;
    el->y = y;
    el->width = width;
    el->height = height;
   strcpy(el->btnText, btnText);
    el->screen = screen;
    el->enableFunc = enableFunc; 
    el->drawFunc = drawFunc;
    el->clickFunc = clickFunc;
    el->targeterFunc = targeterFunc;
    el->enabled = 1;
    elementEnd++;
}

UI_Element UI_getElementByPosition(int x, int y) {
    for (int e = 0; e < elementEnd; e++) {
        UI_Element* el = &elements[e];
        if (!el->enabled) {
            continue;
        }
        if (x >= el->x && y >= el->y && x <= el->x + el->width && y <= el->y + el->height) {
            return elements[e];
        }
    }
    return nullElement;
}

UI_Screen currentScreen;

void UI_isCurrentScreenEnable(UI_Element* el) {
    if (currentScreen == el->screen || el->screen == SCREEN_ALL) {
        el->enabled = 1;
    } else {
        el->enabled = 0;
    }
}

void UI_enableScreen(UI_Screen screen) {
    currentScreen = screen;
    for (int e = 0; e < elementEnd; e++) {
        UI_isCurrentScreenEnable(&elements[e]);
    }
}

void UI_updateEnabled() {
    for (int e = 0; e < elementEnd; e++) {
        if (elements[e].enableFunc) {
            elements[e].enableFunc(&elements[e]);
        }
    }
}

int showEventPanel = 0;

// Click and draw handler functions

void UI_drawButton(UI_Element* el) {
    DrawRectangle(el->x, el->y, el->width, el->height, DARKBLUE);
    DrawRectangleLines(el->x, el->y, el->width, el->height, RAYWHITE);
    DrawText(el->btnText, el->x + 5, el->y + 5, 16, RAYWHITE);
}

void UI_clickFleetTab(UI_Element *el, Vector2 mpos) {
    UI_enableScreen(SCREEN_FLEET);
}

void UI_drawElements() {
    // Draw in reverse priority order
    for (int e = elementEnd - 1; e >= 0; e--) {
        if (elements[e].enabled) {
            elements[e].drawFunc(&elements[e]);
        }
    }
}

void UI_checkSpecialEvents() {
    if (Event_count() == 0) {
        return;
    }
    Event_Entity* e = Event_getPointer(0);
    if (strcmp(e->title, "System Discovered") == 0) {
        UI_enableScreen(SCREEN_SYSTEM);
        return;
    }
}

void UI_handleMouse() {
    Vector2 mpos = GetMousePosition();
    UI_Element el = UI_getElementByPosition((int)mpos.x, (int)mpos.y);
    if (el.clickFunc) {
        el.clickFunc(&el, mpos);
    }
    UI_checkSpecialEvents();
    UI_updateEnabled();
}

UI_Screen UI_getScreen() {
    return currentScreen;
}