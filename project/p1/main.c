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
    long length = 0;

    //pid for executor
    pid_t pid;

//    int tmp = 1024;
//    while(fgets(line, tmp, stdin)){
//        printf("line is %s", line);
//    }
//    fgets(line, tmp, stdin);
//    fgets(line, tmp, stdin);
    length = getline(line_pointer, &size, stdin);
//    printf("line is %s\n", line);
//    printf("lengt is %zu\n", length);
    line = addspace(line, length);
//    printf("line is %s\n", line);
//    printf("lengt is %zu\n", length);
    do{
        //just exit special case
        //TODO: wrong waiting prompt
        printf("mumsh $ ");
        fflush(stdout);
//        getline(line_pointer, &size, stdin);
//        printf("line is %s", line);

        //TODO: does this exit have to be followed by a new line?
        if (strcmp(line, "exit\n") == 0){
            printf("exit\n");
            notExit = 0;
            return 0;
        }
        else {
            int status = 0;
            pid = fork();
            if (pid == 0) {
                parserTable *parsTab;
                int commandLength;
                int redTabLength;
                //TODO: weird behaviour when add two parameters
                //TODO: free up the
                line = addspace(line, length);
//            printf("line is %s\n", line);
                parsTab = returnCommandTable(line_pointer, 0, 0);
                char ** commandTable = {0};
                commandTable = parsTab->commandTable;
                char ** redirectionTable = {0};
                redirectionTable = parsTab->redirectionTable;
                commandLength = parsTab->commandLength;
                redTabLength = parsTab->redTabLength;
//                printf("the command comes out is %i\n", parsTab->commandLength);
//            printf("the redir comes out is %i\n", parsTab->redTabLength);
                //TODO: weird processing order when input redirection is involved
//            int status = 0;
//            pid = fork();
//            if (pid == 0) {
                if (redirectionTable[0]) {
                    redir(redirectionTable, redTabLength);
                }
//                printf("now command tab is %s\n", commandTable[0]);
//                printf("HC: hello from child\n");
                char *argv[1024] = {0};
                for(int tmp = 0; tmp < commandLength; tmp++){
                    argv[tmp] = commandTable[tmp];
                }
//                argv[1] = "-la";
//                argv[2] = NULL;

                if (execvp(commandTable[0], argv) == -1) {
                    printf("false\n");
                }
//                if (execvp(commandTable[0], commandTable) == -1) {
//                    printf("false\n");
//                }
//                printf("pid isis %i\n", pid);
//                printf("now command tab is %s\n", commandTable[0]);
//                execvp(commandTable[0], commandTable);
//                printf("HC: bye from child\n");
//                free(parsTab->commandTable);
//                free(parsTab->redirectionTable);
//                free(parsTab);
//                line = " ";
//                free(parsTab->commandTable);
//                free(parsTab->redirectionTable);
//                free(parsTab);
            }
            else {
//                printf("HP: hello from parent, pid is %i\n", pid);
//                wait(&status);
//                printf("CT: child has terminated\n");
                do {
                    waitpid(pid, &status, WUNTRACED);
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            }
////            execvp(commandTable[0], commandTable);
////            free(parsTab->commandTable);
////            free(parsTab->redirectionTable);
////            free(parsTab);
        }
//        *line_pointer = "sss";
//        free(line_pointer);
    }while(notExit && ((length = getline(line_pointer, &size, stdin)) >=0));
}
