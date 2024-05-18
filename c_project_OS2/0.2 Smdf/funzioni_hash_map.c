#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <ctype.h>


struct nodo {
    wchar_t* stringa;
    struct nodo* predecessore;
    struct nodo* successivo;
    struct nodo_con_contatore* dizionario;
    int cardinalità_successivi;
};

struct nodo_con_contatore {
    struct nodo_con_contatore* successivo;
    struct nodo* nodo_puntato;
    double contatore;
    double percentuale;
};

struct nodo_hashmap
{
    struct nodo* parola;
    struct nodo_hashmap* successivo;
};


void inserisciInHash(struct nodo_hashmap** vettoreptr, struct nodo* nodoptr, int dim, int* contatoreCelle) {
    int valore = 0;
    int i = 0;
    wchar_t* str = nodoptr->stringa;
    
    while (str[i] != L'\0') {
        valore += ((int)str[i]);
        i++;
    }

    if (valore < 0){
    valore = -valore;
    }
    
    int resto = valore % dim;
    
    if (vettoreptr[resto] == NULL) {
        struct nodo_hashmap* nodo = (struct nodo_hashmap*)malloc(sizeof(struct nodo_hashmap));
        nodo->successivo = NULL;
        nodo->parola = nodoptr;
        vettoreptr[resto] = nodo;
        //printf("1 nuova parola ");
        *contatoreCelle += 1;
        return;
    }
    else {
    struct nodo_hashmap* ptr = vettoreptr[resto];
    while (ptr->successivo != NULL) {
        ptr = ptr->successivo;
    }
    struct nodo_hashmap* nodo = (struct nodo_hashmap*)malloc(sizeof(struct nodo_hashmap));
    nodo->successivo = NULL;
    nodo->parola = nodoptr;
    ptr->successivo = nodo;
    return;

    }
}


void ridimensionaHash(struct nodo_hashmap*** vettoreptr, int* dim, int* contatoreCelle) {
    *dim = *dim + 128;
    *contatoreCelle = 0;
    struct nodo_hashmap** nuovovettoreptr = (struct nodo_hashmap**)malloc(*dim * sizeof(struct nodo_hashmap*));
    if (nuovovettoreptr == NULL) {
        // Gestione errore di allocazione della memoria
        exit(1);
    }

    // Inizializza nuovovettoreptr con puntatori NULL
    for (int i = 0; i < *dim; i++) {
        nuovovettoreptr[i] = NULL;
    }

    // Copia i puntatori validi dal vecchio array al nuovo
    for (int j = 0; j < *dim - 128; j++) {
        if ((*vettoreptr)[j] == NULL) {
            continue;
        } 
        else {
            struct nodo_hashmap* ptr = (*vettoreptr)[j];
            while (ptr != NULL) {
                inserisciInHash(nuovovettoreptr, ptr->parola, *dim, contatoreCelle);
                ptr = ptr->successivo;
            }
        }
    }

    // Liberare la memoria occupata dall'array vettoreptr (vecchio vettore);
    free(*vettoreptr);
    // Assegnare a vettoreptr il nuovo array ridimensionato
    *vettoreptr = nuovovettoreptr;
}

struct nodo* checkopt(struct nodo_hashmap** vettoreptr, wchar_t* buffer, int dim){
    int valore = 0;
    int i = 0;
    
    while (buffer[i] != L'\0') {
        valore += ((int)buffer[i]);
        i++;
    }

    if (valore < 0){
    valore = -valore;
    }
    
    int resto = valore % dim;
    
    if (vettoreptr[resto] == NULL) {
        return NULL;
    }
    else {
        struct nodo_hashmap* ptr = vettoreptr[resto];
        
        while (ptr != NULL) {
            if (wcscmp((ptr -> parola) -> stringa, buffer) == 0){
                return ptr -> parola;
            }
            ptr = ptr->successivo;
        }
        return NULL;
    }
}
