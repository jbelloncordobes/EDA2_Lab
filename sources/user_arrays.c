//
// Created by Usuario on 29/05/2023.
//

#include "../headers/user_arrays.h"

void sendFriendRequest(user* active_user, user* friend) {
    if (friend == NULL) {
        return;
    }
    add_to_array(&active_user->friend_requests_sent, friend);
    add_to_array(&friend->friend_requests_received, active_user);
}

void acceptFriendRequest(user* active_user, user* friend) {
    if (friend == NULL) {
        return;
    }
    remove_from_array(&active_user->friend_requests_received, friend);
    remove_from_array(&friend->friend_requests_sent, active_user);
    add_to_array(&active_user->friends, friend);
    add_to_array(&friend->friends, active_user);
}

void rejectFriendRequest(user* active_user, user* friend) {
    if (friend == NULL) {
        return;
    }
    remove_from_array(&active_user->friend_requests_received, friend);
    remove_from_array(&friend->friend_requests_sent, active_user);
}


void add_to_array(user_array* array, user* addeduser) {
    if (array->size == 0) {
        array->users = malloc(sizeof(user*));
    } else {
        array->users = realloc(array->users, sizeof(user*) * (array->size + 1));
    }

    array->users[array->size] = addeduser;
    array->size += 1;
}

void remove_from_array(user_array* array, user* removeduser) {
    int removed = FALSE;
    for (int i = 0; i < array->size; i++){
        if (removed == TRUE){
            array->users[i-1] = array->users[i];
        }
        if (array->users[i]->id == removeduser->id){
            removed = TRUE;
        }
    }

    array->size -= 1;
}

int search_User(user_array users, int id){
    for (int i = 0; i < users.size; i++){
        if (id == users.users[i]->id){
            return TRUE;
        }
    }
    return FALSE;
}