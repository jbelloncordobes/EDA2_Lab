
#include "headers/common.h"
#include "headers/users.h"
#include "headers/csvusers.h"
#include "headers/csvfriends.h"

int main() {
    printf("Bienvenido a nuestra red social!\n");
    menu();
    return 0;
}

void menu(){
    //int userlength = 0;
    //user** Users = NULL;
    int serial = 0;
    char answer[2];
    int decision;
    nodelist *Users = malloc(sizeof(nodelist));
    Users->first = NULL;
    Users->last = NULL;
    read_users_csv(Users);

//Extras

    user* extra1 = malloc(sizeof(user));
    extra1->id = 1;
    strcpy(extra1->username,"ssss");
    extra1->sentfreq = NULL;malloc(sizeof(user));
    extra1->receivedfreq = NULL;malloc(sizeof(user));
    extra1->friendlist = NULL;malloc(sizeof(user));


    add_friend(extra1, Users->first->User);
    add_friend(Users->first->User, extra1);

    user* extra2 = malloc(sizeof(user));
    extra2->id = 2;
    strcpy(extra2->username,"dddd");
    extra2->sentfreq = NULL;malloc(sizeof(user));
    extra2->receivedfreq = NULL;malloc(sizeof(user));
    extra2->friendlist = NULL;malloc(sizeof(user));


    add_friend(extra2, Users->first->User);
    add_friend(Users->first->User, extra2);

    //

    write_friends_csv(Users);

    //

    while(TRUE){
        printf("Que quieres hacer?\n");
        printf("1. Insertar un nuevo usuario\n");
        printf("2. Listar todos los usuarios\n");
        printf("3. Operar como usuario\n");
        printf("4. Salir\n");

        fgets(answer, 2, stdin);
        sscanf(answer, "%d", &decision);


        switch (decision) {
            case 1: {
                user* newuser = createUser();
                //addUser(Users, newuser, &userlength);
                addUser(Users, newuser);
            }
            break;
            case 2:
                //listUsers(Users, userlength);
                listUsers(Users);
            break;
            case 4:
                write_users_csv(Users);
                write_friends_csv(Users);
                return;
            default:
                printf("Opcion invalida\n");

        }
        int c;
        while((c= getchar()) != '\n' && c != EOF) {};
    }
}

struct tm getCurrentDate(){
    struct tm currentdate;
    time_t current_t = time(NULL);
    currentdate = *localtime(&current_t);
    return currentdate;
}