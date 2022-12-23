// ==== ORDERS.C ====

#define ORDER_MAX 32

typedef enum {
    ORDER_NONE, ORDER_MOVE_FLEET
} Order_Type;

typedef struct {
    int originX;
    int originY;
    int targetX;
    int targetY;
    Order_Type type;
    int fleet;
} Order_Entity;

Order_Entity* Order_create();
Order_Entity Order_getCopy(int index);
Order_Entity* Order_getPointer(int index);
void Order_complete(int index);
int Order_count();