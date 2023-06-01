//
// Created by Usuario on 01/05/2023.
//

#ifndef PLANB_USERS_H
#define PLANB_USERS_H
#define MAX_LENGTH 50
#define MAX_HOBBIES 5

typedef struct queue{
    struct user** users;
    int size;
}user_queue;

typedef struct user{
    int id;
    char username[MAX_LENGTH];
    char birthday[MAX_LENGTH];
    char email[MAX_LENGTH];
    char location[MAX_LENGTH];
    char hobbies[MAX_HOBBIES][MAX_LENGTH];
    struct user** friendlist;
    struct user** freqlist;
} user;

typedef struct Node{
    user *User;
    struct Node *next;
    struct Node *prev;
}unode;

typedef struct NodeList{
    unode *first;
    unode *last;
    int size;
    int serial;
}nodelist;

user* createUser();
//void addUser(user**, user*, int*);
void addUser(nodelist*, user*);
void eraseUser(nodelist*, char*);

//void listUsers(user**, int);
void listUsers(nodelist*);



#endif //PLANB_USERS_H
