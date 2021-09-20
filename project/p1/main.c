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

        if (strcmp(line, "exit\n") == 0){
            printf("mumsh $ exit\n");
            notExit = 0;
        }
        else {
            parserTable * parsTab;
            int commandLength = 1;
            int redTabLength = 0;
            //TODO: weird behaviour when add two parameters
            parsTab = returnCommandTable(line_pointer, length, 1, 0);
            char ** commandTable = parsTab->commandTable;
            char ** redirectionTable = parsTab->redirectionTable;
            commandLength = parsTab->commandLength;
            redTabLength = parsTab->redTabLength;
            printf("the command comes out is %i\n", commandLength);
            printf("the redir comes out is %i\n", redTabLength);
            //TODO: weird processing order when input redirection is involved
//            char ** commandTable = returnCommandTable(line_pointer, length);
            pid = fork();
            if (pid == 0) {
                for(int k = 0; k < redTabLength; k++){
                    if(strcmp(redirectionTable[k], "<") == 0){
                        char * fn = redirectionTable[k + 1];
                        int input = open(fn, O_RDONLY);
                        printf("fn is %s\n", fn);
                        dup2(input, STDIN_FILENO);
                        close(input);
                    }
                }
                execvp(commandTable[0], commandTable);
            }
//            execvp(commandTable[0], commandTable);
        }
    } while (notExit);
}