//
// Created by Usuario on 02/06/2023.
//

#include "../headers/posts.h"

// Crea un Post con el texto introducido y lo devuelve
Post* createPost(wchar_t post[POST_LENGTH]){
    Post* newpost = malloc(sizeof(Post));
    wcscpy(newpost->message, post);
    newpost->next = NULL;
}