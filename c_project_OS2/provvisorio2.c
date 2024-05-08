#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>
#include <wctype.h>

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
};

// Funzione per controllare se una parola è già presente nella lista
struct nodo* check(wchar_t* parola, struct nodo** testaptr) {
    struct nodo* index = *testaptr;
    while (index != NULL) {
        if (wcscmp(index->stringa, parola) == 0) {
            return index;
        }
        index = index->successivo;
    }
    return NULL;
}

// Funzione per creare una parola
wchar_t* crea_parola(wchar_t* buffer, int dim, int* numeroparole) {
    wchar_t* parola = (wchar_t*)malloc((dim) * sizeof(wchar_t));
    if (parola == NULL) {
        // Gestione errore di allocazione memoria
        exit(1);
    }
    int index = 0;
    while (index < dim){
        parola[index] = buffer[index];
        index++;
    }
    (*numeroparole) ++;
    return parola;
}

// Funzione per creare un nuovo nodo e aggiungerlo alla lista
struct nodo* crea_nodo(wchar_t* buffer, int dim, struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, int* numeroparole) {
    if (*testaptr == NULL) {
        // Creazione del primo nodo della lista
        wchar_t* parola = crea_parola(buffer, dim, numeroparole);
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
        nuovo_nodo -> cardinalità_successivi = 0;
        return nuovo_nodo;
    } 
    else {
        // Verifica se la parola è già presente nella lista
        struct nodo* esistente = check(buffer, testaptr);
        if (esistente != NULL) {
            return esistente;
        } 
        else {
            // Creazione di un nuovo nodo e aggiunta in coda alla lista
            wchar_t* parola = crea_parola(buffer, dim, numeroparole);
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
            nuovo_nodo -> cardinalità_successivi = 0;
            return nuovo_nodo;
        }
    }
}

void conteggio(struct nodo* nodo, struct nodo** passatoptr) {
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

wchar_t* caps(wchar_t* parola){
    wchar_t carattere = parola[0];
    parola[0] = towupper(carattere);
    return parola;
}

void genera_frase(int numeroparolerequest, int numeroParole, struct nodo** testa) {
    // Inizializza il generatore di numeri casuali utilizzando il tempo corrente come seme
    srand(time(NULL));
    int flag = 1;
    // Genera un numero casuale
    int numero_casuale = rand() % numeroParole;
    struct nodo* indiceptr = *testa;
    struct nodo_con_contatore* successivoPuntato = NULL;
    int massimo = 0;
    for (int i = 0; (i < numero_casuale) && (indiceptr != NULL); i++) {
        indiceptr = indiceptr->successivo;
    }

    for (int j = 0; j < numeroparolerequest; j++){

        if (*(indiceptr -> stringa) == L'.' || *(indiceptr -> stringa) == L'?' || *(indiceptr -> stringa) == L'!'){
            flag = 1;
            wprintf(L"%ls ", (indiceptr->stringa));
        }

        else if (flag == 0){
            wprintf(L" %ls", indiceptr->stringa);
        }

        else if (flag == 1){
            wprintf(L"%ls", caps(indiceptr->stringa));
            flag = 0;
        }

        massimo = indiceptr->cardinalità_successivi;
        if (massimo == 0) break; // Se non ci sono successivi, interrompi la generazione della frase
        numero_casuale = rand() % massimo;
        successivoPuntato = indiceptr->dizionario;
        for (int k = 0; (k < numero_casuale) && (successivoPuntato != NULL); k++) {
            successivoPuntato = successivoPuntato->successivo;
        }
        if (successivoPuntato == NULL) break; // Se non ci sono successivi, interrompi la generazione della frase
        indiceptr = successivoPuntato->nodo_puntato;
    }
    wprintf(L"\n");
}

void libera_memoria(struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo** passatoptr){
    while (*testaptr != NULL){
    struct nodo_con_contatore* lista = (*testaptr) -> dizionario;
        while (lista != NULL){
            struct nodo_con_contatore* savedlista = lista -> successivo;
            free(lista);
            lista = savedlista;
        }
        struct nodo* savedtesta = (*testaptr) -> successivo;
        free(*testaptr);
        *testaptr = savedtesta;
        *testaptr = NULL;
        *codaptr = NULL;
        *ultimoptr = NULL;
        *passatoptr = NULL;
    }
}


void input(char* filename, struct nodo** testa, struct nodo** coda, struct nodo** ultimo, struct nodo** passato, int* numeroParole){
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
            *passato = crea_nodo(buffer, dim, testa, coda, ultimo, numeroParole);
            flag = 2;
            dim = 0;
        }

        else if ((flag == 2) && (carattere == L',')) {
            buffer[dim++] = L'\0';
            struct nodo* nodo = crea_nodo(buffer, dim, testa, coda, ultimo, numeroParole);
            flag = 1;
            dim = 0;
            conteggio(nodo, passato);
            
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

void compito2(char* nomefile, int numeroparolerequest){
    setlocale(LC_ALL, ""); // Imposta la localizzazione per supportare UTF-8
    struct nodo* testa = NULL;
    struct nodo* coda = NULL;
    struct nodo* ultimo = NULL;
    struct nodo* passato = NULL;
    int numeroParole = 0;
    input(nomefile, &testa, &coda, &ultimo, &passato, &numeroParole);
    genera_frase(numeroparolerequest, numeroParole, &testa);
    libera_memoria(&testa, &coda, &ultimo, &passato);
}

//prendi un file csv
void main(int argc, char* argv[]){
    int numeroParolerequest;
    if (argc > 2){
        compito2((char*) argv[1], atoi((argv[2])));
    }
}