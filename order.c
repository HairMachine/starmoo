#include "order.h"

Order_Entity Order_list[ORDER_MAX];
int ordernum = 0;

Order_Entity* Order_create() {
    Order_Entity* o = &Order_list[ordernum];
    o->completed = 0;
    ordernum++;
    return o;
}

Order_Entity Order_getCopy(int index) {
    return Order_list[index];
}

Order_Entity* Order_getPointer(int index) {
    return &Order_list[index];
}

void Order_complete(int index) {
    for (int i = index; i < ordernum - 1; i++) {
        Order_list[i] = Order_list[i + 1];
    }
    ordernum--;
}

void Order_completeAll() {
    for (int i = ordernum - 1; i >= 0; i--) {
        if (Order_list[i].completed) {
            Order_complete(i);
        }
    }
}

int Order_count() {
    return ordernum;
}