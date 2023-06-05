//
// Created by Usuario on 03/06/2023.
//

#include "../headers/WordDictionary.h"
#include <wchar.h>
#include <math.h>

// Inicializa el diccionario con el tamaño indicado
void initDict(WDict* Dict, int size){
    Dict->elements = malloc(sizeof(DictNode) * size);
    Dict->size = size;
    // Inicializa todos los elementos del diccionario con un count de 0
    for(int i = 0; i < size; i++){
        Dict->elements[i].count = 0;
    }
}

// Busca la entrada del diccionario con la clave indicada
DictNode* searchDict(WDict* Dict, wchar_t* key){
    int i = 0;
    int value = 0;
    int total = 0;
    // Convierte cada caracter de la wide string a su valor unicode en UTF-16 y lo suma a total
    while (key[i] != 0){
        value = (int) key[i];
        total += value;
        i++;
    }
    // Multiplica total (la suma del valor unicode de los caracteres) por el número de caracteres en la palabra
    total = total * i;
    // Calcula el residuo de la división del número obtenido entre el tamaño del diccionario
    total = (int) total % (int) Dict->size;
    // Si no encuentra la clave que está buscando busca en el siguiente elemento
    while(wcscmp(key, Dict->elements[total].key) != 0){
        // Si es un elemento vacío significa que no existe la clave en la lista, ya que habría caído ahí de ser así.
        if(Dict->elements[total].count == 0){
            return NULL;
        }
        total++;
        // Si ha llegado al final sigue por el principio del array.
        if (total == Dict->size){
            total = 0;
        }
    }
    // Si lo ha encontrado lo devuelve
    return &Dict->elements[total];
}

// Agrega la clave al diccionario
void addToDict(WDict* Dict, wchar_t* key){
    int i = 0;
    int value = 0;
    int total = 0;
    // Convierte cada caracter de la wide string a su valor unicode en UTF-16 y lo suma a total
    while (key[i] != 0){
        value = (int) key[i];
        total += value;
        i++;
    }
    // Multiplica total (la suma del valor unicode de los caracteres) por el número de caracteres en la palabra
    total = total * i;
    // Calcula el residuo de la división del número obtenido entre el tamaño del diccionario
    total = (int) total % (int) Dict->size;
    // Si la clave que le toca no está vacía
    while(Dict->elements[total].count != 0){
        // Busca linealmente la siguiente clave vacía
        total++;
        // Si llega al final del diccionario empieza de nuevo por el principio
        if (total == Dict->size){
            total = 0;
        }
    }
    // Guarda la clave en la entrada encontrada y pone el contador a 1.
    wcscpy(Dict->elements[total].key, key);
    Dict->elements[total].count = 1;
}

// función merge del mergeSort del diccionario. Ordena por aparición de palabras de forma descendente
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
        // Comprueba que palabra ha aparecido más
        if (L[i].count >= R[j].count){
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

// función mergeSort del diccionario
void mergeSortDict(WDict* Dict, int left, int right){
    if (left < right){
        int middle = (left + right) / 2;

        mergeSortDict(Dict, left, middle);
        mergeSortDict(Dict, middle + 1, right);

        mergeDict(Dict, left, middle, right);
    }

}