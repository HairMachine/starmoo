#include "inventory.h"

Inventory_Entity inventory[512] = {};
int inventorynum = 0;

Inventory_Entity* Inventory_create() {
    Inventory_Entity* inv = &inventory[inventorynum];
    inventorynum++;
    return inv;
}

Inventory_Entity* Inventory_getPointer(int index) {
    return &inventory[index];
}

void Inventory_remove(int index) {
    for (int i = index; i < inventorynum; i++) {
        inventory[i] = inventory[i + 1];
    }
    inventorynum--;
}

int Inventory_volume() {
    int total = 0;
    for (int i = 0; i < inventorynum; i++) {
        total += inventory[i].quantity;
    }
    return total;
}