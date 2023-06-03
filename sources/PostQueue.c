//
// Created by Usuario on 01/06/2023.
//

#include "../headers/PostQueue.h"
#include <stdio.h>

void initQueue(PostQueue* queue){
    queue->first = NULL;
    queue->last = NULL;
    queue->current = 0;
}

void resetQueue(PostQueue* queue){
    queue->current = 0;
}

void addToQueue(PostQueue* queue, Post* post){
    if (queue->first == NULL){
        queue->first = post;
    } else {
        queue->last->next = post;
    }
    queue->last = post;
}

Post* dequeue(PostQueue* queue){
    Post* currpost = queue->first;
    for (int i = 0; i < queue->current; i++){
        currpost = currpost->next;
        if (currpost == NULL){
            return NULL;
        }
    }

    queue->current += 1;
    return currpost;
}