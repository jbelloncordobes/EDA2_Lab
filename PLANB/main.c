
#include "headers/common.h"
#include "headers/users.h"

int main() {
    printf("Bienvenido a nuestra red social!\n");
    menu();
    return 0;
}

void menu(){
    //int userlength = 0;
    //user** Users = NULL;
    nodelist *Users = NULL;
    int serial = 0;
    char answer[2];
    int decision;

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