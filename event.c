#include <string.h>

#include "event.h"

Event_Entity events[16];
int eventnum = 0;

void Event_create(char* title, const char* body) {
    Event_Entity* e = &events[eventnum];
    strcpy(e->title, title);
    strcpy(e->body, body);
    eventnum++;
}

void Event_remove(int index) {
    for (int i = index; i < eventnum ; i++) {
        events[i] = events[i + 1];
    }
    eventnum--;
    if (eventnum < 0) eventnum = 0;
}

void Event_clearList() {
    eventnum = 0;
}

Event_Entity* Event_getPointer(int index) {
    return &events[index];
}

int Event_count() {
    return eventnum;
}