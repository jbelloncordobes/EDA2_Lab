//
// Created by Usuario on 13/05/2023.
//

#include "../headers/users.h"
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

user* getUser(nodelist nlist, wchar_t usernameid[]) {
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



void operateAs(HWND hwnd, nodelist users) {
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

void sendFRModal(HWND hwnd, nodelist users, user active_user) {
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Enviar solicitud de amistad", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT, CW_USEDEFAULT, 248, 480, hwnd, NULL, NULL, NULL);

    unode *curruser = users.first;
    wchar_t name[MAX_LENGTH + 5];
    wchar_t character = '#';

    int id;
    int i = 0;
    while (curruser != NULL) {
        id = curruser->User->id;
        if (search_User(active_user.friends,id) || search_User(active_user.friend_requests_sent,id) || search_User(active_user.friend_requests_received,id) || active_user.id == curruser->User->id){
            curruser = curruser->next;
            continue;
        }

        swprintf(name, sizeof(name) / sizeof(wchar_t), L"%ls%c%d", curruser->User->username, character, id);
        CreateWindowExW(0, L"Button", name, WS_VISIBLE | WS_CHILD | SS_CENTER, 8,
                        24 + i * 48 + i * 8, 200, 48, windowH,
                        (HMENU) FR_BUTTON, NULL, NULL);

        curruser = curruser->next;
        i++;
    }
}

void receivedFRModal(HWND hwnd, user* active_user){
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Solicitudes recibidas", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT, CW_USEDEFAULT, 248, 480, hwnd, NULL, NULL, NULL);


    wchar_t name[MAX_LENGTH + 5];
    wchar_t character = '#';

    int id;
    for (int i = 0; i < active_user->friend_requests_received.size; i++){
        id = active_user->friend_requests_received.users[i]->id;
        swprintf(name, sizeof(name) / sizeof(wchar_t), L"%ls%c%d", active_user->friend_requests_received.users[i]->username, character, id);
        CreateWindowExW(0, L"Button", name, WS_VISIBLE | WS_CHILD | SS_CENTER, 8,
                        24 + i * 48 + i * 8, 200, 48, windowH,
                        (HMENU) RFR_BUTTON, NULL, NULL);
    }
}

void sentFRModal(HWND hwnd, user* active_user){
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Solicitudes enviadas", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT, CW_USEDEFAULT, 248, 480, hwnd, NULL, NULL, NULL);


    wchar_t name[MAX_LENGTH + 5];
    wchar_t character = '#';

    int id;
    for (int i = 0; i < active_user->friend_requests_sent.size; i++){
        id = active_user->friend_requests_sent.users[i]->id;
        swprintf(name, sizeof(name) / sizeof(wchar_t), L"%ls%c%d", active_user->friend_requests_sent.users[i]->username, character, id);
        CreateWindowExW(0, L"Button", name, WS_VISIBLE | WS_CHILD | SS_CENTER, 8,
                        24 + i * 48 + i * 8, 200, 48, windowH,
                        (HMENU) SFR_BUTTON, NULL, NULL);
    }
}