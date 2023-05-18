//
// Created by Usuario on 13/05/2023.
//

#include "../headers/handleStack.h"

handleStack* initHandleStack(){
    handleStack* Stack = malloc(sizeof(struct handleStack));
    Stack->size = 0;
    Stack->hlist = NULL;
    return Stack;
}


void addHandle(handleStack* stack, HWND window){
    if (stack->hlist == NULL){
        stack->hlist = malloc(sizeof(HWND));
    } else {
       stack->hlist =  realloc(stack->hlist, sizeof(HWND)*(stack->size + 1));
    }

    stack->hlist[stack->size] = window;
    stack->size += 1;
}

HWND popHandle(handleStack* stack){
    if (stack->size == 0){
        return NULL;
    }
    stack-> size -= 1;
    // No realloc porque se hace ya al añadir algo
    return stack->hlist[stack->size];
}

