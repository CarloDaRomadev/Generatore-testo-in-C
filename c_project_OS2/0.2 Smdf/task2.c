#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>
#include <wctype.h>

// Definizione delle strutture dati input
#include "strutture_dati.h"


void libera_memoria(struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo** passatoptr, struct nodo_hashmap*** vettoreptr);
//prototipi funzioni
void conteggio2(struct nodo* nodo, struct nodo** passatoptr);

struct nodo* checkopt(struct nodo_hashmap** vettoreptr, wchar_t* buffer, int dim);
void ridimensionaHash(struct nodo_hashmap*** vettoreptr, int* dim, int* contatoreCelle);
void inserisciInHash(struct nodo_hashmap** vettoreptr, struct nodo* nodoptr, int dim, int* contatoreCelle);

// Funzione per creare una parola
wchar_t* crea_parola(wchar_t* buffer, int dim);

// Funzione per creare un nuovo nodo e aggiungerlo alla lista
struct nodo* crea_nodo(wchar_t* buffer, int dim, struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo_hashmap*** vettoreptr, int* dimhash, int* contatoreCelle);

wchar_t* caps2(wchar_t* parola){
    wchar_t carattere = parola[0];
    parola[0] = towupper(carattere);
    return parola;
}

struct nodo* cerca_Punteggiatura(int dimhash, struct nodo_hashmap** vettoreHashmap){
    struct nodo* punto = NULL;
    wchar_t chars[] = {L'?', L'!', L'.'};
    int check[] = {0,0,0};
    while (punto == NULL){
        if ((check[0] == 1) && (check[1] == 1) && (check[2] == 1)){
            perror("nessuna punteggiatura presente nel testo");
            exit(1);
        }
        int valore = rand() % 3;
        wchar_t random_char = chars[valore];
        check[valore] = 1;
        punto = checkopt(vettoreHashmap, &random_char, dimhash);
    }
    return punto;
}


void genera_frase2(int numeroparolerequest, struct nodo** testa, wchar_t* parola, int dimhash, struct nodo_hashmap** vettoreHashmap) {
    // Inizializza il generatore di numeri casuali utilizzando il tempo corrente come seme
    srand(time(NULL));
    int flag = 1;
    // Genera un numero casuale
    int numero_casuale;
    struct nodo* indiceptr = *testa;
    struct nodo_con_contatore* successivoPuntato = NULL;
    int massimo = 0;
    if (wcslen(parola) == 0){
            indiceptr = cerca_Punteggiatura(dimhash, vettoreHashmap);
    }

    else{
        indiceptr = checkopt(vettoreHashmap, parola, dimhash);
        if (indiceptr == NULL){
            perror("parola non presente");
            exit(1);
        }
    }

    for (int j = 0; j < numeroparolerequest; j++){
        massimo = indiceptr->cardinalità_successivi;
        if (massimo == 0) break; // Se non ci sono successivi, interrompi la generazione della frase
        numero_casuale = rand() % massimo;
        successivoPuntato = indiceptr->dizionario;
        for (int k = 0; (k < numero_casuale) && (successivoPuntato != NULL); k++) {
            successivoPuntato = successivoPuntato->successivo;
        }
        if (successivoPuntato == NULL) break; // Se non ci sono successivi, interrompi la generazione della frase
        indiceptr = successivoPuntato->nodo_puntato;
        if (*(indiceptr -> stringa) == L'.' || *(indiceptr -> stringa) == L'?' || *(indiceptr -> stringa) == L'!'){
            flag = 1;
            printf("%ls ", (indiceptr->stringa));
        }

        else if (flag == 0){
            printf(" %ls", indiceptr->stringa);
        }

        else if (flag == 1){
            printf("%ls", caps2(indiceptr->stringa));
            flag = 0;
        }

    }
    printf("\n");
}


void input2(char* filename, struct nodo** testa, struct nodo** coda, struct nodo** ultimo, struct nodo** passato, struct nodo_hashmap*** vettoreptr, int* dimhash, int* contatoreCelle){
    FILE* fileCsvgInput = fopen(filename, "r, ccs=UTF-8");
    wchar_t carattere;
    wchar_t buffer[100]; //************************************************************************************************************* modifica in 31
    int flag = 0;
    int dim = 0;

    while (1){
        carattere = (fgetwc(fileCsvgInput));
        
        //parolaPrincipale
        if ((flag == 0) && (carattere == L',')){
            buffer[dim++] = L'\0';
            *passato = crea_nodo(buffer, dim, testa, coda, ultimo, vettoreptr, dimhash, contatoreCelle);
            flag = 2;
            dim = 0;
        }

        else if ((flag == 2) && (carattere == L',')) {
            buffer[dim++] = L'\0';
            struct nodo* nodo = crea_nodo(buffer, dim, testa, coda, ultimo, vettoreptr, dimhash, contatoreCelle);
            flag = 1;
            dim = 0;
            conteggio2(nodo, passato);
            
        }

        else if ((flag == 1) && (carattere == L',')) {
            dim = 0;
            flag = 2;
            continue;
        }

        else if (carattere == L'\n'){
            dim = 0;
            flag = 0;
            continue;
        }

        else if (carattere == WEOF){
            break;
        }

        else{
            buffer[dim++] = carattere;
        }
    }
    fclose(fileCsvgInput);
}
void Task2(char* nomefile, int numeroparolerequest, wchar_t* parola){
    int dimhash = 1024; 
    int contatoreCelle = 0;
    struct nodo_hashmap** vettoreHashmap = (struct nodo_hashmap**)malloc(dimhash * sizeof(struct nodo_hashmap*));
    for (int i = 0; i < dimhash; i++) {
        vettoreHashmap[i] = NULL;
    }
    setlocale(LC_ALL, ""); // Imposta la localizzazione per supportare UTF-8
    setlocale(LC_NUMERIC, "en_US.UTF-8");
    struct nodo* testa = NULL;
    struct nodo* coda = NULL;
    struct nodo* ultimo = NULL;
    struct nodo* passato = NULL;
    input2(nomefile, &testa, &coda, &ultimo, &passato, &vettoreHashmap, &dimhash, &contatoreCelle);
    genera_frase2(numeroparolerequest, &testa, parola, dimhash, vettoreHashmap);
    libera_memoria(&testa, &coda, &ultimo, &passato, &vettoreHashmap);
}