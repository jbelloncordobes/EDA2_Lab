//
// Created by Usuario on 01/05/2023.
//

#include "../headers/common.h"
#include "../headers/users.h"

user* createUser(){
    user* newuser = malloc(sizeof(user));
    username:
    printf("\nNombre del usuario: \n");
    scanf("%s", newuser->username);
    if (strlen(newuser->username) < 1){
        printf("El nombre de usuario no puede estar vacio\n");
        goto username;
    }

    email:
    printf("\nEmail: \n");
    scanf("%s", newuser->email);
    for(int i = 0; i < MAX_LENGTH; i++){
        if (newuser->email[i] == '@'){
            break;
        }
        if (newuser->email[i] == 0) {
            printf("Formato de email incorrecto");
            goto email;
        }
    }
    int year;
    int month;
    int day;
    struct tm time = getCurrentDate();

    printf("\nCumpleanyos\n");
    printf("\nAno:\n");
    yyear:
    while (scanf("%d", &year)!=1){
        printf("Introduce un ano valido:\n");
        fflush(stdin);
    }

    if (year > time.tm_year+1900 || year < 1900){
        printf("Introduce un ano valido:\n");
        goto yyear;
    }

    printf("Mes (en numero):\n");
    mmonth:
    while (scanf("%d", &month)!=1){
        printf("Introduce un mes valido:\n");
        fflush(stdin);
    }
    if (month > 12 || month < 1){
        printf("Introduce un mes valido:");
        goto mmonth;
    }

    printf("Dia (en numero):\n");
    dday:
    while (scanf("%d", &day)!=1){
        printf("Introduce un dia valido:\n");
        fflush(stdin);
    }
    if (day > 28){
        if (month == 2){
            if ((year % 4 && (year % 100 != 0 || year % 400 == 0)) || day > 29){
                printf("Introduce un dia valido:\n");
                goto dday;
            }
        }
    }
    if (day == 31){
        if (month == 2 || month == 4 || month == 6 || month == 9 || month == 11){
            printf("Introduce un dia valido:\n");
            goto dday;
        }
    }

    char buffer[21];

    itoa(year, buffer, 10);
    strcpy(newuser->birthday, (const char *) &buffer);
    strcat(newuser->birthday, "/");

    itoa(month, buffer, 10);
    strcat(newuser->birthday, (const char *) &buffer);
    strcat(newuser->birthday, "/");

    itoa(day, buffer, 10);
    strcat(newuser->birthday, (const char *) &buffer);

    location:
    printf("\nUbicacion:\n");
    scanf("%s", newuser->location);

    hobbies:
    printf("\nEscribe tus 5 gustos o preferencias\n");
    for (int i = 0; i < 5; i++){
        printf("%d:\n", i+1);
        scanf("%s", newuser->hobbies[i]);
    }

    newuser->friendlist = NULL;
    newuser->sentfreq = NULL;
    newuser->receivedfreq = NULL;

    printf("\nUsuario Creado\n");
    return newuser;
}

void addUser(nodelist* nlist, user* newuser){
    unode *n = malloc(sizeof(unode)); //Create node variable.
    n->User = newuser; //Set node's user to the newuser.
    if (nlist->first == NULL) { //If there is no nodes in the list.
        nlist->first = n; //Put it in first place.
        nlist->size = 1; //Starting size for the list.
        nlist->serial = 0; //Starting serial. This is the ID that will be given to the next user created.
        n->prev = NULL; //No prev since it's the first node.
    }
    else{
        nlist->last->next = n; //Set node as next to the last node in the list.
        nlist->size += 1; //Increase size.
        n->prev = nlist->last; //Last element on the list is previous to the new last.
    }
    n->User->id = nlist->serial; //Give ID to the new user.
    nlist->serial+=1; //Increase serial.
    nlist->last = n; //Set node as last.
    n->next = NULL; //No next node since it's last.
}

void eraseUser(nodelist *nlist, char* name){
    unode *n = nlist->first; //For the first iteration of the search.
    while (n!= NULL){ //Until last node in the list.
        if (strcmp(n->User->username, name) == 0){ //If the name given is the same as the iterated user's name.
            if (n!= nlist->first){ //If the node is not the first one.
                n->prev->next = n->next; //It's previous next node will be it's next node.
            }
            if (n!= nlist->last){ //If it is not the last node.
                n->next->prev = n->prev;
            }
            /*
            if (n == nlist->first && n == nlist->last){ //If it's the only node in the list.
            }
            */
            //Erase user from other users' lists.
            if (n->User->friendlist != NULL){
                for (int i = 0; n->User->friendlist->size; i++){
                    erase_freq(n->User->friendlist->list[i]->friendlist, n->User); //Erase from friend's friend list.
                }
            }
            if (n->User->receivedfreq != NULL){
                for (int i = 0; n->User->receivedfreq->size; i++){
                    erase_freq(n->User->receivedfreq->list[i]->sentfreq, n->User); //Erase from other user's friend request list.
                }
            }
            if (n->User->sentfreq != NULL){
                for (int i = 0; n->User->sentfreq->size; i++){
                    erase_freq(n->User->sentfreq->list[i]->receivedfreq, n->User); //Erase from other user's friend request list.
                }
            }
            n = NULL;
            free(n); //Free space of the erased node.
            nlist->size-=1;
            return;
        }
        n = n->next; //Go to next node.
    }
}

