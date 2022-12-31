// ==== ORDERS.C ====

#define ORDER_MAX 32

typedef enum {
    ORDER_NONE, ORDER_MOVE_FLEET, ORDER_BUILD_SHIP
} Order_Type;

typedef struct {
    int originX;
    int originY;
    int targetX;
    int targetY;
    Order_Type type;
    int fleet;
    int param1;
    int param2;
    int completed;
} Order_Entity;

Order_Entity* Order_create();
Order_Entity Order_getCopy(int index);
Order_Entity* Order_getPointer(int index);
void Order_complete(int index);
void Order_completeAll();
int Order_count();