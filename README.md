# Generatore di Testo in C

## Panoramica
Questo progetto è stato prodotto per il corso "Sistemi operativi (Modulo II)" in La sapienza. Anno accademico 2023/2024

Questo progetto implementa un generatore di testo che può:
1. Analizzare un file di testo per costruire un modello statistico di sequenze di parole
2. Generare nuovo testo basato sui modelli appresi
3. Funzionare con testo codificato in UTF-8 in più lingue
4. Operare sia tramite chiamate dirette che comunicazione tra processi

Il sistema è composto da quattro task principali che dimostrano diverse funzionalità.

## Struttura del Progetto

Il codice è organizzato in diversi componenti:

1. **Task Principali**:
   - `Task1`: Costruisce il modello di Markov da un file di testo e lo salva in CSV
   - `Task2`: Genera testo da un modello CSV pre-costruito
   - `Task3`: Implementa la costruzione del modello usando comunicazione tra processi
   - `Task4`: Implementa la generazione di testo usando comunicazione tra processi

2. **Componenti Base**:
   - Hashmap per la ricerca efficiente delle parole
   - Strutture a lista collegata per memorizzare le relazioni tra parole
   - Calcolo delle probabilità per le transizioni tra parole
   - Elaborazione di testo UTF-8

## Requisiti

- Compilatore C con supporto C11
- Sistema POSIX-compliant (per le funzionalità pipe/fork nei Task 3 e 4)
- Supporto per locale UTF-8

## Utilizzo

### Compilazione

Compilare con un compilatore C che supporti C11 e collegare le librerie necessarie:

```bash
gcc -std=c11 -o markov main.c
```

### Esecuzione

1. **Task 1**: Costruire il modello da testo
   ```c
   Task1("output.csv", "input.txt");
   ```

2. **Task 2**: Generare testo dal modello
   ```c
   Task2("model.csv", 20, L"parola_iniziale");
   ```

3. **Task 3**: Costruire il modello con IPC
   ```c
   Task3("output.csv", "input.txt");
   ```

4. **Task 4**: Generare testo con IPC
   ```c
   Task4("model.csv", 20, L"");
   ```

## Strutture Dati

Le strutture dati principali sono definite in `strutture_dati.h`:

1. `struct nodo`: Rappresenta una parola/nodo nella catena di Markov
2. `struct nodo_con_contatore`: Tiene traccia delle parole successive e delle loro probabilità
3. `struct nodo_hashmap`: Voce della hashmap per la ricerca efficiente delle parole

## Esempio

Per costruire un modello da "lotr.txt" e generare 10 parole partendo da un segno di punteggiatura casuale:

```c
Task4("lotr.csv", 10, L"");
```

Purtroppo il progetto non è completo e presenta alcuni bug consistenti.
