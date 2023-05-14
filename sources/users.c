//
// Created by Usuario on 13/05/2023.
//

#include "../headers/users.h"
#include "../headers/common.h"
#include <time.h>
#include <stdio.h>

struct tm getCurrentDate(){
    struct tm currentdate;
    time_t current_t = time(NULL);
    currentdate = *localtime(&current_t);
    return currentdate;
}

void addUser(nodelist* nlist, user* newuser){
    unode *n = malloc(sizeof(unode));
    n->User = newuser;
    if (nlist->first == NULL) {
        nlist->first = n;
    }
    else{
        nlist->last->next = n;
    }
    nlist->last = n;
    n->next = NULL;
}

void OperateAs(HWND hwnd, nodelist users) {
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Choose a user", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT, CW_USEDEFAULT, 248, 480, hwnd, NULL, NULL, NULL);

    unode *curruser = users.first;
    wchar_t name[MAX_LENGTH+5];
    wchar_t character = '#';

    int id;
    for (int i = 0; i < users.size; i++) {
        id = curruser->User->id;
        swprintf(name, sizeof(name) / sizeof(wchar_t), L"%ls%c%d", curruser->User->username, character, id);
        CreateWindowExW(0, L"Button", name, WS_VISIBLE | WS_CHILD | SS_CENTER, 8,
                        24 + i * 48 + i * 8, 200, 48, windowH,
                        (HMENU) SELECT_USER, NULL, NULL);

        curruser = curruser->next;
    }
}