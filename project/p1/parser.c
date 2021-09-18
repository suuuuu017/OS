//
// Created by ShiHe Wang on 2021/9/18.
//

#include "parser.h"

typedef struct{
    char * cmd;
    char * argument[10];
} command;

typedef struct{
    command * table[10];
} commandTable;

char ** returnCommandTable(char **line_pointer, size_t length) {
//    command comm;
    char ** commandTable = malloc(sizeof(char *) * length);
    const char space[2] = " \n";
    char * token;
//            printf("line is %s", *line_pointer);
    token = strtok(*line_pointer, space);
    commandTable[0] = token;
//            printf("command is %s", token);
    int i = 1;
    while(token != NULL){
        token = strtok(NULL, space);
//                printf("i is %i\n", i);
        commandTable[i] = token;
        i = i + 1;
//                printf("argument is %s", token);
    }
    return commandTable;
}
