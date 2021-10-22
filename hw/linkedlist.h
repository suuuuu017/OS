//
// Created by ShiHe Wang on 2021/10/20.
//

#ifndef HW3_VE482_LINKEDLIST_H
#define HW3_VE482_LINKEDLIST_H

typedef struct node_t{
    char * key;
    void * data;
    struct node_t * next;
} node;

void readFile(char * filename, node * linkedlist);

#endif //HW3_VE482_LINKEDLIST_H
