//
// Created by Usuario on 01/06/2023.
//

#include "../headers/PostQueue.h"

// Inicializa la cola
void initQueue(PostQueue* queue){
    queue->first = NULL;
    queue->last = NULL;
    queue->current = 0;
}

// Devuelve el "puntero" de la cola al principio sin vaciarla
void resetQueue(PostQueue* queue){
    queue->current = 0;
}

// Agrega el nuevo elemento a la cola
void addToQueue(PostQueue* queue, Post* post){
    // Si es el primero
    if (queue->first == NULL){
        // Lo agrega a la lista como primer elemento
        queue->first = post;
    } else {
        // Si ya hay primer elemento, el último elemento de la lista apunta al nuevo post
        queue->last->next = post;
    }
    // El último post es ahora el nuevo
    queue->last = post;
}

// Saca un elemento de la cola en la posición del "puntero" actual
Post* dequeue(PostQueue* queue){
    // Carga el primer elemento
    Post* currpost = queue->first;
    // Itera entre todos los elementos de la lista hasta encontrar el elemento al cual apunta el puntero
    for (int i = 0; i < queue->current; i++){
        currpost = currpost->next;
        if (currpost == NULL){
            return NULL;
        }
    }
    // El puntero apunta al siguiente post y devuelve el post sacado de la iteración
    queue->current += 1;
    return currpost;
}