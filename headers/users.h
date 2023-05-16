//
// Created by Usuario on 13/05/2023.
//

#ifndef EDA2_LAB_USERS_H
#define EDA2_LAB_USERS_H

#include <Windows.h>

// Definitions
#define MAX_LENGTH 100
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
    struct user** friend_requests_received; // Cola
    int frr_size;
    struct user** friend_requests_sent; // Cola
    int frs_size;
    struct user** friends;
    int f_size;
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
user* findUser(nodelist nlist, wchar_t usernameid[]);
void OperateAs(HWND, nodelist);
void addUser(nodelist* nlist, user* newuser);
void add_to_queue(user **userarray, int *size, user* addeduser);
int sendFriendRequest(nodelist nlist, wchar_t friend_nameid[], wchar_t user_nameid[]);
#endif //EDA2_LAB_USERS_H
