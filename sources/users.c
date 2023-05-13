//
// Created by Usuario on 13/05/2023.
//

#include "../headers/users.h"
#include <time.h>

struct tm getCurrentDate(){
    struct tm currentdate;
    time_t current_t = time(NULL);
    currentdate = *localtime(&current_t);
    return currentdate;
}

void addUser(nodelist* nlist, user* newuser){
    unode *n = malloc(sizeof(unode));
    n->User = newuser;
    if (nlist->first == NULL) {
        nlist->first = n;
    }
    else{
        nlist->last->next = n;
    }
    nlist->last = n;
    n->next = NULL;
}