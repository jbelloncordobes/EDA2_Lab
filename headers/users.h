//
// Created by Usuario on 13/05/2023.
//

#ifndef EDA2_LAB_USERS_H
#define EDA2_LAB_USERS_H

#include <Windows.h>

// Definitions
#define MAX_LENGTH 50
#define MAX_HOBBIES 5
#define MAX_USERS 10
// Structs
typedef struct user{
    int id;
    wchar_t username[MAX_LENGTH];
    wchar_t birthday[MAX_LENGTH];
    wchar_t email[MAX_LENGTH];
    wchar_t location[MAX_LENGTH];
    wchar_t hobbies[MAX_HOBBIES][MAX_LENGTH];
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
void OperateAs(HWND hwnd);
void addUser(nodelist* nlist, user* newuser);
#endif //EDA2_LAB_USERS_H
