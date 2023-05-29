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
typedef struct user_array{
    struct user** users;
    int size;
}user_array;


typedef struct user{
    int id;
    wchar_t username[MAX_LENGTH];
    wchar_t birthday[MAX_LENGTH];
    wchar_t email[MAX_LENGTH];
    wchar_t location[MAX_LENGTH];
    wchar_t hobbies[MAX_HOBBIES][MAX_LENGTH];
    struct user_array friend_requests_received; // Cola
    struct user_array friend_requests_sent; // Cola
    struct user_array friends;
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
void add_to_array(user_array* array, user* addeduser);
void remove_from_array(user_array* array, user* addeduser);
void sendFriendRequest(user*, user*);
void acceptFriendRequest(user*, user*);
void rejectFriendRequest(user*, user*);
int search_User(user_array, int);
void SendFRModal(HWND hwnd, nodelist users, user active_user);
void receivedFRModal(HWND hwnd, user* active_user);
void sentFRModal(HWND hwnd, user* active_user);
#endif //EDA2_LAB_USERS_H
