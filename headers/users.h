//
// Created by Usuario on 13/05/2023.
//

#ifndef EDA2_LAB_USERS_H
#define EDA2_LAB_USERS_H

#include <Windows.h>
#include "common.h"

// Definitions
#define MAX_LENGTH 100
#define POST_LENGTH 250
#define MAX_HOBBIES 5

// Structs
// user_array se define aquí porque da problemas para leerlo bien en su header
typedef struct user_array{
    struct user** users;
    int size;
}user_array;

typedef struct Post{
    wchar_t message[POST_LENGTH];
    struct Post* next;
}Post;

// PostQueue se define aquí porque da problemas para leerlo bien en su header
typedef struct PostQueue{
    Post* first;
    Post* last;
    int current;
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

typedef struct Node{
    user *User;
    struct Node *next;
} unode;

typedef struct NodeList{
    unode *first;
    unode *last;
    int size;
} nodelist;

// Forward Declarations
struct tm getCurrentDate();
user* getUser(nodelist, wchar_t[]);
void operateAs(HWND, nodelist);
void addUser(nodelist*, user*);

void sendFRModal(HWND hwnd, nodelist, user);
void receivedFRModal(HWND hwnd, user*);
void sentFRModal(HWND hwnd, user*);
#endif //EDA2_LAB_USERS_H
