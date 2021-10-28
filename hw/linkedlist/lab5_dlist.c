//
// Created by ShiHe Wang on 2021/10/27.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lab5_dlist.h"

typedef struct node_t{
    char * key;
    dlistValue data;
    struct node_t * next;
} node;

typedef struct linkedlist_t{
    node * list;
    unsigned long length;
    dlistValueType listType;
} linkedlist;

int randE(){
    return (((rand())%2)*2-1);
}

int incInt(const void *a, const void *b){
    return ((node*) a)->data.intValue - ((node*) b)->data.intValue;
}

int decInt(const void *a, const void *b){
    return ((node*) b)->data.intValue - ((node*) a)->data.intValue;
}

int incDouble(const void *a, const void *b){
    return ((node*) a)->data.doubleValue > ((node*) b)->data.doubleValue;
}

int decDouble(const void *a, const void *b){
    return ((node*) a)->data.doubleValue < ((node*) b)->data.doubleValue;
}

int incChar(const void *a, const void *b){
    return strcmp((((node*) a)->data.strValue), (((node*) b)->data.strValue));
}

int decChar(const void *a, const void *b){
    return strcmp((((node*) b)->data.strValue), (((node*) a)->data.strValue));
}

int (*cmp[3][3]) (const void *, const void *) = {
        {randE, incInt, decInt},
        {randE, incChar, decChar},
        {randE, incDouble, decDouble}
};

dlist createDlist(dlistValueType type) {
    if(type == DLIST_UNKOWN){
        return NULL;
    }
    dlist ll = (linkedlist *)malloc(sizeof(linkedlist));
    ((linkedlist*)ll)->length = 0;
    ((linkedlist*)ll)->list = (node *)malloc(sizeof(node));
//    ((linkedlist*)ll)->list = NULL;
    ((linkedlist*)ll)->list->next = NULL;
    ((linkedlist*)ll)->list->key = NULL;
    ((linkedlist*)ll)->list->data.strValue = NULL;
    ((linkedlist*)ll)->listType = type;
    return ll;
}

int dlistIsEmpty(dlist_const this) {
    if(((linkedlist *)this)->length == 0){
        return 1;
    }
    return 0;
}

void dlistAppend(dlist this, const char *key, dlistValue value) {
    unsigned long i = 0;
    node * target = ((linkedlist *)this)->list;
    while(i < ((linkedlist *)this)->length){
        target = target->next;
//        printf("now key and value are %s, %i\n", (target->key), target->data.intValue);
        i = i + 1;
    }
    target->next = (node *)malloc(sizeof(node));
    target = target->next;
    target->next = NULL;
    target->key = malloc(strlen(key) + 1);
    strcpy(target->key , key);
    if(((linkedlist *)this)->listType == DLIST_STR){
//        printf("here\n");
        target->data.strValue = malloc(strlen(value.strValue) + 1);
        strcpy(target->data.strValue, value.strValue);
    }
    else{
        target->data = value;
    }
//    printf("length is %lu\n", ((linkedlist *)this)->length);
    ((linkedlist *)this)->length = ((linkedlist *)this)->length + 1;
}

void dlistPrint(dlist_const this) {
    unsigned long i = 0;
    node * itr = ((linkedlist *)this)->list->next;
    while(i < ((linkedlist *)this)->length) {
        if(((linkedlist *)this)->listType == DLIST_INT){
            printf("%s=%i\n", itr->key, itr->data.intValue);
        }
        if(((linkedlist *)this)->listType == DLIST_DOUBLE){
            printf("%s=%f\n", itr->key, itr->data.doubleValue);
        }
        if(((linkedlist *)this)->listType == DLIST_STR){
            printf("%s=%s\n", itr->key, itr->data.strValue);
        }
        itr = itr->next;
        i = i + 1;
    }
}

void dlistSort(dlist_const this, dlist listDst, dlistSortMethod method) {
    node *nodeArray = malloc(sizeof(node) * (((linkedlist *)this)->length + 1));
    node * curr = ((linkedlist *)this)->list->next;
    for(unsigned long i = 0; i < ((linkedlist *)this)->length; i++){
        memcpy(nodeArray + i, curr, sizeof(node));
        curr = curr->next;
    }
    qsort(nodeArray, ((linkedlist *)this)->length, sizeof(node),
            cmp[((linkedlist *)this)->listType - 1][method-1]);

    //TODO: if listDst is not empty
//    curr = ((linkedlist *)this)->list->next;
//    dlistFree(listDst);
//    listDst = createDlist(((linkedlist *)this)->listType);
    if(((linkedlist *)listDst)->length > 0){
        node * itr = ((linkedlist *)listDst)->list->next;
        node * next = ((linkedlist *)listDst)->list->next;
        while(next){
            free(itr->key);
            if(((linkedlist *)listDst)->listType == DLIST_STR){
                free(itr->data.strValue);
            }
            next = itr->next;
            free(itr);
            itr = next;
        }
    }
    ((linkedlist *)listDst)->list->next = NULL;
    ((linkedlist *)listDst)->length = 0;

    for(unsigned long i = 0; i < ((linkedlist *)this)->length; i++){
//        printf("appending\n");
        dlistAppend(listDst, (nodeArray + i)->key, (nodeArray + i)->data);
    }

    free(nodeArray);

}

void dlistFree(dlist this) {
    if(dlistIsEmpty(this)){
        free(((linkedlist*)this)->list->key);
        free(((linkedlist*)this)->list);
        free((linkedlist*)this);
        return;
    }
    node * itr = ((linkedlist *)this)->list;
    node * next = ((linkedlist *)this)->list;
    while(next){
        free(itr->key);
//        free(itr->data.strValue);
        if(((linkedlist *)this)->listType == DLIST_STR){
            free(itr->data.strValue);
        }
        next = itr->next;
        free(itr);
        itr = next;
    }
//    free(itr->key);
//    free(itr);
//    free(((linkedlist*)this)->list->key);
//    free(((linkedlist*)this)->list);
    free((linkedlist*)this);
}


