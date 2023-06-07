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
                    u->id = atoi(value); //First column = ID.
                    break;
                }
                case 1: {
                    strcpy(u->username, value); //First column = username.
                    break;
                }
                case 2: {
                    strcpy(u->birthday, value); //Second column = birthday.
                    break;
                }
                case 3: {
                    strcpy(u->email, value); //Third column == Email.
                    break;
                }
                case 4: {
                    strcpy(u->location, value); //Fourth column = Location.
                    break;
                }
                case 5: {
                    strcpy(u->hobbies[0], value); //Fifth column = Hobby 1.
                    break;
                }
                case 6: {
                    strcpy(u->hobbies[1], value); //Sixth column = Hobby 1.
                    break;
                }
                case 7: {
                    strcpy(u->hobbies[2], value); //Seventh column = Hobby 1.
                    break;
                }
                case 8: {
                    strcpy(u->hobbies[3], value); //Eighth column = Hobby 1.
                    break;
                }
                case 9: {
                    strcpy(u->hobbies[4], value); //Nineth column = Hobby 1.
                    break;
                }

            }
            value = strtok(NULL, ","); //Collect next value in row.
            column++; //Increase column.
        }
        u->friendlist = NULL; //Set internal list to NULL.
        u->sentfreq = NULL; //Set internal list to NULL.
        u->receivedfreq = NULL; //Set internal list to NULL.
        addUser(nlist, u); //Add to node list of all users.
    }
    fclose(fp);
}

void write_users_csv(nodelist *nlist){
    unode *n = nlist->first; //Create a node and assign it to the first one of the list.
    FILE *fp = fopen("../Files/users.csv", "w"); //Open the file.
    fprintf(fp, "id,username,bday,email,location,h1,h2,h3,h4,h5\n"); //Print first line of the file.
    while (n!=NULL){
        user *u = n->User; //Check user info and print it on the file.
        fprintf(fp, "%i,%s,%s,%s,%s,%s,%s,%s,%s,%s", u->id, u->username, u->birthday, u->email, u->location, u->hobbies[0], u->hobbies[1], u->hobbies[2], u->hobbies[3], u->hobbies[4]);
        n = n->next; //Go to next node.
    }
    fclose(fp);
}
