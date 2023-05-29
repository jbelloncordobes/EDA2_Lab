//
// Created by Usuario on 29/05/2023.
//

#ifndef EDA2_LAB_USER_ARRAYS_H
#define EDA2_LAB_USER_ARRAYS_H
#include "users.h"


// Forward Declarations
void add_to_array(user_array*, user*);
void remove_from_array(user_array*, user*);
void sendFriendRequest(user*, user*);
void acceptFriendRequest(user*, user*);
void rejectFriendRequest(user*, user*);
int search_User(user_array, int);
#endif //EDA2_LAB_USER_ARRAYS_H
