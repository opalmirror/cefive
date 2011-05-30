#include "jumpstack.h"

int jumpstack_clear(JumpStack* prStack) {
    int i = 0;
    if (prStack == NULL) {
        return JUMPSTACK_NULLPTR;
    }
    for (i = 0; i < JUMPSTACK_SIZE; i++) {
        prStack->element[i] = 0;
    }
    prStack->elements = 0;
    return JUMPSTACK_SUCCESS;
}

int jumpstack_init(JumpStack* prStack) {
    if (prStack == NULL) {
        return JUMPSTACK_NULLPTR;
    }
    if (jumpstack_clear(prStack) != JUMPSTACK_SUCCESS) {
        return JUMPSTACK_FAILURE;
    }
    return JUMPSTACK_SUCCESS;
}

SceUInt32 jumpstack_pop(JumpStack* prStack) {
    SceUInt32 address = 0;
    if (prStack != NULL) {
        if (prStack->elements > 0) {
            address = prStack->element[prStack->elements - 1];
            prStack->elements--;
        }
    }
    return address;
}

int jumpstack_push(JumpStack* prStack, SceUInt32 address) {
    int i = 0;
    if (prStack == NULL) {
        return JUMPSTACK_NULLPTR;
    }
    if (prStack->elements + 1 >= JUMPSTACK_SIZE) {
        for (i = 1; i < JUMPSTACK_SIZE; i++) {
            prStack->element[i - 1] = prStack->element[i];
        }
        prStack->element[prStack->elements - 1] = address;
    } else {
        prStack->element[prStack->elements] = address;
        prStack->elements++;
    }
    
    return JUMPSTACK_SUCCESS;
}
