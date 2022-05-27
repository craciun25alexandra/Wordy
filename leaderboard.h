#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _Node
{
    void *value;
    struct _Node* next; ///pointer catre nodul urmator
}Node;
typedef struct _List
{
    struct _Node* head; ///pointer catre capul de lista
    unsigned int size; ///numarul de elemente din lista
}List;
typedef struct _Jucator
{
    char *nume;
    int incercari;
    char *date;
}Player;

List* CreateList()
{
    List* list = (List*)malloc(sizeof(struct _List)); ///aloc memorie pentru o lista
    list->head = NULL; ///marchez faptul ca este goala punand capul de lista NNULL
    list->size = 0; ///marchez dimensiunea 0 (o lista goala nu are elemente)
    return list;
}
void AddList(List* list, void* value, int (*compare)(void*, void*)){

    Node* n = list->head;
    if(list->head == NULL) {    //daca trebuie adaugat primul element din sublista
        list->head = (Node*)malloc(sizeof(struct _Node));
        list->head->next = NULL;
        list->head->value = value;
        list ->size = 1;
        return;
    }
    if(compare(n->value, value) > 0){
        Node* aux = (Node*)malloc(sizeof(struct _Node));
        aux->value = value;
        aux->next = list->head;
        list->head = aux;
        list->size ++;
        return;
    }
    while (n->next != NULL && (compare(n->next->value, value) < 0)){
        n = n->next;
    }
        Node* aux = (Node*)malloc(sizeof(struct _Node));
        aux->value = value;
        aux->next = n->next;
        n->next = aux;
        list->size++;
}

int cmp_lexic(void* a, void*b){

    Player *A = (Player*)a;
    Player *B = (Player*)b;
    return strcmp(A->nume, B->nume);
}
int cmp_incercari(void *a, void*b){

    Player *A = (Player*)a;
    Player *B = (Player*)b;
    if(A->incercari > B->incercari) return 1;
    else return -1;

}
void freePlayer(void *p){

    Player *P = (Player*)p;
    free(P->nume);
    free(P->date);
    free(P);
}
int FindInList(List *l, void *value, int (*compare1)(void*, void*), int (*compare2)(void*, void*)){

    if(l && l->head){

        Node *n = l->head;
        if(compare1(n->value, value) == 0){
            if(compare2(n->value, value) < 0) return 0;
            else{
                Node *aux = n->next;
                freePlayer(n->value);
                free(n);
                l->head = aux;
                l->size --;
                return 1;
            }}
        while(n->next && compare1(n->next->value, value)!=0){
            n = n->next;
        }
        if (n->next){
            if(compare2(n->next->value, value) > 0){
                Node *aux = n->next->next;
                freePlayer(n->next->value);
                free(n->next);
                n->next = aux;
                l->size --;
                return 1;
            }
            else return 0;
        }
    }
    return 1;
}
void Print(void *a){

    Player *A = (Player*)a;
    printf("%s with %d attempts at %s\n", A->nume, A->incercari, A->date);
}

void FreeList(List**l, void (*freeValue)(void*)){

    Node *n = (*l)->head;
    Node *next;
    while(n){
        
        next = n->next;
        freeValue(n->value);
        free(n);
        n = next;
    }
    free(*l);
    (*l) = NULL;
}
