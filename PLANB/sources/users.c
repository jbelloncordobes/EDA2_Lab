//
// Created by Usuario on 01/05/2023.
//

#include "../headers/common.h"
#include "../headers/users.h"

user* createUser(){
    char username[MAX_LENGTH];
    char email[MAX_LENGTH];
    char birthday[MAX_LENGTH];
    char location[MAX_LENGTH];
    char hobbies[5][MAX_LENGTH];

    username:
    printf("\nNombre del usuario: \n");
    scanf("%s", username);
    if (strlen(username) < 1){
        printf("El nombre de usuario no puede estar vacio\n");
        goto username;
    }

    email:
    printf("\nEmail: \n");
    scanf("%s", email);
    for(int i = 0; i < MAX_LENGTH; i++){
        if (email[i] == '@'){
            break;
        }
        if (email[i] == 0) {
            printf("Formato de email incorrecto");
            goto email;
        }
    }
    int year;
    int month;
    int day;
    struct tm time = getCurrentDate();
    birthday:
    printf("\nCumpleanyos\n");
    printf("\nAno:\n");
    scanf("%d", &year);
    if (year > time.tm_year+1900 || year < 1900){
        printf("Introduce un ano valido");
        goto birthday;
    }
    printf("\nMes (en numero):\n");
    scanf("%d", &month);
    if (month > 12 || month < 1){
        printf("Mes invalido");
        goto birthday;
    }
    printf("\nDia:\n");
    scanf("%d", &day);
    if (day > 28){
        if (month == 2){
            if (!(year % 4 && (year % 100 != 0 || year % 400 == 0))){
                printf("Día invalido");
                goto birthday;
            }
        }
    }
    if (day == 31){
        if (month == 2 || month == 4 || month == 6 || month == 9 || month == 11){
            printf("Día invalido");
            goto birthday;
        }
    }

    char buffer[21];

    itoa(year, buffer, 10);
    strcpy(birthday, (const char *) &buffer);
    strcat(birthday, "/");

    itoa(month, buffer, 10);
    strcat(birthday, (const char *) &buffer);
    strcat(birthday, "/");

    itoa(day, buffer, 10);
    strcat(birthday, (const char *) &buffer);

    location:
    printf("\nUbicacion:\n");
    scanf("%s", location);

    hobbies:
    printf("\nEscribe tus 5 gustos o preferencias\n");
    for (int i = 0; i < 5; i++){
        printf("%d:\n", i+1);
        scanf("%s", hobbies[i]);
    }

    user* newuser = malloc(sizeof(user));
    strcpy(newuser->username, username);
    strcpy(newuser->email, email);
    strcpy(newuser->birthday, birthday);
    strcpy(newuser->location, location);
    for(int i = 0; i < 5; i++){
        strcpy(newuser->hobbies[i], hobbies[i]);
    }


    printf("\nUsuario Creado\n");
    return newuser;
}

void addUser(user** array, user* newuser, int *userlength){
    if (array == NULL){
        array = malloc(sizeof(user));
    } else {
        array = realloc(array, sizeof(user) * (*userlength + 1));
    }
    array[*userlength] = newuser;
    *userlength += 1;
}

void listUsers(user** array, int userlength){
    printf("Estos son todos los usuarios: \n");
    for(int i = 0; i < userlength; i++){
        printf("%s\n", array[i]->username);
    }
    printf("\n");
}