void listUsers(nodelist* nlist, user* u){
    printf("Estos son todos los usuarios: \n");
    unode *n = nlist->first; //Select first node.
    while(n!=NULL){ //To go through all the list.
        if (n->User != u){ //Check no not active user.
            printf("%s\n", n->User->username);
        }
        n = n->next; //Go to next node.
    }
    printf("\n");
}

void send_friend_req(user* sender, user* receiver){
    if (is_in_list(sender->friendlist, receiver->id) == TRUE){ //If receiver is already friend.
        printf("Este usuario ya es tu amigo\n");
        return;
    }

    if (sender->sentfreq != NULL){ //If there is a sent friend requests list.
        for (int i=0;i < sender->sentfreq->size;i++){ //Through all the list.
            if(sender->sentfreq->list[i] == receiver){ //If the receiver is already on list.
                printf("Ya has enviado una solicitud a este usuario\n");
                return;
            }
        }
        sender->sentfreq->list = realloc(sender->sentfreq->list, sizeof(user)*sender->sentfreq->size+1); //Allocate space for the new friend.
        sender->sentfreq->list[sender->sentfreq->size] = sender; //Set friend in the list.
        sender->sentfreq->size+=1; //Increase list size.
    }
    else{ //New sent friend request list.

        sender->sentfreq = malloc(sizeof(userlist)); //Create list.
        sender->sentfreq->list = malloc(sizeof(user)); //Create list.
        sender->sentfreq->list[0] = receiver; //Set receiver.
        sender->sentfreq->size=1; //Set list size.
    }
    printf("La solicitud de amistad ha sido enviada\n");
}

/*

void confirm_friend(user* sender, user* receiver){
    //Llamar a add_friend desde las dos perspectivas (sender y receiver).
    add_friend(sender, receiver);
    add_friend(receiver, sender);
}
*/

void receive_friend_req(user* sender, user* receiver){

    if (is_in_list(sender->friendlist, receiver->id) == TRUE) return; //If receiver is already friend.

    if (receiver->receivedfreq != NULL){ //If there is a received friend requests list.
        for (int i=0;i < receiver->receivedfreq->size;i++){ //Through all the list.
            if(receiver->receivedfreq->list[i] == sender){ //If the sender is already on list.
                return;
            }
        }
        receiver->receivedfreq->list = realloc(receiver->receivedfreq->list,sizeof(user)*receiver->receivedfreq->size+1); //Allocate space for the new friend.
        receiver->receivedfreq->list[receiver->receivedfreq->size] = sender; //Set friend in the list.
        receiver->receivedfreq->size+=1; //Increase list size.
    }
    else{
        receiver->receivedfreq = malloc(sizeof(userlist)); //Create list.
        receiver->receivedfreq->list = malloc(sizeof(user)); //Create list.
        receiver->receivedfreq->list[0] = sender; //Set sender.
        receiver->receivedfreq->size=1; //Set list size.
    }
}

void add_friend(user* user1, user* newfriend){
    //User 1
    if (user1->friendlist == NULL){ //If there is no friend list.
        user1->friendlist = malloc(sizeof(userlist)); //Allocate space for list.
        user1->friendlist->list = malloc(sizeof(user)); //Allocate space for list.
        user1->friendlist->list[0] = newfriend; //Set friend.
        user1->friendlist->size = 1; //Set list size.
    }
    else{
        user1->friendlist->list = realloc(user1->friendlist->list, sizeof(user)*user1->friendlist->size+1); //Allocate space.
        user1->friendlist->list[user1->friendlist->size] = newfriend; //Set friend.
        user1->friendlist->size+=1; //Increase list size.
    }
    if (newfriend->friendlist == NULL){//If there is no friend list.
        newfriend->friendlist = malloc(sizeof(userlist)); //Allocate space for list.
        newfriend->friendlist->list = malloc(sizeof(user)); //Allocate space for list.
        newfriend->friendlist->list[0] = user1; //Set friend.
        newfriend->friendlist->size = 1; //Set list size.
    }
    //User 2
    else{
        newfriend->friendlist->list = realloc(newfriend->friendlist->list, sizeof(user)*newfriend->friendlist->size+1); //Allocate space.
        newfriend->friendlist->list[newfriend->friendlist->size] = user1; //Set friend.
        newfriend->friendlist->size+=1; //Increase list size.
    }
    erase_freq(user1->sentfreq, newfriend); //Erase friend request.
    erase_freq(newfriend->receivedfreq, user1); //Erase friend request.
}

void erase_freq(userlist* freqs, user* u){
    if (freqs == NULL) return; //If there is no list.
    for (int i = 0; i<freqs->size; i++){ //Go through all the list.
        if (freqs->list[i] == u){ //If user is found.
            freqs->list[i] = NULL; //Erase user.
            free(freqs->list[i]); //Free memory.
            freqs->size-=1; //Decrease size.
        }
    }
    if (freqs->size == 0){ //If list is empty, free space.
        freqs = NULL;
        free(freqs);
    }
}

void list_userlist(userlist *ulist){ //Print list of user structures.
    for (int i = 0; i<ulist->size; i++){
        printf("%s\n",ulist->list[i]->username);
    }
}