//
// Created by Usuario on 03/06/2023.
//

#ifndef EDA2_LAB_WORDDICTIONARY_H
#define EDA2_LAB_WORDDICTIONARY_H
#include "users.h"
#include "common.h"
#define MAX_WORD_LENGTH 20

typedef struct DictNode{
    wchar_t key[MAX_WORD_LENGTH];
    int count;
} DictNode;

typedef struct WDict{
    DictNode* elements;
    int size;
} WDict;

void initDict(WDict*, int);
DictNode* searchDict(WDict*, wchar_t*);
void addToDict(WDict*, wchar_t*);
#endif //EDA2_LAB_WORDDICTIONARY_H
