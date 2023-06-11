
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



    int serial = 0;
    char answer[2];
    int decision;
    nodelist *Users = malloc(sizeof(nodelist));
    Users->first = NULL;
    Users->last = NULL;
    read_users_csv(Users);
    read_friends_csv(Users);
    user *active_user = NULL;

    while(TRUE){
        if (active_user == NULL){ //If the operator has not selected a user.
            printf("Que quieres hacer?\n");
            printf("1. Insertar un nuevo usuario\n");
            printf("2. Listar todos los usuarios\n");
            printf("3. Operar como usuario\n");
            printf("4. Eliminar usuario\n");
            printf("5. Salir\n");




            fgets(answer, 2, stdin);
            sscanf(answer, "%d", &decision);

            switch (decision) {
                case 1: { //Create user.
                    user* newuser = createUser(); //Call function to create user from command prompt.
                    addUser(Users, newuser); //Add it to the list containing all users.
                }
                    break;
                case 2: //List users.
                    listUsers(Users, active_user); //Print every user's username.
                    break;
                case 3: { //Operate as user.
                    if (Users->first == NULL){
                        printf("No hay usuarios.\n");
                        break;
                    }
                    int selected = FALSE; //Variable to end loop.
                    while (selected == FALSE){ //Loop.
                        char answer2[MAX_LENGTH]; //Variable to store the answer.
                        unode *currentnode = Users->first; //Create node to store the current node inspected.
                        printf("Usuarios disponibles:\n");
                        listUsers(Users, active_user); //List all users available.
                        printf("Escribe el nombre del usuario deseado:\n");
                        scanf("%s", answer2);
                        while(currentnode!=NULL){
                            if (strcmp(currentnode->User->username, answer2) == 0){
                                selected = TRUE; //Desired player has been found.
                                active_user = currentnode->User; //Set desired user as active user.
                                break;
                            }
                            currentnode = currentnode->next; //Go to next node to check.
                        }
                    }
                    break;
                }
                case 4: { //Erase user.
                    int selected = FALSE; //Variable to end loop.
                    char erase_name[MAX_LENGTH]; //Variable to copy name to make sure it's the right one.
                    while (selected == FALSE) {
                        char answer2[MAX_LENGTH]; //Variable to store the answer.
                        unode *currentnode = Users->first; //Create node to store the current node inspected.
                        printf("Usuarios disponibles:\n");
                        listUsers(Users, active_user); //List all users available.
                        printf("Escribe el nombre del usuario deseado:\n");
                        scanf("%s", answer2);
                        while (currentnode != NULL) {
                            if (strcmp(currentnode->User->username, answer2) == 0) {
                                selected = TRUE; //Desired player has been found.
                                strcpy(erase_name, currentnode->User->username); //Copy the user's name.
                                eraseUser(Users, erase_name); //Erase user.
                                break;
                            }
                            currentnode = currentnode->next; //Go to next node to check.
                        }
                    }
                    break;
                }
                case 5: //Update files
                    write_users_csv(Users);
                    write_friends_csv(Users);
                    return;
                default: //Invalid option
                    printf("Opcion invalida\n");
            }
            int c;
            while((c= getchar()) != '\n' && c != EOF) {};
        }
        else{ //If the operator has already selected a user.
            printf("\nQue quieres hacer?\n");
            printf("1. Anadir post (funcion no implementada)\n");
            printf("2. Enviar solicitud de amistad\n");
            printf("3. Aceptar solicitud de amistad\n");
            printf("4. Ignorar solicitud de amistad\n");
            printf("5. Listar amigos\n");
            printf("6. Listar solicitudes de amistad\n");
            printf("7. Cerrar sesion\n");

            fgets(answer, 2, stdin);
            sscanf(answer, "%d", &decision);

            switch (decision) {
                case 1: { //New post.
                    break;
                }
                case 2: { //Send friend request.
                    char answer2[MAX_LENGTH]; //Variable to store the answer.
                    unode *currentnode = Users->first; //Create node to store the current node inspected.
                    user *receiver = NULL; //Receiver variable for the friend request.
                    printf("Usuarios disponibles:\n");
                    listUsers(Users, active_user); //List all users available.
                    printf("Escribe el nombre del usuario deseado:\n");
                    scanf("%s", answer2);
                    if(strcmp(answer2, active_user->username) == 0)
                        printf("No puedes enviarte una solicitud de amistad a ti mismo.\n"); //If the name written is your own name.
                    else{ //Any other case.
                        while (currentnode != NULL){ //Search until last nodelist item.
                            if (strcmp(currentnode->User->username, answer2) == 0) { //If current node's username coincides with input.
                                receiver = currentnode->User; //Set current node's user as receiver.
                                break;
                            }
                            currentnode = currentnode->next; //Go to next node to check.
                        }
                        if (receiver != NULL){ //If the name is found.
                            send_friend_req(active_user, receiver); //Send.
                            receive_friend_req(active_user, receiver); //Receive.
                        }
                        else printf("No se ha podido enviar la solicitud.\n");
                    }
                    break;
                }

                case 3: { //Accept friend request.
                    int selected = FALSE; //Variable to end loop.
                    while (selected == FALSE){
                        char answer2[MAX_LENGTH]; //Variable to store the answer.
                        unode *currentnode = Users->first; //Create node to store the current node inspected.
                        if(active_user->receivedfreq == NULL){ //If received friend requests list is empty.
                            printf("No tienes solicitudes de amistad!\n");
                            break;
                        }
                        printf("Estas son tus solicitudes de amistad recibidas:\n");
                        list_userlist(active_user->receivedfreq); //List all users available.
                        printf("Escribe el nombre del usuario deseado:\n");
                        scanf("%s", answer2);
                        while(currentnode!=NULL){ //Search until last nodelist item.
                            if (strcmp(currentnode->User->username, answer2) == 0){ //If current node's username coincides with input.
                                selected = TRUE; //Desired player has been found.
                                add_friend(currentnode->User, active_user); //Add friend from both sides.
                                printf("Solicitud de amistad aceptada!\n");
                                break;
                            }
                            currentnode = currentnode->next; //Go to next node to check.
                        }
                    }
                    break;
                }
                case 4:{ //Ignore friend request
                    int selected = FALSE; //Variable to end loop.
                    while (selected == FALSE){
                        char answer2[MAX_LENGTH]; //Variable to store the answer.
                        unode *currentnode = Users->first; //Create node to store the current node inspected.
                        if(active_user->receivedfreq == NULL){ //If received friend requests list is empty.
                            printf("No tienes solicitudes de amistad!\n");
                            break;
                        }
                        printf("Estas son tus solicitudes de amistad recibidas:\n");
                        list_userlist(active_user->receivedfreq); //List all users available.
                        printf("Escribe el nombre del usuario deseado:\n");
                        scanf("%s", answer2);
                        while(currentnode!=NULL){ //Search until last nodelist item.
                            if (strcmp(currentnode->User->username, answer2) == 0){ //If current node's username coincides with input.
                                selected = TRUE; //Desired player has been found.
                                erase_freq(active_user->receivedfreq, currentnode->User); //Ignore friend request from both sides.
                                erase_freq(currentnode->User->sentfreq, active_user);
                                printf("Solicitud de amistad ignorada >:(\n");
                                break;
                            }
                            currentnode = currentnode->next; //Go to next node to check.
                        }
                    }
                    break;
                }
                case 5: //List friends.
                    if (active_user->friendlist == NULL) printf("No tienes amigos :(\n");
                    else {
                        printf("Estos son tus amigos:\n");
                        list_userlist(active_user->friendlist);
                    }
                    break;
                case 6: //List friend requests.
                    if (active_user->sentfreq == NULL) printf("No has enviado ninguna solicitud de amistad!\n");
                    else {
                        printf("Estas son tus solicitudes de amistad enviadas:\n");
                        list_userlist(active_user->sentfreq);
                    }
                    if (active_user->receivedfreq == NULL) printf("No has recibido ninguna solicitudes de amistad!\n");
                    else {
                        printf("Estas son tus solicitudes de amistad recibidas:\n");
                        list_userlist(active_user->receivedfreq);
                    }
                    break;
                case 7: //Log out.
                    active_user = NULL;
                    break;
                default: //Invalid option.
                    printf("Opcion invalida\n");

            }
            int c;
            while((c= getchar()) != '\n' && c != EOF) {}; //Clear buffer to ask for input again.
        }


    }
}

struct tm getCurrentDate(){ //Get current date.
    struct tm currentdate;
    time_t current_t = time(NULL);
    currentdate = *localtime(&current_t);
    return currentdate;
}