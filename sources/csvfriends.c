//
// Created by Dídac on 03/06/2023.
//

#include "../headers/csvfriends.h"
#include "../headers/common.h"
#include "../headers/users.h"

void read_friends_csv(nodelist *nlist){
    int row = 0;
    int column = 0;
    char buffer[1024]; //Create a string buffer for every row
    FILE *fp = fopen("../Files/users.csv", "r"); //Open the file in read mode
    if (fp == NULL) { //In case there is no user file
        printf("No hay usuarios.\n");
        return;
    }

    while (fgets(buffer, 1024, fp)){ //If there is a file
        user *u = malloc(sizeof(user));

        column = 0; //Starting column for each row
        row++; //Starting row of the file
        if (row == 1) continue; //If row is the first one, pass
        char* value = strtok(buffer, ", "); //Check value
        char type[MAX_CHARACTERS];
        unode *current = nlist->first;
        user *u1;
        user *u2;
        int id1;
        int id2;
        while (value){ //While it reads something
            switch(column){
                case 0: {
                    strcpy(type, value);
                    break;
                }
                case 1: {
                    id1 = atoi(value);
                    break;
                }
                case 2: {
                    id2 = atoi(value);
                    break;
                }
            }
            value = strtok(NULL, ",");
            column++;
        }

        //Take the desired 2 users
        for (int i = 0; i<nlist->size;i++){
            if (current->User->id == id1) u1 = current->User;
            if (current->User->id == id2) u2 = current->User;
            current = current->next;
        }
        //Case friend:
        if (strcmp(type, "Friend") == 0){
            if (is_in_list(u1->friendlist, id2) == FALSE){ //If not already in friend list
                add_friend(u1, u2);
            }
            if (is_in_list(u2->friendlist, id1) == FALSE){
                add_friend(u2, u1);
            }
        }
        //Case friend request:
        else if (strcmp(type, "Friend Request") == 0){
            if (is_in_list(u1->sentfreq, id2) == FALSE){//If not already in sent friend requests list
                send_friend_req(u1, u2);
            }
            if (is_in_list(u1->sentfreq, id2) == FALSE){//If not already in received friend requests list
                receive_friend_req(u1, u2);
            }
        }
    }
    fclose(fp);

}
void write_friends_csv(nodelist *nlist){
    FILE *f = fopen("../Files/friends.csv", "w");
    user *u;
    unode *n = nlist->first;
    fprintf(f, "Type,Sender,Receiver\n");

    //userduolist *flist = NULL;
    //userduolist *frlist = NULL;

    while(n!=NULL){
        u = n->User;
        user *friend;
        for (int i = 0; i<u->friendlist->size; i++){
            friend = u->friendlist->list[i];
            fprintf(f, "Friend,%d,%d", friend->id,u->id);
            /*if (is_in_duo_list(flist, u, friend) == FALSE){
                userduo *ud = malloc(sizeof(userduo));
                ud->duo[0] = u;
                ud->duo[1] = friend;
                add_to_duo_list(flist, ud);
            }*/
        }
        for (int i = 0; i<u->receivedfreq->size; i++){
            friend = u->receivedfreq->list[i];
            fprintf(f, "Friend Request,%d,%d", friend->id,u->id);
            /*if (is_in_duo_list(frlist, u, friend) == FALSE){
                userduo *ud = malloc(sizeof(userduo));
                ud->duo[0] = u;
                ud->duo[1] = friend;
                add_to_duo_list(flist, ud);
            }*/
        }
        for (int i = 0; i<u->sentfreq->size; i++){
            friend = u->sentfreq->list[i];
            fprintf(f, "Friend Request,%d,%d", friend->id,u->id);
            /*if (is_in_duo_list(frlist, u, friend) == FALSE){
                userduo *ud = malloc(sizeof(userduo));
                ud->duo[0] = u;
                ud->duo[1] = friend;
                add_to_duo_list(flist, ud);
            }*/
        }
        n = n->next;
    }
    fclose(f);
}

/*int is_in_duo_list(userduolist* list, user* u1, user* u2){
    if(list == NULL) return FALSE;
    for (int i = 0; i<list->size;i++){
        if ((list->list[i]->duo[0] == u1 && list->list[i]->duo[1] == u2)||(list->list[i]->duo[1] == u1 && list->list[i]->duo[0] == u2)) return TRUE;
    }
    return FALSE;
}*/

/*
void add_to_duo_list(userduolist *udlist, userduo *ud){
    if (udlist == NULL){
        udlist = malloc(sizeof(userduolist));
        udlist->list = malloc(sizeof (userduo));
        udlist->list[0] = ud;
        udlist->size = 1;
    }
    else{
        udlist->list = realloc(udlist->list, sizeof (userduo)*udlist->size+1);
        udlist->list[udlist->size] = ud;
        udlist->size+=1;
    }
}*/
int is_in_list(userlist* nlist, int id){
    for (int i = 0; i<nlist->size; i++){
        if (nlist->list[i]->id == id) return TRUE;
    }
    return FALSE;
}