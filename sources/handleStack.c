//
// Created by Usuario on 13/05/2023.
//

#include "../headers/handleStack.h"

// Inicializa el stack de handles
handleStack* initHandleStack(){
    handleStack* Stack = malloc(sizeof(struct handleStack));
    Stack->size = 0;
    Stack->hlist = NULL;
    return Stack;
}

// Agrega un handle al stack
void addHandle(handleStack* stack, HWND window){
    // Si la lista no está definida (no tiene ningún elemento)
    if (stack->hlist == NULL){
        // Asigna memoria para un elemento
        stack->hlist = malloc(sizeof(HWND));
    } else {
        // Asigna memoria para un elemento más
       stack->hlist =  realloc(stack->hlist, sizeof(HWND)*(stack->size + 1));
    }

    // Agrega el elemento al stack y aumenta en 1 el tamaño
    stack->hlist[stack->size] = window;
    stack->size += 1;
}

// Devuelve y elimina el último elemento del Stack.
HWND popHandle(handleStack* stack){
    // No devuelve nada si el stack no tiene ningún elemento
    if (stack->size == 0){
        return NULL;
    }
    // Resta 1 al tamaño del stack
    stack-> size -= 1;
    // Devuelve el último elemento
    // Cuando se vuelva a cargar un elemento en el stack ya se hará un realloc/sobreescribirá el último elemento
    return stack->hlist[stack->size];

}

