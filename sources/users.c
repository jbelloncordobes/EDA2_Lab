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
    newuser->friendlist = malloc(sizeof(user));
    newuser->friendlist->size = 0;
    newuser->sentfreq = malloc(sizeof(user));
    newuser->sentfreq->size = 0;
    newuser->receivedfreq = malloc(sizeof(user));
    newuser->receivedfreq->size = 0;

    printf("\nUsuario Creado\n");
    return newuser;
}


/*void addUser(user** array, user* newuser, int *userlength){
    if (array == NULL){
        array = malloc(sizeof(user));
    } else {
        array = realloc(array, sizeof(user) * (*userlength + 1));
    }
    array[*userlength] = newuser;
    *userlength += 1;
}*/

void addUser(nodelist* nlist, user* newuser){
    unode *n = malloc(sizeof(unode));
    n->User = newuser;
    if (nlist->first == NULL) {
        nlist->first = n;
        nlist->size = 1;
        nlist->serial = 0;
        n->prev = NULL;
    }
    else{
        nlist->last->next = n;
        nlist->size += 1;
        n->prev = nlist->last;
    }
    n->User->id = nlist->serial;
    nlist->serial+=1;
    nlist->last = n;
    n->next = NULL;
}

void eraseUser(nodelist *nlist, char* name){
    unode *n = nlist->first;
    while (n!= NULL){
        if (strcmp(n->User->username, name) == 0){
            n->prev->next = n->next;
            n->next->prev = n->prev;
            n = NULL;
            free(n);
            return;
        }
        n = n->next;
    }
}

/*void listUsers(user** array, int userlength){
    printf("Estos son todos los usuarios: \n");
    for(int i = 0; i < userlength; i++){
        printf("%s\n", array[i]->username);
    }
    printf("\n");
}*/

void listUsers(nodelist* nlist){
    printf("Estos son todos los usuarios: \n");
    unode *n = nlist->first;
    while(n!=NULL){
        printf("%s\n", n->User->username);
        n = n->next;
    }
    printf("\n");
}

void send_friend_req(user* sender, user* receiver){
    //Si en la lista de sent friend requests del sender ya está el receiver, entonces printf para avisar y return.
    //En cualquier otro caso, añadir el receiver a la sent friend request list.
    // y notificar al sender con un printf.
}

void receive_friend_req(user* sender, user* receiver){
    //Si en la lista de sent friend requests del sender ya está el receiver, entonces printf para avisar y return.
    //En cualquier otro caso, añadir el sender a la received friend request list del receiver.
    // y notificar al sender con un printf.
}

void confirm_friend(user* sender, user* receiver){
    //Llamar a add_friend desde las dos perspectivas (sender y receiver).
}


void add_friend(user* user1, user* newfriend){
    if (user1->friendlist == NULL){
        user1->friendlist = malloc(sizeof(userlist));
        user1->friendlist->list = malloc(sizeof(user));
        user1->friendlist->list[0] = newfriend;
        user1->friendlist->size = 1;
    }
    else{
        user1->friendlist->list = realloc(user1->friendlist->list, sizeof(user)*user1->friendlist->size+1);
        user1->friendlist->list[user1->friendlist->size] = newfriend;
        user1->friendlist->size+=1;
    }
    erase_freq(user1->sentfreq, newfriend);
    erase_freq(newfriend->receivedfreq, user1);
}

void erase_freq(userlist* freqs, user* u){
    if (freqs == NULL) return;
    for (int i = 0; i<freqs->size; i++){
        if (freqs->list[i] == u){
            free(freqs->list[i]);
            freqs->list[i] = NULL;
            freqs->size-=1;
        }
    }
}
