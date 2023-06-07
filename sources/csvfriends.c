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
    FILE *fp = fopen("../Files/friends.csv", "r"); //Open the file in read mode
    if (fp == NULL) { //In case there is no user file
        printf("No hay usuarios.\n");
        return;
    }

    while (fgets(buffer, 1024, fp)){ //If there is a file

        column = 0; //Starting column for each row
        row++; //Starting row of the file
        if (row == 1) continue; //If row is the first one, pass
        char* value = strtok(buffer, ","); //Check value
        char type[MAX_CHARACTERS]; //Type of friend variable.
        unode *current = nlist->first; //First node of the node list.
        user *u1;
        user *u2;
        int id1;
        int id2;
        while (value){ //While it reads something
            switch(column){
                case 0: {
                    strcpy(type, value); //First column = Type.
                    break;
                }
                case 1: {
                    id1 = atoi(value); //First column = ID 1.
                    break;
                }
                case 2: {
                    id2 = atoi(value); //First column = ID 2.
                    break;
                }
            }
            value = strtok(NULL, ","); //Check next value in row.
            column++; //Increase column.
        }

        //Take the desired 2 users
        for (int i = 0; i<nlist->size;i++){ //Go through entire list.
            if (current->User->id == id1) u1 = current->User; //If ID 1 is found.
            if (current->User->id == id2) u2 = current->User; //If ID 2 is found.
            current = current->next; //Go to next node.
        }
        //Case friend:
        if (strcmp(type, "Friend") == 0){
            if (is_in_list(u1->friendlist, id2) == FALSE){ //If not already in friend list
                add_friend(u1, u2);
            }
            /*if (is_in_list(u2->friendlist, id1) == FALSE){
                add_friend(u2, u1);
            }*/
        }
            //Case friend request:
        else if (strcmp(type, "Friend Request") == 0){
            if (is_in_list(u1->sentfreq, id2) == FALSE){ //If not already in sent friend requests list
                send_friend_req(u1, u2);
            }
            if (is_in_list(u2->receivedfreq, id1) == FALSE){ //If not already in received friend requests list
                receive_friend_req(u1, u2);
            }
        }
    }
    fclose(fp);

}
void write_friends_csv(nodelist *nlist){
    FILE *f = fopen("../Files/friends.csv", "w");
    user *u;
    unode *n = nlist->first; //First node of the list.
    fprintf(f, "Type,Sender,Receiver"); //Print header.

    while(n!=NULL){ //Go through entire node list.
        u = n->User; //User variable.
        user *friend;
        if (u->friendlist != NULL){ //If it has friend list.
            for (int i = 0; i<u->friendlist->size; i++){ //Go through entire sent friend request list.
                friend = u->friendlist->list[i]; //Check friend info.
                fprintf(f, "\nFriend,%d,%d", friend->id,u->id);
            }
        }
        if (u->sentfreq != NULL){ //If it has sent friend request list.
            for (int i = 0;i<u->sentfreq->size; i++){ //Go through entire sent friend request list.
                friend = u->sentfreq->list[i]; //Check friend info.
                fprintf(f, "\nFriend Request,%d,%d", u->id, friend->id);
            }
        }
        /*
        if (u->receivedfreq != NULL){
            for (int i = 0; i<u->receivedfreq->size; i++){
                friend = u->receivedfreq->list[i];
                fprintf(f, "\nFriend Request,%d,%d", friend->id,u->id);
            }
        }
        */
        n = n->next; //Go to next node.
    }
    fclose(f);
}


int is_in_list(userlist* nlist, int id){ //Search in user lists (dynamic arrays of users).
    if (nlist == NULL) return FALSE; //If there is no list.
    for (int i = 0; i<nlist->size; i++){ //Go through entire list.
        if (nlist->list[i]->id == id) return TRUE; //If ID found.
    }
    return FALSE; //Any other case.
}