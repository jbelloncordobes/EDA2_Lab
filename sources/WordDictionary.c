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

void mergeDict(WDict* Dict, int left, int middle, int right){
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    DictNode L[n1], R[n2];

    for (i = 0; i < n1; i++){
        L[i] = Dict->elements[left + i];
    }
    for (j = 0; j < n2; j++){
        R[j] = Dict->elements[middle + 1 + j];
    }
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2){
        if (L[i].count <= R[j].count){
            //wchar_t test[40];
            //wcscpy(test, L[i].key);
            //int count = L[i].count;
            Dict->elements[k] = L[i];
            i++;
        } else {
            Dict->elements[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1){
        Dict->elements[k] = L[i];
        i++;
        k++;
    }

    while (j < n2){
        Dict->elements[k] = R[j];
        j++;
        k++;
    }

}

void mergeSortDict(WDict* Dict, int left, int right){
    if (left < right-1){
        int middle = (left + right) / 2;

        mergeSortDict(Dict, left, middle);
        mergeSortDict(Dict, middle + 1, right);

        mergeDict(Dict, left, middle, right);
    }

}