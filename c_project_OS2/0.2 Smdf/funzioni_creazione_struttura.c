#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <ctype.h>



// Definizione delle strutture dati input
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

//prototipi delle funzioni per interaggire con l'hashmap
struct nodo* checkopt(struct nodo_hashmap** vettoreptr, wchar_t* buffer, int dim);
void ridimensionaHash(struct nodo_hashmap*** vettoreptr, int* dim, int* contatoreCelle);
void inserisciInHash(struct nodo_hashmap** vettoreptr, struct nodo* nodoptr, int dim, int* contatoreCelle);

// Funzione per creare una parola
wchar_t* crea_parola(wchar_t* buffer, int dim) {
    wchar_t* parola = (wchar_t*)malloc((dim) * sizeof(wchar_t));
    if (parola == NULL) {
        // Gestione errore di allocazione memoria
        exit(1);
    }
    int index = 0;
    while (index < dim) {
        parola[index] = buffer[index];
        index++;
    }
    return parola;
}

// Funzione per creare un nuovo nodo e aggiungerlo alla lista
struct nodo* crea_nodo(wchar_t* buffer, int dim, struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo_hashmap*** vettoreptr, int* dimhash, int* contatoreCelle) {
    if (*testaptr == NULL) {
        // Creazione del primo nodo della lista
        wchar_t* parola = crea_parola(buffer, dim);
        struct nodo* nuovo_nodo = (struct nodo*)malloc(sizeof(struct nodo));
        if (nuovo_nodo == NULL) {
            // Gestione errore di allocazione memoria
            exit(1);
        }
        nuovo_nodo->predecessore = NULL;
        nuovo_nodo->successivo = NULL;
        nuovo_nodo->stringa = parola;
        nuovo_nodo->dizionario = NULL;
        *testaptr = nuovo_nodo;
        *codaptr = nuovo_nodo;
        *ultimoptr = nuovo_nodo;
        nuovo_nodo->cardinalità_successivi = 0;
        inserisciInHash(*vettoreptr, nuovo_nodo, *dimhash, contatoreCelle);
        return nuovo_nodo;
    } else {
        // Verifica se la parola è già presente nella lista
        struct nodo* esistente = checkopt(*vettoreptr, buffer, *dimhash);
        if (esistente != NULL) {
            return esistente;
        } else {
            // Creazione di un nuovo nodo e aggiunta in coda alla lista
            wchar_t* parola = crea_parola(buffer, dim);
            struct nodo* nuovo_nodo = (struct nodo*)malloc(sizeof(struct nodo));

            if (nuovo_nodo == NULL) {
                // Gestione errore di allocazione memoria
                exit(1);
            }
            nuovo_nodo->predecessore = *ultimoptr;
            nuovo_nodo->successivo = NULL;
            nuovo_nodo->stringa = parola;
            nuovo_nodo->dizionario = NULL;
            (*ultimoptr)->successivo = nuovo_nodo;
            *codaptr = nuovo_nodo;
            *ultimoptr = nuovo_nodo;
            nuovo_nodo->cardinalità_successivi = 0;
            if (*contatoreCelle > ((*dimhash / 4 )*3)){
                ridimensionaHash(vettoreptr, dimhash, contatoreCelle);
            }
            inserisciInHash(*vettoreptr, nuovo_nodo, *dimhash, contatoreCelle);
            return nuovo_nodo;
        }
    }
}
// Funzione per creare la lista dei successivi e conteggiare le parole
void conteggio(struct nodo* nodo, struct nodo** passatoptr) {
    if (*passatoptr != NULL) {
        //a prescindere da cosa succederà dopo aggiorno il contatore dei successivi.
        (*passatoptr)->cardinalità_successivi++;
        if ((*passatoptr)->dizionario == NULL) {
            // Se il dizionario è vuoto, crea un nuovo nodo per il dizionario
            struct nodo_con_contatore* nodo_dizionario = (struct nodo_con_contatore*)malloc(sizeof(struct nodo_con_contatore));
            if (nodo_dizionario == NULL) {
                // Gestione errore di allocazione della memoria
                exit(1);
            }
            nodo_dizionario->contatore = 1;
            nodo_dizionario->nodo_puntato = nodo;
            nodo_dizionario->successivo = NULL;
            (*passatoptr)->dizionario = nodo_dizionario;
        }
        else {
            struct nodo_con_contatore* counter = (*passatoptr)->dizionario;
            while (counter != NULL) {
                if (counter->nodo_puntato == nodo) {
                    counter->contatore += 1;
                    *passatoptr = nodo;
                    return;
                }
                counter = counter->successivo;
            }
            struct nodo_con_contatore* nodo_dizionario = (struct nodo_con_contatore*)malloc(sizeof(struct nodo_con_contatore));
            nodo_dizionario->nodo_puntato = nodo;
            nodo_dizionario->contatore = 1;
            struct nodo_con_contatore* ptr = ((*passatoptr)->dizionario);
            nodo_dizionario->successivo = ptr;
            (*passatoptr)->dizionario = nodo_dizionario;

        }
    }
    (*passatoptr) = nodo;
}

