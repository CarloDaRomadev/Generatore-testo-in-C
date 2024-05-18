#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <ctype.h>



// Definizione delle strutture dati input
#include "strutture_dati.h"

// Prototipi delle funzioni
void produci_csv(char* filedaAprire, struct nodo* testa);
void input(struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo** passatoptr, char* fileSorgente, struct nodo_hashmap*** vettoreptr, int* dimhash, int* contatoreCelle);
void Task1(char* nomefilecsv, char* nomefiletxt);

struct nodo* checkopt(struct nodo_hashmap** vettoreptr, wchar_t* buffer, int dim);
void ridimensionaHash(struct nodo_hashmap*** vettoreptr, int* dim, int* contatoreCelle);
void inserisciInHash(struct nodo_hashmap** vettoreptr, struct nodo* nodoptr, int dim, int* contatoreCelle);
// Funzione per creare una parola
wchar_t* crea_parola(wchar_t* buffer, int dim);
// Funzione per creare un nuovo nodo e aggiungerlo alla lista
struct nodo* crea_nodo(wchar_t* buffer, int dim, struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo_hashmap*** vettoreptr, int* dimhash, int* contatoreCelle);


// Funzione per creare la lista dei successivi e conteggiare le parole
void conteggio(struct nodo* nodo, struct nodo** passatoptr);

void calcola_percentuale(struct nodo* testaptr);

void libera_memoria(struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo** passatoptr, struct nodo_hashmap*** vettoreptr);


void produci_csv(char* filedaAprire, struct nodo* testa){
    if (filedaAprire != NULL){
        FILE* fileAperto = fopen(filedaAprire, "w, ccs=UTF-8");
        if (fileAperto != NULL){
            struct nodo* curr = testa;
            while (curr != NULL) {
                fwprintf(fileAperto, L"%ls", curr->stringa);
                struct nodo_con_contatore* dada = curr->dizionario;
                while (dada != NULL) {
                    if ((dada->percentuale) != 1){
                        fwprintf(fileAperto, L",%ls,%f", (dada->nodo_puntato)->stringa, dada->percentuale);
                        
                    }
                    else{
                         fwprintf(fileAperto, L",%ls,%d", (dada->nodo_puntato)->stringa,(int) dada->percentuale);
                        
                    }
                    dada = dada->successivo;
                }
                fwprintf(fileAperto, L"\n");
                curr = curr->successivo;
            }
            fclose(fileAperto);
        } else {
            printf("Impossibile aprire il file per la scrittura.");
        }
    }
}


// Funzione per gestire l'input
void input(struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo** passatoptr, char* fileSorgente, struct nodo_hashmap*** vettoreptr, int* dimhash, int* contatoreCelle) {
    wchar_t buffer[100]; //DA CAMBIARE IN 32
    int dim = 0;
    wchar_t carattere;
    FILE* fileSorgenteP = fopen(fileSorgente, "r, ccs=UTF-8");

    while (1) {
        carattere = tolower(fgetwc(fileSorgenteP));
        if (carattere == WEOF) {
            if (dim > 0) {
                buffer[dim++] = L'\0';
                struct nodo* nodo = crea_nodo(buffer, dim, testaptr, codaptr, ultimoptr, vettoreptr, dimhash, contatoreCelle); // se necessario creo un nodo
                conteggio(nodo, passatoptr);
            }
            break;
        }
        else if (carattere == L' ' || carattere == L'\n') {
            if (dim > 0) {
                buffer[dim++] = L'\0';
                struct nodo* nodo = crea_nodo(buffer, dim, testaptr, codaptr, ultimoptr, vettoreptr, dimhash, contatoreCelle);
                conteggio(nodo, passatoptr);
            }
            dim = 0;
        }
        else if (carattere == L'\'') {
            if (dim > 0) {
                buffer[dim++] = '\'';
                buffer[dim++] = L'\0';
                struct nodo* nodo = crea_nodo(buffer, dim, testaptr, codaptr, ultimoptr, vettoreptr, dimhash, contatoreCelle);
                conteggio(nodo, passatoptr);
            }
            dim = 0;
        }
        else if (carattere == L'.' || carattere == L'?' || carattere == L'!') {
            if (dim > 0) {
                buffer[dim++] = L'\0';
                struct nodo* nodo = crea_nodo(buffer, dim, testaptr, codaptr, ultimoptr, vettoreptr, dimhash, contatoreCelle);
                conteggio(nodo, passatoptr);
            }
            dim = 0;
            buffer[dim] = carattere;
            buffer[++dim] = L'\0';
            struct nodo* nodo = crea_nodo(buffer, dim, testaptr, codaptr, ultimoptr, vettoreptr, dimhash, contatoreCelle);
            conteggio(nodo, passatoptr);
            dim = 0;
        }
        else if (carattere == L',' || carattere == L'^' || carattere == L'-' || carattere == L'_' 
              || carattere == L':' || carattere == L';' || carattere == L',' || carattere == L'|' 
              || carattere == L'(' || carattere == L')' || carattere == L'{' || carattere == L'}'
              || carattere == L'[' || carattere == L']' || carattere == L'#' || carattere == L'"'
              || carattere == L'*'){
            continue;
        }
        else {
            if (dim < 100) { //DA CAMBIARE IN 31
                buffer[dim++] = carattere;
            }
            else {
                // Gestione parola troppo lunga
                printf("Errore: Parola troppo lunga.\n");
                exit(1);
            }
        }
    }
    conteggio(*testaptr, passatoptr);
    fclose(fileSorgenteP);
}

void Task1(char* nomefilecsv, char* nomefiletxt) {
    // Dichiarazione delle variabili 
    int dimhash = 2048; 
setlocale(LC_ALL, "");
    int contatoreCelle = 0;

    struct nodo_hashmap** vettoreHashmap = (struct nodo_hashmap**)malloc(dimhash * sizeof(struct nodo_hashmap*));
    for (int i = 0; i < dimhash; i++) {
        vettoreHashmap[i] = NULL;
    }
    struct nodo* testa = NULL;
    struct nodo* coda = NULL;
    struct nodo* ultimo = NULL;
    struct nodo* passato = NULL;
    setlocale(LC_ALL, ""); // Imposta la localizzazione per supportare UTF-8
    setlocale(LC_NUMERIC, "en_US.UTF-8");
    input(&testa, &coda, &ultimo, &passato, nomefiletxt, &vettoreHashmap, &dimhash, &contatoreCelle);
    calcola_percentuale(testa);
    produci_csv(nomefilecsv, testa);
    libera_memoria(&testa, &coda, &ultimo, &passato, &vettoreHashmap);
    
}
