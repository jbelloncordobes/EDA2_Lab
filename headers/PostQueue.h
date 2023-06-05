//
// Created by Usuario on 01/06/2023.
//

#ifndef EDA2_LAB_POSTQUEUE_H
#define EDA2_LAB_POSTQUEUE_H
#include "users.h"
#include "common.h"

void initQueue(PostQueue*);
void resetQueue(PostQueue*);
void addToQueue(PostQueue*, Post*);
Post* dequeue(PostQueue*);


#endif //EDA2_LAB_POSTQUEUE_H