void conteggio2(struct nodo* nodo, struct nodo** passatoptr) {
    if (*passatoptr != NULL) {
        //a prescindere da cosa succederà dopo aggiorno il contatore dei successivi.
        if ((*passatoptr) ->dizionario == NULL) {
            // Se il dizionario è vuoto, crea un nuovo nodo per il dizionario
            struct nodo_con_contatore* nodo_dizionario = (struct nodo_con_contatore*)malloc(sizeof(struct nodo_con_contatore));
            if (nodo_dizionario == NULL) {
                // Gestione errore di allocazione della memoria
                exit(1);
            }
            nodo_dizionario->nodo_puntato = nodo;
            nodo_dizionario->successivo = NULL;
            (*passatoptr) ->dizionario = nodo_dizionario;
            (*passatoptr) -> cardinalità_successivi ++;
        }
        else {
            struct nodo_con_contatore* counter = (*passatoptr) -> dizionario;
            while(counter != NULL){
                if(counter -> nodo_puntato == nodo){
                    *passatoptr = nodo;
                    return;
                }
                counter = counter -> successivo;
            }
            struct nodo_con_contatore* nodo_dizionario = (struct nodo_con_contatore*)malloc(sizeof(struct nodo_con_contatore));
            nodo_dizionario -> nodo_puntato = nodo;
            struct nodo_con_contatore* ptr = ((*passatoptr)->dizionario);
            nodo_dizionario->successivo = ptr;
            (*passatoptr)->dizionario = nodo_dizionario;
            (*passatoptr) -> cardinalità_successivi ++;

        }
    }
    (*passatoptr) = nodo;
}

void calcola_percentuale(struct nodo* testaptr) {
    while (testaptr != NULL) {
        struct nodo_con_contatore* lista = (testaptr)->dizionario;
        while (lista != NULL) {
            lista->percentuale = ((lista->contatore) / ((testaptr)->cardinalità_successivi));
            lista = lista->successivo;
        }
        testaptr = (testaptr)->successivo;
    }
}

void libera_memoria(struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo** passatoptr, struct nodo_hashmap*** vettoreptr) {
    while (*testaptr != NULL) {
        struct nodo_con_contatore* lista = (*testaptr)->dizionario;
        while (lista != NULL) {
            struct nodo_con_contatore* savedlista = lista->successivo;
            free(lista);
            lista = savedlista;
        }
        struct nodo* savedtesta = (*testaptr)->successivo;
        free(*testaptr);
        *testaptr = savedtesta;
        *testaptr = NULL;
        *codaptr = NULL;
        *ultimoptr = NULL;
        *passatoptr = NULL;
        free(*vettoreptr);
    }
}