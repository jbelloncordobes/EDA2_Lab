//
// Created by u215109 on 16/05/2023.
//

#include "../headers/common.h"
#include "../headers/csvusers.h"
#include "../headers/users.h"


void read_users_csv(nodelist *nlist){
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

        while (value){ //While it reads something
            switch(column){
                case 0: {
                    u->id = atoi(value);
                    break;
                }
                case 1: {
                    strcpy(u->username, value);
                    break;
                }
                case 2: {
                    strcpy(u->birthday, value);
                    break;
                }
                case 3: {
                    strcpy(u->email, value);
                    break;
                }
                case 4: {
                    strcpy(u->location, value);
                    break;
                }
                case 5: {
                    strcpy(u->hobbies[0], value);
                    break;
                }
                case 6: {
                    strcpy(u->hobbies[1], value);
                    break;
                }
                case 7: {
                    strcpy(u->hobbies[2], value);
                    break;
                }
                case 8: {
                    strcpy(u->hobbies[3], value);
                    break;
                }
                case 9: {
                    strcpy(u->hobbies[4], value);
                    break;
                }

            }
            value = strtok(NULL, ",");
            column++;
        }
        addUser(nlist, u);

    }
    fclose(fp);
    //fopendkamkd
    //fsacnf
    //createuserparams(fwwwd,dkwkwk,ddldlwwl,dlddldll)
    //adduser()
}

void write_users_csv(nodelist *nlist){

    unode *n = nlist->first; //Create a node and assign it to the first one of the list.
    FILE *fp = fopen("../Files/users.csv", "w"); //Open the file.
    fprintf(fp, "id,username,bday,email,location,h1,h2,h3,h4,h5\n"); //Print first line of the file.
    while (n!=NULL){
        user *u = n->User; //Check user info and print it on the file.
        fprintf(fp, "%i,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", u->id, u->username, u->birthday, u->email, u->location, u->hobbies[0], u->hobbies[1], u->hobbies[2], u->hobbies[3], u->hobbies[4]);
        n = n->next; //Go to next node.
    }

    fclose(fp);
}
