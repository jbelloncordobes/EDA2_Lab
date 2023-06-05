//
// Created by Usuario on 29/05/2023.
//

#include "../headers/user_arrays.h"

// Envia una petición de amistad de un usuario a otro
void sendFriendRequest(user* active_user, user* friend) {
    if (friend == NULL) {
        return;
    }
    // Se agrega el "amigo" a la lista de peticiones enviadas del usuario activo
    add_to_array(&active_user->friend_requests_sent, friend);
    // Se agrega el usuario activo a la lista de peticiones recibidas del "amigo"
    add_to_array(&friend->friend_requests_received, active_user);
}

// Acepta una petición de amistad de un usuario a otro :)
void acceptFriendRequest(user* active_user, user* friend) {
    if (friend == NULL) {
        return;
    }
    // Se elimina el amigo de la lista de peticiones recibidas del usuario activo
    remove_from_array(&active_user->friend_requests_received, friend);
    // Se elimina el usuario activo de la lista de peticiones enviadas del amigo
    remove_from_array(&friend->friend_requests_sent, active_user);
    // Se añade el amigo de la lista de amigos del usuario activo
    add_to_array(&active_user->friends, friend);
    // Se añade el usuario activo de la lista de amigos del amigo
    add_to_array(&friend->friends, active_user);
}

// Rechaza una petición de amistad de un usuario a otro :(
void rejectFriendRequest(user* active_user, user* notfriend) {
    if (notfriend == NULL) {
        return;
    }
    // Se elimina el no amigo de la lista de peticiones recibidas del usuario activo
    remove_from_array(&active_user->friend_requests_received, notfriend);
    // Se elimina el usuario activo de la lista de peticiones enviadas del no amigo
    remove_from_array(&notfriend->friend_requests_sent, active_user);
}

// Se agrega al usuario al array indicado
void add_to_array(user_array* array, user* addeduser) {
    // Si el tamaño es 0
    if (array->size == 0) {
        // Se inicializa el array con una posición
        array->users = malloc(sizeof(user*));
    } else {
        // Se redimensiona el array con una posición más.
        array->users = realloc(array->users, sizeof(user*) * (array->size + 1));
    }

    // Se agrega al usuario al final y se añade 1 al tamaño
    array->users[array->size] = addeduser;
    array->size += 1;
}

// Se elimina al usuario del array indicado
void remove_from_array(user_array* array, user* removeduser) {
    int removed = FALSE;
    // Se itera en el array.
    // Cuando el usuario a borrar es encontrado, todos los usuarios que vienen después se desplazan un puesto a la izquierda
    // sobreescribiendo al usuario borrado
    for (int i = 0; i < array->size; i++){
        // Si el usuario ha sido encontrado
        if (removed == TRUE){
            // Se sobreescribe el usuario en la posición anterior con el de la posición actual
            array->users[i-1] = array->users[i];
        }
        // Si el usuario es el usuario a borrar
        if (array->users[i]->id == removeduser->id){
            // Removed pasa a ser True
            removed = TRUE;
        }
    }
    // Se actualiza el tamaño
    array->size -= 1;
}

// Busca al usuario y devuelve TRUE o FALSE dependiendo de si existe
int search_User(user_array users, int id){
    // Itera entre los usuarios
    for (int i = 0; i < users.size; i++){
        // Si encuentra al usuario con la id indicada devuelve True
        if (id == users.users[i]->id){
            return TRUE;
        }
    }
    // Si ha iterado todos los usuarios y no lo ha encontrado devuelve False
    return FALSE;
}