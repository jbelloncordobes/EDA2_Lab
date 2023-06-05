//
// Created by Usuario on 13/05/2023.
//

#include "../headers/users.h"
#include "../headers/user_arrays.h"
#include <time.h>
#include <stdio.h>

// Recibe la fecha actual
struct tm getCurrentDate() {
    struct tm currentdate;
    time_t current_t = time(NULL);
    currentdate = *localtime(&current_t);
    return currentdate;
}

// Agrega un usuario a la lista de usuarios
void addUser(UserList *nlist, user *newuser) {
    // Crea un nodo para el usuario
    UserNode *n = malloc(sizeof(UserNode));
    n->User = newuser;
    // Si la lista está vacía lo agrega como primer usuario
    if (nlist->first == NULL) {
        nlist->first = n;
    } else {
        // Asigna al next del último el nuevo usuario
        nlist->last->next = n;
    }
    // Pone el nuevo usuario al final de la lista
    nlist->last = n;
    n->next = NULL;
    nlist->size += 1;
}

// Devuelve un usuario de la lista
user* getUser(UserList nlist, wchar_t usernameid[]) {
    // Recoge el número del texto del botón presionado
    wchar_t username[MAX_LENGTH];
    int id = -1;
    swscanf(usernameid, L"%[^#]#%d", username, &id);

    // Busca al usuario con la id especificada y lo devuelve
    UserNode *curruser = nlist.first;
    for (int i = 0; i < nlist.size; i++) {
        if (curruser->User->id == id) {
            return curruser->User;
        }
        curruser = curruser->next;
    }
    // En caso de que no lo encuentre
    return NULL;

}


// Crea una lista con botones para elegir el usuario activo
void operateAs(HWND hwnd, UserList users) {
    // Crea la ventana que contendrá los botones
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Choose a user", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT, CW_USEDEFAULT, 248, 480, hwnd, NULL, NULL, NULL);

    UserNode *curruser = users.first;
    wchar_t name[MAX_LENGTH + 5];
    wchar_t character = '#';
    int id;

    // Itera la lista de usuarios y crea los botones
    for (int i = 0; i < users.size; i++) {
        id = curruser->User->id;
        // Junta el nombre de usuario con la id para el texto del botón y lo crea
        swprintf(name, sizeof(name) / sizeof(wchar_t), L"%ls%c%d", curruser->User->username, character, id);
        CreateWindowExW(0, L"Button", name, WS_VISIBLE | WS_CHILD | SS_CENTER, 8,
                        24 + i * 48 + i * 8, 200, 48, windowH,
                        (HMENU) SELECT_USER, NULL, NULL);
        // Pasa al siguiente usuario
        curruser = curruser->next;
    }
}

// Crea una lista con botones para mandar solicitudes de amistad
void sendFRModal(HWND hwnd, UserList users, user active_user) {
    // Crea la ventana que contendrá los botones
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Enviar solicitud de amistad", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT, CW_USEDEFAULT, 248, 480, hwnd, NULL, NULL, NULL);

    UserNode *curruser = users.first;
    wchar_t name[MAX_LENGTH + 5];
    wchar_t character = '#';

    int id;
    int i = 0;
    // Itera todos los usuarios y crea sus botones si cumplen las condiciones.
    while (curruser != NULL) {
        id = curruser->User->id;
        // Se salta al usuario si está en la lista de amigos, peticiones recibidas o enviadas o si es el usuario activo
        if (search_User(active_user.friends,id) || search_User(active_user.friend_requests_sent,id) || search_User(active_user.friend_requests_received,id) || active_user.id == curruser->User->id){
            curruser = curruser->next;
            continue;
        }

        // Junta el nombre de usuario con la id para el texto del botón y lo crea
        swprintf(name, sizeof(name) / sizeof(wchar_t), L"%ls%c%d", curruser->User->username, character, id);
        CreateWindowExW(0, L"Button", name, WS_VISIBLE | WS_CHILD | SS_CENTER, 8,
                        24 + i * 48 + i * 8, 200, 48, windowH,
                        (HMENU) FR_BUTTON, NULL, NULL);
        // Pasa al siguiente usuario
        curruser = curruser->next;
        i++;
    }
}

// Crea una lista con botones para gestionar las solicitudes de amistad recibidas
void receivedFRModal(HWND hwnd, user* active_user){
    // Crea la ventana que contendrá los botones
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Solicitudes recibidas", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT, CW_USEDEFAULT, 248, 480, hwnd, NULL, NULL, NULL);


    wchar_t name[MAX_LENGTH + 5];
    wchar_t character = '#';
    int id;

    // Itera la lista de solicitudes de amistad recibidas
    for (int i = 0; i < active_user->friend_requests_received.size; i++){
        id = active_user->friend_requests_received.users[i]->id;
        // Junta el nombre de usuario con la id para el texto del botón y lo crea
        swprintf(name, sizeof(name) / sizeof(wchar_t), L"%ls%c%d", active_user->friend_requests_received.users[i]->username, character, id);
        CreateWindowExW(0, L"Button", name, WS_VISIBLE | WS_CHILD | SS_CENTER, 8,
                        24 + i * 48 + i * 8, 200, 48, windowH,
                        (HMENU) RFR_BUTTON, NULL, NULL);
    }
}

// Crea una lista con botones para gestionar las solicitudes de amistad enviadas
void sentFRModal(HWND hwnd, user* active_user){
    // Crea la ventana que contendrá los botones
    HWND windowH = CreateWindowExW(0, L"DialogWindow", L"Solicitudes enviadas", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT, CW_USEDEFAULT, 248, 480, hwnd, NULL, NULL, NULL);


    wchar_t name[MAX_LENGTH + 5];
    wchar_t character = '#';
    int id;

    // Itera todos los usuarios y crea sus botones si cumplen las condiciones.
    for (int i = 0; i < active_user->friend_requests_sent.size; i++){
        id = active_user->friend_requests_sent.users[i]->id;
        // Junta el nombre de usuario con la id para el texto del botón y lo crea
        swprintf(name, sizeof(name) / sizeof(wchar_t), L"%ls%c%d", active_user->friend_requests_sent.users[i]->username, character, id);
        CreateWindowExW(0, L"Button", name, WS_VISIBLE | WS_CHILD | SS_CENTER, 8,
                        24 + i * 48 + i * 8, 200, 48, windowH,
                        (HMENU) SFR_BUTTON, NULL, NULL);
    }
}