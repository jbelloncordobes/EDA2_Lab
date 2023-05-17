//
// Created by u215109 on 16/05/2023.
//

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
            /*switch(column){
                case 0: {
                    u->id = atoi(value);
                }
                case 1: {
                    strcpy(u->username, value);
                }
                case 2: {
                    strcpy(u->birthday, value);
                }
                case 3: {
                    strcpy(u->email, value);
                }
                case 4: {
                    strcpy(u->username, value);
                }
                case 5: {
                    strcpy(u->hobbies[0], value);
                }
                case 6: {
                    strcpy(u->hobbies[1], value);
                }
                case 7: {
                    strcpy(u->hobbies[2], value);
                }
                case 8: {
                    strcpy(u->hobbies[3], value);
                }
                case 9: {
                    strcpy(u->hobbies[4], value);
                }

            }*/
            if (column == 0){ //ID
                u->id = atoi(value);
            }
            else if (column == 1){ //Name
                strcpy(u->username, value);
            }
            else if (column == 2){ //BD
                strcpy(u->birthday, value);
            }
            else if (column == 3){ //Email
                strcpy(u->email, value);
            }
            else if (column == 4){ //Location
                strcpy(u->location, value);
            }
            else if (column == 5){ //Hobby 1
                strcpy(u->hobbies[0], value);
            }
            else if (column == 6){ //Hobby 2
                strcpy(u->hobbies[1], value);
            }
            else if (column == 7){ //Hobby 3
                strcpy(u->hobbies[2], value);
            }
            else if (column == 8){ //Hobby 4
                strcpy(u->hobbies[3], value);
            }
            else if (column == 9){ //Hobby 5
                strcpy(u->hobbies[4], value);
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

