typedef struct {
    char title[32];
    char body[256];
} Event_Entity;

void Event_create(char* title, const char* body);
void Event_remove(int index);
void Event_clearList();
Event_Entity* Event_getPointer(int index);
int Event_count();