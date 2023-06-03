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

typedef struct userlist{
    struct user** list;
    int size;
} userlist;

typedef struct userduolist{
    struct userduo** list;
    int size;
}userduolist;

typedef struct userduo{
    struct user* duo[2];
}userduo;

typedef struct user{
    int id;
    char username[MAX_LENGTH];
    char birthday[MAX_LENGTH];
    char email[MAX_LENGTH];
    char location[MAX_LENGTH];
    char hobbies[MAX_HOBBIES][MAX_LENGTH];
    struct userlist* friendlist;
    struct userlist* sentfreq;
    struct userlist* receivedfreq;
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



//Friends
void erase_freq(userlist*, user*);
void add_friend(user*, user*);
void send_friend_req(user*, user*);
// confirm_friend(user*, user*);
//int is_in_duo_list(userduolist*, user*, user*);
//void add_to_duo_list(userduolist *, userduo *);
int is_in_list(userlist*, int);
void receive_friend_req(user*, user*);

#endif //PLANB_USERS_H
