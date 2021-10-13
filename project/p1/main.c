//
// Created by ShiHe Wang on 2021/9/17.
//
#include "main.h"

static sigjmp_buf env;
char **commandTable = {0};
char **redirectionTable = {0};
parserTable *parsTab;

void sigint_handler() {
    if(commandTable){
        free(commandTable);
        commandTable = NULL;
    }
    if(redirectionTable){
        free(redirectionTable);
        redirectionTable = NULL;
    }
    if(parsTab){
        free(parsTab);
        parsTab = NULL;
    }
    siglongjmp(env, 42);
}

int main(){
    //exit flag
    int notExit = 1;

    //input line setup
    char l[1024];
    char * line = l;
    size_t size = 1024;
    char **line_pointer = &line;
    long length = 0;
    signal(SIGINT, sigint_handler);
    while(notExit){
        if (sigsetjmp(env, 1) == 42) {
        }
        printf("mumsh $ ");
        fflush(stdout);
//        printf("line is %s", line);

        length = getline(line_pointer, &size, stdin);
        if (length == 1) {
            continue;
        }
        if (strcmp(line, "exit\n") == 0) {
            printf("exit\n");
            notExit = 0;
            return 0;
        }
        else if (length == -1) {
            printf("exit\n");
            notExit = 1;
            return 0;
        }
        else {
            int commandLength;
            int redTabLength;
            line = addspace(line, length);
            parsTab = returnCommandTable(line_pointer, 0, 0);
            commandTable = parsTab->commandTable;
            redirectionTable = parsTab->redirectionTable;
            commandLength = parsTab->commandLength;
            redTabLength = parsTab->redTabLength;

            int redirInH = parsTab->redirInH;
            int redirCreateH = parsTab->redirCreateH;
            int redirAppendH = parsTab->redirAppendH;

            if(commandLength == 0){
                fprintf(stderr, "error: missing program\n");
                free(commandTable);
                free(redirectionTable);
                free(parsTab);
                continue;
            }

//            if (redirectionTable[0]) {
//                redir(redirectionTable, redTabLength);
//            }

            if(commandLength == 2){
                if(strcmp(commandTable[0], "cd") == 0){
                    if(chdir(commandTable[1]) < 0){
                        fprintf(stderr, "%s: No such file or directory\n", commandTable[1]);
                    }
                    free(commandTable);
                    free(redirectionTable);
                    free(parsTab);
                    continue;
                }
            }

//            printf("now command tab is %s\n", commandTable[0]);
            char *argv[1024] = {0};
            int cmdNum = 1;

            int missingP = 0;

            for (int tmp = 0; tmp < commandLength; tmp++) {
                if (strcmp(commandTable[tmp], "|") == 0) {
                    cmdNum = cmdNum + 1;
                    if(strcmp(commandTable[tmp + 1], "|") == 0) {
                        fprintf(stderr, "error: missing program\n");
                        missingP = 1;
                        break;
                    }
                }
//                printf("arg is %s\n", commandTable[tmp]);
                argv[tmp] = commandTable[tmp];
            }

            if(strcmp(commandTable[0], "|") == 0){
                fprintf(stderr, "error: missing program\n");
                missingP = 1;
            }

            if(missingP){
                free(commandTable);
                free(redirectionTable);
                free(parsTab);
                continue;
            }

            if(cmdNum > 1){
//                printf("%i\n", redirInH);
                if(redirInH > 0 && parsTab->redirIn){
                    fprintf(stderr, "error: duplicated input redirection\n");
                    free(commandTable);
                    free(redirectionTable);
                    free(parsTab);
                    continue;
                }
                else if(((redirAppendH < cmdNum - 1) && parsTab->redirOutAppend) ||
                        ((redirCreateH < cmdNum - 1) && parsTab->redirOutCreate)){
//                    printf("%i\n", redirAppendH);
//                    printf("%i\n", redirCreateH);
                    fprintf(stderr, "error: duplicated output redirection\n");
                    free(commandTable);
                    free(redirectionTable);
                    free(parsTab);
                    continue;
                }
            }
//            printf("cmdnum is %i\n", cmdNum);
//            printf("cmdLength is %i\n", commandLength);
//            sortCommand(cmdNum, argv, commandLength);
            pipeCmd(cmdNum, argv, commandLength, redirectionTable, redTabLength);
            if(commandTable){
                free(commandTable);
                commandTable = NULL;
            }
            if(redirectionTable){
                free(redirectionTable);
                redirectionTable = NULL;
            }
            if(parsTab){
                free(parsTab);
                parsTab = NULL;
            }
        }
    }
}
