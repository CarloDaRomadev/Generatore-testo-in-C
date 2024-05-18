
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

struct nodo_hashmap
{
    wchar_t* parola;
    struct nodo_hashmap* succcessivo;
};

void inserisciInHash(struct nodo_hashmap** hashmapptr, wchar_t* stringa, int dim){
    int valore = 0;
    int i = 0;
    while (stringa[i] != L'\0'){
        valore += (int) stringa[i];
        i++;
    }
    valore = valore % dim;
    struct nodo_hashmap* hashmap = &(*hashmapptr[valore]);

    if (hashmap == 0){
        (hashmap) -> parola = stringa;
        (hashmap) -> succcessivo = NULL;
    }
    else{
        struct nodo_hashmap* succ = (hashmap) -> succcessivo;
        while (succ != NULL){
            succ = succ -> succcessivo;
        }
        succ -> parola = stringa;
        succ -> succcessivo = NULL;
    }
}

int main(){
    int dim = 1000;
    wchar_t stringa[10]= "lezione 7";
    struct nodo_hashmap* vettoreHashmap = (struct nodo_hashmap*) calloc(dim,dim * sizeof(struct nodo_hashmap));
    inserisciInHash(&vettoreHashmap,stringa, dim);
    return 1;
}