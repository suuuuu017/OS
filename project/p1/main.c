//
// Created by ShiHe Wang on 2021/9/17.
//

#include "main.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    //exit flag
    int notExit = 1;

    //input line setup
    char l[1024];
    char * line = l;
    size_t size = 1024;
    char **line_pointer = &line;
    size_t length = 0;

    //pid for executor
    pid_t pid = 0;

    do{
        //just exit special case
        //TODO: wrong waiting prompt
        printf("mumsh $ ");
        length = getline(line_pointer, &size, stdin);
//        printf("The number of characters getline() read: %zu\n", characters);
//        printf("You typed: %s\n",line);
        if (strcmp(line, "exit\n") == 0){
            printf("mumsh $ exit\n");
            notExit = 0;
        }
        else {
            parserTable * parsTab;
            parsTab = returnCommandTable(line_pointer, length);
            char ** commandTable = parsTab->commandTable;
//            char ** commandTable = returnCommandTable(line_pointer, length);
            pid = fork();
            if (pid == 0) {
                execvp(commandTable[0], commandTable);
            }
//            execvp(commandTable[0], commandTable);
        }
    } while (notExit);
}
