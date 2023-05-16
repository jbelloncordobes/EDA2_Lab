//
// Created by u215109 on 16/05/2023.
//

#include "../headers/csvusers.h"



void read_users_csv(){
    int row = 0;
    int column = 0;
    char buffer[1024]; //Create a string buffer for every row
    FILE *fp = fopen("Files/users", "r"); //Open the file in read mode
    if (fp!= NULL) { //In case there is no user file
        printf("No hay usuarios.");
        return;
    }

    while (fgets(buffer, 1024, fp)){ //If there is a file
        column = 0; //Starting column for each row
        row++; //Starting row of the file
        if (row == 1) continue; //If row is the first one, pass
        char* value = strtok(buffer, ", "); //Check value

        while (value){ //While it reads something

        }

    }
    //fopendkamkd
    //fsacnf
    //createuserparams(fwwwd,dkwkwk,ddldlwwl,dlddldll)
    //adduser()
}

