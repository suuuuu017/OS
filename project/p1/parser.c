//
// Created by ShiHe Wang on 2021/9/18.
//

#include "parser.h"

typedef struct{
    char * cmd;
    char * argument[10];
} command;

//typedef struct{
//    char ** commandTable;
//    char ** redirectionTable;
//} parserTable;

parserTable * returnCommandTable(char **line_pointer, size_t length) {
//    command comm;
    parserTable *parserTab = malloc(sizeof(char **) * 2);

    //TODO: can i shrink the size to write size?

    char ** commandTable = malloc(sizeof(char *) * length);
    char ** redirectionTable = malloc(sizeof(char *) * length);

    const char space[2] = " \n";
    char * token;
//            printf("line is %s", *line_pointer);
    token = strtok(*line_pointer, space);
    commandTable[0] = token;
//            printf("command is %s", token);
    int i = 1;
    int j = 0;
    while(token != NULL){
        token = strtok(NULL, space);
        //TODO: why double check token == null?
        if(token){
            if (strcmp(token, ">") == 0){
                //TODO: what it ">1.txt" with no space
                //TODO: wrong writing template
                redirectionTable[j] = token;
                printf("direction symbol is %s\n", token);
                j = j + 1;
                token = strtok(NULL, space);
                redirectionTable[j] = token;
                j = j + 1;
                printf("file name is %s\n", token);
                int overwrite = creat(token, 0644);
                dup2(overwrite, STDOUT_FILENO);
                close(overwrite);
            }
//                printf("i is %i\n", i);
            else if (strcmp(token, ">>") == 0){
                //TODO: what it ">1.txt" with no space
                redirectionTable[j] = token;
                printf("direction symbol is %s\n", token);
                j = j + 1;
                token = strtok(NULL, space);
                redirectionTable[j] = token;
                j = j + 1;
                printf("file name is %s\n", token);
                //TODO: is this open correct and the 0644
                int append = open(token, O_CREAT | O_RDWR | O_APPEND, 0644);
                dup2(append, STDOUT_FILENO);
                close(append);
            }
            else{
                commandTable[i] = token;
                i = i + 1;
//            printf("argument is %s", token);
            }
        }
    }
    parserTab->commandTable = commandTable;
    parserTab->redirectionTable = redirectionTable;
    return parserTab;
}
