//
// Created by ShiHe Wang on 2021/9/17.
//

#include "main.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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
    pid_t pid;

    do{
        //just exit special case
        //TODO: wrong waiting prompt
        printf("mumsh $ ");
        length = getline(line_pointer, &size, stdin);

        if (strcmp(line, "exit\n") == 0){
            printf("mumsh $ exit\n");
            notExit = 0;
            break;
        }
        else {
            parserTable * parsTab;
//            int commandLength;
//            int redTabLength;
            //TODO: weird behaviour when add two parameters
            //TODO: free up the space
            parsTab = returnCommandTable(line_pointer, length, 1, 0);
            char ** commandTable = parsTab->commandTable;
            char ** redirectionTable = parsTab->redirectionTable;
//            commandLength = parsTab->commandLength;
//            redTabLength = parsTab->redTabLength;
//            printf("the command comes out is %i\n", parsTab->commandLength);
//            printf("the redir comes out is %i\n", parsTab->redTabLength);
            //TODO: weird processing order when input redirection is involved
//            char ** commandTable = returnCommandTable(line_pointer, length);
//            for(int k = 0; k < 3; k++){
//                if(strcmp(redirectionTable[k], "<") == 0){
//                    char * fn = redirectionTable[k + 1];
//                    int input = open(fn, O_RDONLY);
//                    printf("fn is %i\n", k);
//                    dup2(input, STDIN_FILENO);
////                    close(input);
//                }
//            }
            int status = 0;
            pid = fork();
            if (pid == 0) {
                if(redirectionTable[0]){
                    redir(redirectionTable);
                }
                printf("now command tab is %s\n", commandTable[0]);
                printf("HC: hello from child\n");
                if(execvp(commandTable[0], commandTable) == -1){
                    printf("false\n");
                }
//                printf("pid isis %i\n", pid);
                printf("now command tab is %s\n", commandTable[0]);
//                execvp(commandTable[0], commandTable);
                printf("HC: bye from child\n");
            }
            else {
//                printf("HP: hello from parent, pid is %i\n", pid);
//                wait(&status);
//                printf("CT: child has terminated\n");
                do {
                    waitpid(pid, &status, WUNTRACED);
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            }
//            execvp(commandTable[0], commandTable);
//            free(commandTable);
//            free(redirectionTable);
//            free(parsTab);
        }
//        free(line_pointer);
    }while(notExit);
}