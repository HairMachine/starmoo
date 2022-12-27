typedef struct {
    int storedGoodId;
    int storedResourceId;
    int storedComponentId;
    int storingUnitId;
    int quantity;
} Inventory_Entity;

Inventory_Entity* Inventory_create();
Inventory_Entity* Inventory_getPointer(int index);
void Inventory_remove(int index);
int Inventory_volume();
int Inventory_count();
