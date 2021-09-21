//
// Created by ShiHe Wang on 2021/9/18.
//

#include "parser.h"

parserTable * returnCommandTable(char **line_pointer, size_t length, int cL, int rL) {
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
//    printf("command is %s", token);
    int i = cL;
    int j = rL;
    while(token != NULL){
        token = strtok(NULL, space);
        //TODO: why double check token == null?
        printf("wtf  is %s\n", token);
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
//                int overwrite = creat(token, 0644);
//                dup2(overwrite, STDOUT_FILENO);
//                close(overwrite);
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
//                //TODO: is this open correct and the 0644
//                int append = open(token, O_CREAT | O_RDWR | O_APPEND, 0644);
//                dup2(append, STDOUT_FILENO);
//                close(append);
            }
            else if (strcmp(token, "<") == 0){
                //TODO: what it ">1.txt" with no space
                redirectionTable[j] = token;
//                printf("direction symbol is %s\n", token);
                j = j + 1;
                token = strtok(NULL, space);
                redirectionTable[j] = token;
                j = j + 1;
                printf("file name is %s\n", token);
//                TODO: is this open correct
                int input = 0;
                if ((input = open(token, O_RDONLY)) < 0) {
                    fprintf(stderr, "error\n");
                }
//                input = open(token, O_RDONLY, 0);
                printf("input code is %i\n", input);
//                dup2(input, STDIN_FILENO);
//                close(input);
            }
            else{
                printf("argument is %s, i is %i\n", token, i);
                commandTable[i] = token;
                i = i + 1;
            }
        }
    }

//    parserTab->commandLength = i;
//    parserTab->redTabLength = j;
    parserTab->commandTable = commandTable;
    parserTab->redirectionTable = redirectionTable;
//    printf("here??");

    return parserTab;
}
