//
// Created by ShiHe Wang on 2021/10/20.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linkedlist.h"

int mode = -1;
int type = -1;
int llLength = 0;

int randE(const void *a, const void *b){
    return (((rand())%2)*2-1);
}

int incInt(const void *a, const void *b){
    return *((int *)(((node*) a)->data)) - *((int *)(((node*) b)->data));
}

int decInt(const void *a, const void *b){
    return *((int *)(((node*) b)->data)) - *((int *)(((node*) a)->data));
}

int incDouble(const void *a, const void *b){
    return *(double *)(((node*) a)->data) > *(double *)(((node*) b)->data);
}

int decDouble(const void *a, const void *b){
    return *((double *)(((node*) b)->data)) > *((double *)(((node*) a)->data));
}

int incChar(const void *a, const void *b){
    return strcmp((char *)(((node*) a)->data), (char *)(((node*) b)->data));
}

int decChar(const void *a, const void *b){
    return strcmp(((char *)(((node*) b)->data)), ((char *)(((node*) a)->data)));
}

int (*cmp[3][3]) (const void *, const void *) = {
        {randE, incInt, decInt},
        {randE, incDouble, decDouble},
        {randE, incChar, decChar}
};


void readFile(char * filename, node * linkedList){
    FILE * fptr;
    fptr = fopen(filename, "r");

    char * line = NULL;
    size_t size = 1024;
    long length = 0;
    node * tmp = linkedList;
    char * token;
    while ((length = getline(&line, &size, fptr)) > 1) {
        int i = 0;
        tmp->next = (node *)malloc(sizeof(node));
        tmp = tmp->next;
        while ((token = strsep(&line, "=\n")) != NULL && i < 2){
            if(token){
                if(i == 0){
                    tmp->key = token;
                }
                else if(i == 1){
                    if(type == 0) {
                        tmp->data = (void *) malloc(sizeof(int));
                        *(int *) tmp->data = (atoi(token));
                    }
                    if(type == 1) {
                        tmp->data = (void *) malloc(sizeof(double ));
                        *(double *) tmp->data = (atof(token));
                    }
                    if(type == 2) {
                        tmp->data = (void *) malloc(sizeof(char) * strlen(token));
                        strcpy((char *) tmp->data, token);
                    }
                }
                i = i + 1;
            }
        }
        llLength = llLength + 1;
    }
    free(line);
    fclose(fptr);
}

int main(int argc, char **argv){
    char * filename = argv[1];

    //deal with the type identify
    char line[256];
    char * typeString;
    strcpy(line, argv[1]);
    typeString = strtok(line,"_");
    typeString = strtok(NULL,".");

    if(strcmp(typeString, "int") == 0){
        type = 0;
    }
    else if(strcmp(typeString, "double") == 0){
        type = 1;
    }
    else if(strcmp(typeString, "char") == 0){
        type = 2;
    }

    //deal with sorting type
    if(strcmp(argv[2], "rand") == 0){
        mode = 0;
    }
    else if(strcmp(argv[2], "inc") == 0){
        mode = 1;
    }
    else if(strcmp(argv[2], "dec") == 0){
        mode = 2;
    }

    //create linked list and input
    node * linkedList = (node *)malloc(sizeof(node));

    printf("reading %s\n", argv[1]);
    readFile(filename, linkedList);

    node *curr = linkedList->next;

    //use qsort to sort list
    node *nodeArray = malloc(sizeof(node) * (llLength));
    curr = linkedList->next;
    for (int i = 0; i < llLength; i++)
    {
        memcpy(nodeArray + i, curr, sizeof(node));
        curr = curr->next;
    }

    printf("sorting elements\n");
    qsort(nodeArray, llLength, sizeof(node), cmp[type][mode]);

    //copy the array back to the linked list
    curr = linkedList->next;
    for (int i = 0; i < llLength; i++)
    {
        curr->key = (nodeArray + i)->key;
        curr->data = (nodeArray + i)->data;
        curr = curr->next;
    }

    free(nodeArray);

    FILE * fp;
    char fn[1024];
    strcat(fn, argv[2]);
    strcat(fn, "_");
    strcat(fn, typeString);
    strcat(fn, ".txt");
    printf("writing %s\n", fn);
    fp = fopen(fn, "w");
    curr = linkedList->next;
    for (int i = 0; i < llLength; i++)
    {
        if(strcmp(typeString, "int") == 0){
            fprintf(fp, "%s=%i\n", curr->key, *(int *)curr->data);
        }
        else if(strcmp(typeString, "double") == 0){
            fprintf(fp, "%s=%f\n", curr->key, *(double *)curr->data);
        }
        else if(strcmp(typeString, "char") == 0){
            fprintf(fp, "%s=%s\n", curr->key, (char *)curr->data);
        }
        curr = curr->next;
    }
    fclose(fp);

    curr = linkedList->next;
    for (int i = 0; i < llLength; i++)
    {
        free(curr->key);
        free(curr->data);
        node * victim = curr;
        free(curr);
        curr = victim->next;
    }
    free(curr);
    free(linkedList->key);
    free(linkedList->data);
    free(linkedList);
}