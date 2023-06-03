//
// Created by Usuario on 02/06/2023.
//

#include "../headers/posts.h"

//typedef struct Post{
//    wchar_t message[POST_LENGTH];
//    struct Post* next;
//}Post;

Post* createPost(wchar_t post[POST_LENGTH]){
    Post* newpost = malloc(sizeof(Post));
    wcscpy(newpost->message, post);
    newpost->next = NULL;
}