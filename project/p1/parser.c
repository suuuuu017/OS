//
// Created by ShiHe Wang on 2021/9/18.
//

#include "parser.h"

parserTable * returnCommandTable(char **line_pointer, int cL, int rL) {
//    command comm;

    int i = cL;
    int j = rL;

    parserTable *parserTab = malloc(sizeof(char **) * 1024);

    //TODO: can i shrink the size to write size?

    char ** commandTable = malloc(sizeof(char *) * 1024);
    char ** redirectionTable = malloc(sizeof(char *) * 1024);

    //TODO: space[]??
//    const char space[1024] = " \t\n";
    char * token;
    *line_pointer = strtok(*line_pointer, "\n'?");
//    printf("line isis %s\n", *line_pointer);
    token = strtok(*line_pointer, " \t\n");
    if(strcmp(token,">") == 0){
        redirectionTable[j] = token;
        j = j + 1;
        token = strtok(NULL, " \t\n");
        redirectionTable[j] = token;
        j = j + 1;
        printf("here");
    }
    else if (strcmp(token, ">>") == 0){
        redirectionTable[j] = token;
        j = j + 1;
        token = strtok(NULL, " \t\n");
        redirectionTable[j] = token;
        j = j + 1;
    }
    else if (strcmp(token, "<") == 0){
        //TODO: what it ">1.txt" with no space
        redirectionTable[j] = token;
        j = j + 1;
        token = strtok(NULL, " \t\n");
        redirectionTable[j] = token;
        j = j + 1;
        int input = 0;
        if ((input = open(token, O_RDONLY)) < 0) {
            fprintf(stderr, "error\n");
        }
    }
    else{
        commandTable[0] = token;
        i = i + 1;
    }
//    printf("command is %s", token);
    while(token != NULL){
        token = strtok(NULL, " \t\n");
        //TODO: why double check token == null?
//        printf("wtf  is %s\n", token);
        if(token){
            if (strcmp(token, ">") == 0){
                //TODO: what it ">1.txt" with no space
                //TODO: wrong writing template
                redirectionTable[j] = token;
//                printf("direction symbol is %s\n", token);
                j = j + 1;
                token = strtok(NULL, " \t\n");
                redirectionTable[j] = token;
                j = j + 1;
            }
//                printf("i is %i\n", i);
            else if (strcmp(token, ">>") == 0){
                //TODO: what it ">1.txt" with no space
                redirectionTable[j] = token;
//                printf("direction symbol is %s\n", token);
                j = j + 1;
                token = strtok(NULL, " \t\n");
                redirectionTable[j] = token;
                j = j + 1;
//                printf("file name is %s\n", token);
//                //TODO: is this open correct and the 0644
            }
            else if (strcmp(token, "<") == 0){
                //TODO: what it ">1.txt" with no space
                redirectionTable[j] = token;
//                printf("direction symbol is %s\n", token);
                j = j + 1;
                token = strtok(NULL, " \t\n");
                redirectionTable[j] = token;
                j = j + 1;
//                printf("file name is %s\n", token);
//                TODO: is this open correct
                int input = 0;
                if ((input = open(token, O_RDONLY)) < 0) {
                    fprintf(stderr, "error\n");
                }
            }
            else{
//                printf("argument is %s, i is %i\n", token, i);
                commandTable[i] = token;
                i = i + 1;
            }
        }
    }

    parserTab->commandLength = i;
    parserTab->redTabLength = j;
    parserTab->commandTable = commandTable;
    parserTab->redirectionTable = redirectionTable;
//    printf("here??");

    return parserTab;
}

void redir(char ** redirectionTable, int redTabLength){
    for(int k = 0; k < redTabLength; k++){
//        char * tmp = redirectionTable[k];
//        printf("tmp is %s\n", tmp);
        if(strcmp(redirectionTable[k], "<") == 0){
//            printf("wht\n");
            k = k + 1;
            char * fn = redirectionTable[k];
            int input = open(fn, O_RDONLY);
//            printf("fn is %s\n", fn);
            dup2(input, STDIN_FILENO);
            close(input);
        }
        else if (strcmp(redirectionTable[k], ">") == 0){
            //TODO: what it ">1.txt" with no space
            //TODO: wrong writing template
            k = k + 1;
            char * fn = redirectionTable[k];
//            printf("file name is %s\n", fn);
            int overwrite = creat(fn, 0644);
            dup2(overwrite, STDOUT_FILENO);
            close(overwrite);
        }
        else if (strcmp(redirectionTable[k], ">>") == 0){
            //TODO: what it ">1.txt" with no space
            //TODO: wrong writing template
            k = k + 1;
            char * fn = redirectionTable[k];
//            printf("file name is %s\n", fn);
            int append = open(fn, O_CREAT | O_RDWR | O_APPEND, 0644);
            dup2(append, STDOUT_FILENO);
            close(append);
        }
    }
}

char * addspace(char * line, long length){
    int flag = 0;
    for(long i = 0; i < length - 1; i++){
        char tmp = line[i];
//        printf("ptr isis %c\n", tmp);
        if(tmp == '>' || tmp == '<' || strcmp(&tmp, ">>") == 0){
//            printf("jere\n");
            if(line[i+1] == '>'){
//                printf("jjjjere\n");
                flag = 1;
            }
            if(!flag){
                for (long j = length + 16; j >= i; j--) {
                    line[j + 1] = line[j];
                }
                line[i] = ' ';
                for (long j = length + 16; j >= i + 1; j--) {
                    line[j + 1] = line[j];
                }
                line[i + 2] = ' ';
                i = i + 1;
            }
            else{
                for (long j = length + 16; j >= i + 1; j--) {
                    line[j + 1] = line[j];
                }
                line[i] = ' ';
                for (long j = length + 16; j >= i + 2; j--) {
                    line[j + 1] = line[j];
                }
                line[i + 3] = ' ';
                i = i + 2;
            }
        }
    }
    return line;
}
