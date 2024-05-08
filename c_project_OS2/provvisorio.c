#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <ctype.h>

// Definizione delle strutture dati input
struct nodo {
    wchar_t* stringa; // Cambiato da char* a wchar_t*
    struct nodo* predecessore;
    struct nodo* successivo;
    struct nodo_con_contatore* dizionario;
    int cardinalità_successivi;
};

struct nodo_con_contatore {
    struct nodo_con_contatore* successivo;
    struct nodo* nodo_puntato;
    float contatore;
    float percentuale;
};



// Prototipi delle funzioni
struct nodo* crea_nodo(wchar_t* buffer, int dim, struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr);
void conteggio(struct nodo* nodo, struct nodo** passatoptr);
struct nodo* check(wchar_t* parola, struct nodo** testaptr);
wchar_t* crea_parola(wchar_t* buffer, int dim);
void calcola_percentuale(struct nodo* testaptr);
void libera_memoria(struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo** passatoptr);
void produci_csv(char* filedaAprire, struct nodo* testa);
void input(struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo** passatoptr, char* fileSorgente);
void compito1(int argc, char* nomefilecsv, char* nomefiletxt);

// Funzione principale
int main(int argc, char* argv[]) {
    compito1(argc, (char*) argv[1], (char*) argv[2]);
    return 0;
}

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
struct nodo* crea_nodo(wchar_t* buffer, int dim, struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr) {
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
        return nuovo_nodo;
    } else {
        // Verifica se la parola è già presente nella lista
        struct nodo* esistente = check(buffer, testaptr);
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

void libera_memoria(struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo** passatoptr) {
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
    }
}

void produci_csv(char* filedaAprire, struct nodo* testa) {
    if (filedaAprire != NULL) {
        FILE* fileAperto = fopen(filedaAprire, "w, ccs=UTF-8");
        struct nodo* curr = testa;
        while (curr != NULL) {
            fwprintf(fileAperto, L"%ls", curr->stringa);
            struct nodo_con_contatore* dada = curr->dizionario;
            while (dada != NULL) {
                fwprintf(fileAperto, L",%ls,%f", (dada->nodo_puntato)->stringa, dada->percentuale);
                dada = dada->successivo;
            }
            fwprintf(fileAperto, L"\n");
            curr = curr->successivo;
        }
        fclose(fileAperto);
    }
}

// Funzione per gestire l'input
void input(struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo** passatoptr, char* fileSorgente) {
    wchar_t buffer[100]; //DA CAMBIARE IN 32
    int dim = 0;
    wchar_t carattere;
    FILE* fileSorgenteP = fopen(fileSorgente, "r, ccs=UTF-8");

    while (1) {
        carattere = tolower(fgetwc(fileSorgenteP));
        if (carattere == WEOF) {
            if (dim > 0) {
                buffer[dim++] = L'\0';
                struct nodo* nodo = crea_nodo(buffer, dim, testaptr, codaptr, ultimoptr); // se necessario creo un nodo
            }
            break;
        }
        else if (carattere == L' ' || carattere == L'\n') {
            if (dim > 0) {
                buffer[dim++] = L'\0';
                struct nodo* nodo = crea_nodo(buffer, dim, testaptr, codaptr, ultimoptr);
                conteggio(nodo, passatoptr);
            }
            dim = 0;
        }
        else if (carattere == L'.' || carattere == L'?' || carattere == L'!') {
            if (dim > 0) {
                buffer[dim++] = L'\0';
                struct nodo* nodo = crea_nodo(buffer, dim, testaptr, codaptr, ultimoptr);
                conteggio(nodo, passatoptr);
            }
            dim = 0;
            buffer[dim] = carattere;
            buffer[++dim] = L'\0';
            struct nodo* nodo = crea_nodo(buffer, dim, testaptr, codaptr, ultimoptr);
            conteggio(nodo, passatoptr);
        }
        else if (carattere == L',' || carattere == L'^' || carattere == L'-' || carattere == L'_' 
              || carattere == L':' || carattere == L';' || carattere == L',' || carattere == L'|' 
              || carattere == L'(' || carattere == L')' || carattere == L'{' || carattere == L'}'
              || carattere == L'[' || carattere == L']' || carattere == L'#'){
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
    fclose(fileSorgenteP);
}

void compito1(int argc, char* nomefilecsv, char* nomefiletxt) {
    // Dichiarazione delle variabili 
    struct nodo* testa = NULL;
    struct nodo* coda = NULL;
    struct nodo* ultimo = NULL;
    struct nodo* passato = NULL;
    setlocale(LC_ALL, ""); // Imposta la localizzazione per supportare UTF-8
    setlocale(LC_NUMERIC, "en_US.UTF-8");
    input(&testa, &coda, &ultimo, &passato, nomefiletxt);
    calcola_percentuale(testa);
    if (argc > 1) {
        produci_csv(nomefilecsv, testa);
        libera_memoria(&testa, &coda, &ultimo, &passato);
    }
}