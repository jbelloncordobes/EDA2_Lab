//
// Created by Usuario on 13/05/2023.
//

#ifndef EDA2_LAB_USERS_H
#define EDA2_LAB_USERS_H

#include <Windows.h>
#include "common.h"

// Definitions
#define MAX_LENGTH 100
#define POST_LENGTH 500
#define MAX_HOBBIES 5

// Structs
// muchas estructuras se definen aquí porque dan problemas para leerlas bien en los headers en los que les tocaría ir
typedef struct user_array{
    struct user** users;
    int size;
}user_array;

typedef struct Post{
    wchar_t message[POST_LENGTH];
    struct Post* next;
}Post;

// PostQueue es una cola
typedef struct PostQueue{
    Post* first;
    Post* last;
    int current; // Referido a menudo como "puntero" en comentarios
}PostQueue;

typedef struct user{
    int id;
    wchar_t username[MAX_LENGTH];
    wchar_t birthday[MAX_LENGTH];
    wchar_t email[MAX_LENGTH];
    wchar_t location[MAX_LENGTH];
    wchar_t hobbies[MAX_HOBBIES][MAX_LENGTH];
    user_array friend_requests_received; // Cola
    user_array friend_requests_sent; // Cola
    user_array friends;
    PostQueue posts;
} user;

typedef struct UserNode{
    user *User;
    struct UserNode *next;
} UserNode;

// Lista de usuarios
typedef struct UserList{
    UserNode *first;
    UserNode *last;
    int size;
} UserList;

// Forward Declarations
struct tm getCurrentDate();
user* getUser(UserList, wchar_t[]);
void operateAs(HWND, UserList);
void addUser(UserList*, user*);

void sendFRModal(HWND hwnd, UserList, user);
void receivedFRModal(HWND hwnd, user*);
void sentFRModal(HWND hwnd, user*);
#endif //EDA2_LAB_USERS_H
