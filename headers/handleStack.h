//
// Created by Usuario on 13/05/2023.
//

#ifndef EDA2_LAB_HANDLESTACK_H
#define EDA2_LAB_HANDLESTACK_H
#include <Windows.h>

typedef struct handleStack{
    int size;
    HWND* hlist;
} handleStack;

handleStack* initHandleStack();

void addHandle(handleStack*, HWND);

HWND popHandle(handleStack*);

#endif //EDA2_LAB_HANDLESTACK_H
