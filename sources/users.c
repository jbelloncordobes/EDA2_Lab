//
// Created by Usuario on 13/05/2023.
//

#include "../headers/users.h"
#include "../headers/common.h"
#include <time.h>
#include <stdio.h>

struct tm getCurrentDate() {
    struct tm currentdate;
    time_t current_t = time(NULL);
    currentdate = *localtime(&current_t);
    return currentdate;
}

void addUser(nodelist *nlist, user *newuser) {
    unode *n = malloc(sizeof(unode));
    n->User = newuser;
    if (nlist->first == NULL) {
        nlist->first = n;
    } else {
        nlist->last->next = n;
    }
    nlist->last = n;
    n->next = NULL;
}

user *findUser(nodelist nlist, wchar_t usernameid[]) {
    wchar_t username[MAX_LENGTH];
    int id = -1;
    swscanf(usernameid, L"%[^#]#%d", username, &id);

    unode *curruser = nlist.first;
    for (int i = 0; i < nlist.size; i++) {
        if (curruser->User->id == id) {
            return curruser->User;
        }
        curruser = curruser->next;
    }
    return NULL;

}

int sendFriendRequest(nodelist nlist, wchar_t friend_nameid[], wchar_t user_nameid[]) {
    user *friend = findUser(nlist, friend_nameid);
    user *user = findUser(nlist, user_nameid);
    if (friend == NULL) {
        return FALSE;
    }

//    friend->friend_request_sent
    add_to_queue(&user->friend_requests_sent, friend);
    add_to_queue(&friend->friend_requests_received,user);
    return TRUE;
}

void add_to_queue(user_queue* queue, user* addeduser) {
    if (queue->size == 0) {
        queue->users = malloc(sizeof(user*));
    } else {
        queue->users = realloc(queue->users, sizeof(user*) * (queue->size + 1));
    }

    queue->users[queue->size] = addeduser;
    queue->size += 1;
}

void OperateAs(HWND hwnd, nodelist users) {
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Choose a user", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT, CW_USEDEFAULT, 248, 480, hwnd, NULL, NULL, NULL);

    unode *curruser = users.first;
    wchar_t name[MAX_LENGTH + 5];
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