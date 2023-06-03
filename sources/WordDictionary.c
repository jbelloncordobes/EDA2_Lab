//
// Created by Usuario on 03/06/2023.
//

#include "../headers/WordDictionary.h"
#include <wchar.h>
#include <math.h>

void initDict(WDict* Dict, int size){
    Dict->elements = malloc(sizeof(DictNode) * size);
    Dict->size = size;
    for(int i = 0; i < size; i++){
        Dict->elements[i].count = 0;
    }
}

DictNode* searchDict(WDict* Dict, wchar_t* key){
    int i = 0;
    int value = 0;
    int total = 0;
    wprintf(L"%ls\n", key);
    while (key[i] != 0){
        value = (int) key[i];
        // wprintf(L"Character %lc numerical value is %d\n", key[i], value);
        total += value;
        i++;
    }
    // wprintf(L"Total: %d\n", total);
    total = total * i;
    total = (int) total % (int) Dict->size;
    // printf(L"Total module: %d\n", total);
    // Si no encuentra la key
    while(wcscmp(key, Dict->elements[total].key) != 0){
        // Si es un elemento vacío no existe en la lista.
        if(Dict->elements[total].count == 0){
            return NULL;
        }
        total++;
        // Si ha llegado al final sigue por el principio del array.
        if (total == Dict->size){
            total = 0;
        }
    }
    return &Dict->elements[total];
}

void addToDict(WDict* Dict, wchar_t* key){
    int i = 0;
    int value = 0;
    int total = 0;
    // wprintf(L"%ls\n", key);
    while (key[i] != 0){
        value = (int) key[i];
        // wprintf(L"Character %lc numerical value is %d\n", key[i], value);
        total += value;
        i++;
    }
    // wprintf(L"Total: %d\n", total);
    total = total * i;
    total = (int) total % (int) Dict->size;
    // wprintf(L"Total module: %d\n", total);
    while(Dict->elements[total].count != 0){
        total++;
        if (total == Dict->size){
            total = 0;
        }
    }
    wcscpy(Dict->elements[total].key, key);
    Dict->elements[total].count = 1;
}
