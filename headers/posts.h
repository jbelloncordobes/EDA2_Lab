//
// Created by Usuario on 02/06/2023.
//

#ifndef EDA2_LAB_POSTS_H
#define EDA2_LAB_POSTS_H
#include "common.h"
#include "users.h"

//typedef struct Post{
//    wchar_t message[POST_LENGTH];
//    struct Post* next;
//}Post;

Post* createPost(wchar_t post[POST_LENGTH]);

#endif //EDA2_LAB_POSTS_H
