#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>



// Definizione delle strutture dati input
#include "strutture_dati.h"

// Prototipi delle funzioni


void ridimensionaHash(struct nodo_hashmap*** vettoreptr, int* dim, int* contatoreCelle);
void inserisciInHash(struct nodo_hashmap** vettoreptr, struct nodo* nodoptr, int dim, int* contatoreCelle);
struct nodo* checkopt(struct nodo_hashmap** vettoreptr, wchar_t* buffer, int dim);
wchar_t* crea_parola(wchar_t* buffer, int dim);
struct nodo* crea_nodo(wchar_t* buffer, int dim, struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo_hashmap*** vettoreptr, int* dimhash, int* contatoreCelle);
void conteggio(struct nodo* nodo, struct nodo** passatoptr);
void calcola_percentuale(struct nodo* testaptr);
void libera_memoria(struct nodo** testaptr, struct nodo** codaptr, struct nodo** ultimoptr, struct nodo** passatoptr, struct nodo_hashmap*** vettoreptr);


void input(char* file, int pipefd[]){
        setlocale(LC_ALL, "");
        setlocale(LC_NUMERIC, "en_US.UTF-8");
        wchar_t carattere;
        int dim = 0;
        wchar_t buffer[50]; 
        FILE* fileSorgenteP = fopen(file, "r, ccs=UTF-8");
        while (1) {
            carattere = tolower(fgetwc(fileSorgenteP));
            if (carattere == WEOF) {
                if (dim > 0) {
                    buffer[dim++] = L'\0';
                    write(pipefd[1], buffer, (sizeof(wchar_t))*50);
                }
                buffer[0] = L'\0';
                write(pipefd[1], buffer, (sizeof(wchar_t))*50);
                break;
            }
            else if (carattere == L'\'') {
            if (dim > 0) {
                buffer[dim++] = '\'';
                buffer[dim++] = L'\0';
                write(pipefd[1], buffer, (sizeof(wchar_t))*50);
            }
            dim = 0;
            }
            else if (carattere == L' ' || carattere == L'\n') {
                if (dim > 0) {
                    buffer[dim++] = L'\0';
                    write(pipefd[1], buffer, (sizeof(wchar_t))*50);
                }
                dim = 0;
            }
            else if (carattere == L'.' || carattere == L'?' || carattere == L'!') {
                if (dim > 0) {
                    buffer[dim++] = L'\0';
                    write(pipefd[1], buffer, (sizeof(wchar_t))*50);
                }
                dim = 0;
                buffer[dim] = carattere;
                buffer[++dim] = L'\0';
                write(pipefd[1], buffer, (sizeof(wchar_t))*50);
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
        fclose(fileSorgenteP);
    }


void ricezione(struct nodo* testa, int pipefd2[]){

            close(pipefd2[0]);
            struct nodo* curr = testa; 
            int numero;
            wchar_t buffer2[50];
            wchar_t buffer3[50];
            double valore;
            int dim = 0;
            while (curr != NULL){
                numero = curr -> cardinalità_successivi;
                dim = 0;
                while ((curr -> stringa)[dim] != L'\0'){
                    buffer2[dim] = (curr-> stringa)[dim];
                    dim++;
                }
                buffer2[dim++] = L'\0';
                dim = 0;
                write(pipefd2[1], buffer2, (sizeof(wchar_t))*50);
                struct nodo_con_contatore* succ = curr -> dizionario;
                while(succ != NULL){
                    valore = succ ->percentuale;
                    buffer2[0] = L',';
                    dim = 0;
                    while (((succ -> nodo_puntato) -> stringa)[dim] != L'\0'){
                        buffer2[dim + 1] = ((succ -> nodo_puntato) -> stringa)[dim];
                        dim++;
                    }
                    dim ++;
                    buffer2[dim++] = L',';
                    buffer2[dim++] = L'\0';
                    dim = 0;
                    write(pipefd2[1], buffer2, (sizeof(wchar_t))*50);
                    if (valore != 1){
                        swprintf(buffer2, 100,L"%lf", valore);
                        write(pipefd2[1], buffer2, (sizeof(wchar_t))*50);
                        }
                    else{
                        int valore2 = (int) valore;
                        swprintf(buffer2, 100,L"%ld", valore2);
                        write(pipefd2[1], buffer2, (sizeof(wchar_t))*50);
                    }
                    succ = succ -> successivo;
                }
                curr = curr -> successivo;
                buffer2[0] = L'\n';
                buffer2[1] = L'\0';
                write(pipefd2[1], buffer2, (sizeof(wchar_t))*50);
                
            } 

}

int Task3(char* outputp, char* inputp) {
    setlocale(LC_ALL, "");
    setlocale(LC_NUMERIC, "en_US.UTF-8");
    // Apri il file per la lettura

    // Crea la pipe per la comunicazione tra padre e figlio
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Errore nella creazione della pipe");
        return 1;
    }

    // Fork: crea un nuovo processo
    pid_t pid = fork();

    if (pid == -1) {
        perror("Errore nella fork");
        return 1;
    }

    if (pid > 0) {
        int pipefd2[2];
        if (pipe(pipefd2) == -1) {
            perror("Errore nella creazione della pipe");
            return 1;
        }
        // Fork: crea un nuovo processo
        pid_t pid2 = fork();

        if (pid2 > 0){

        int dimhash = 1024; 
        int contatoreCelle = 0;
        setlocale(LC_ALL, "");
        setlocale(LC_NUMERIC, "en_US.UTF-8");

        struct nodo_hashmap** vettoreHashmap = (struct nodo_hashmap**)malloc(dimhash * sizeof(struct nodo_hashmap*));
        for (int i = 0; i < dimhash; i++) {
            vettoreHashmap[i] = NULL;
        }
        struct nodo* testa = NULL;
        struct nodo* coda = NULL;
        struct nodo* ultimo = NULL;
        struct nodo* passato = NULL;

        close(pipefd[1]);
        wchar_t buffer[50]; //DA CAMBIARE IN 32
        while (1){
            read(pipefd[0], buffer, (sizeof(buffer)));
            if (wcscmp(buffer, L"\0") == 0){
                close(pipefd[0]);
                break;
            }
            struct nodo* nodo = crea_nodo(buffer, wcslen(buffer) +1, &testa, &coda, &ultimo, &vettoreHashmap, &dimhash, &contatoreCelle);
            conteggio(nodo, &passato);
            }  
        conteggio(testa, &passato);
        calcola_percentuale(testa);
        ricezione(testa, pipefd2);
        }

        else if (pid2 == 0){
            close(pipefd2[1]);
            char* filedaAprire = outputp;
            FILE* fileAperto = fopen(filedaAprire, "w, ccs=UTF-8");
            wchar_t parola[50];
            while  (read(pipefd2[0], parola, (sizeof(wchar_t))*50) > 0){
                if (wcscmp(parola, L"\0") == 0){
                    break;
                }
                fwprintf(fileAperto,L"%ls",parola);
            }
            close(pipefd2[0]); 
        }
    }
    if (pid == 0) {
        close(pipefd[0]);  
        input(inputp, pipefd);
        close(pipefd[1]);
    }
}

void main(){
    Task3("lotr.csv", "lotr.txt");
}