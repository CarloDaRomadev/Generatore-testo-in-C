
#ifndef STRUTTURE_DATI_H
#define STRUTTURE_DATI_H

#include <wchar.h>
#include <wctype.h>

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

#endif