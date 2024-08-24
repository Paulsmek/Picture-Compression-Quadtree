#ifndef _DOUBLE_LINKED_LIST_H_
#define _DOUBLE_LINKED_LIST_H_
//Lavric Paul 312CD

typedef struct {
    unsigned char red, green, blue;
} Pixel;


typedef struct arbore_cuaternal {
    
    struct arbore_cuaternal * top_left;
    struct arbore_cuaternal * top_right;
    struct arbore_cuaternal * bottom_left;
    struct arbore_cuaternal * bottom_right;
    unsigned int width, height;
    unsigned char type; //daca e sau nu nod terminal/frunza
    Pixel culoare;
    int x, y; //pozitia din stanga sus
} Nod_arbore;

typedef struct node {
    Nod_arbore *arbore;
    struct node* next;
} TNode, TSortedList; //nod de coada ce contine nod de arbore

typedef struct queue{
    TNode *head;
    TNode *tail;
    int size;
} TQueue; //coada

TSortedList *coada_create_nod(Nod_arbore *arbore) {
    TSortedList *rez = (TSortedList*) malloc(sizeof(TSortedList));
    rez -> arbore = arbore;
    rez -> next = NULL; 
    return rez;
}

TQueue *create_queue(Nod_arbore *arbore){
    TQueue *rezz = (TQueue*) malloc(sizeof(TQueue));
    rezz -> head = rezz -> tail = coada_create_nod(arbore);
    rezz -> size = 1;
    return rezz;
}

TQueue *enqueue(TQueue *queue, Nod_arbore * arbore){
    TNode *nod;
    if (queue == NULL) return create_queue(arbore);
    nod = coada_create_nod(arbore);
    queue -> tail -> next = nod;
    queue -> tail = nod;
    queue -> size++;
    return queue;

}
TQueue *dequeue(TQueue *queue){
    TNode *nod;
    nod = queue -> head;
    queue -> head = queue -> head -> next;
    if (nod) free(nod);
    queue -> size--;
    return queue;
    

}

#endif