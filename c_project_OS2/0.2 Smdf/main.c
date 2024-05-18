#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <wchar.h>

// Prototipi delle funzioni
void Task1(char* csvFileName, char* txtFileName);
void Task2(char* filename, int words_requested, wchar_t* parola);

int main() {
    setlocale(LC_ALL, ""); // Imposta la localizzazione per supportare UTF-8
    
    int choice;
    printf("Seleziona l'azione da eseguire:\n");
    printf("1. Esegui Task1\n");
    printf("2. Esegui Task2\n");
    printf("3. Esegui entrambi\n");
    printf("Scelta: ");
    scanf("%d", &choice);

    while (choice < 1 || choice > 3) {
        printf("Scelta non valida. Riprova: ");
        scanf("%d", &choice);
    }

    switch (choice) {
        
        case 1: {
            // Esegui solo il Task1
            char csvFileName[100];
            char txtFileName[100];
            printf("Inserisci il nome del file CSV: ");
            scanf("%s", csvFileName);
            printf("Inserisci il nome del file di testo: ");
            scanf("%s", txtFileName);
            Task1(csvFileName, txtFileName);
            printf("Task1 completato.\n");
            break;
        }

        case 2: {
            // Esegui solo il Task2
            wchar_t parola[100];
            char filename[100];
            int words_requested;
            char choice2;
            bool valid = false;
            while (!valid) {
                printf("Inserisci il nome del file: ");
                scanf("%s", filename);
                printf("Inserisci il numero di parole richieste: ");
                if (scanf("%d", &words_requested) < 1) {
                    printf("Valore non valido. Riprova.\n");
                    while (getchar() != '\n'); // Pulisce il buffer di input
                } else {
                    valid = true;
                }
            }
            while (true) {
                printf("Vuoi inserire la parola? (s/n): ");
                scanf(" %c", &choice2);
                if (choice2 == 's' || choice2 == 'S') {
                    printf("Inserisci parola: ");
                    scanf("%ls", parola);
                    break;
                } else if (choice2 == 'n' || choice2 == 'N') {
                    parola[0] = '\0';
                    break;
                } else {
                    printf("Scelta non valida. Riprova.\n");
                }
            }
            Task2(filename, words_requested, parola);
            printf("Task2 completato.\n");
            break;
        }

        case 3: {
            wchar_t parola[1000];
            // Esegui entrambi i task
            char csvFileName[100];
            char txtFileName[100];
            char choice2;
            printf("Inserisci il nome del file CSV: ");
            scanf("%s", csvFileName);
            printf("Inserisci il nome del file di testo: ");
            scanf("%s", txtFileName);
            Task1(csvFileName, txtFileName);
            printf("Task1 completato.\n");
            int words_requested;
            bool valid = false;
            while (!valid) {
                printf("Inserisci il numero di parole richieste: ");
                if (scanf("%d", &words_requested) < 1) {
                    printf("Valore non valido. Riprova.\n");
                    while (getchar() != '\n'); // Pulisce il buffer di input
                } else {
                    valid = true;
                }
            }
            while (true) {
                printf("Vuoi inserire la parola? (s/n): ");
                scanf(" %c", &choice2);
                if (choice2 == 's' || choice2 == 'S') {
                    printf("Inserisci parola: ");
                    scanf("%ls", parola);
                    break;
                } else if (choice2 == 'n' || choice2 == 'N') {
                    parola[0] = '\0';
                    break;
                } else {
                    printf("Scelta non valida. Riprova.\n");
                }
            }
            Task2(csvFileName, words_requested, parola);
            printf("Task2 completato.\n");
            break;
        }
    }

    return 0;
}
