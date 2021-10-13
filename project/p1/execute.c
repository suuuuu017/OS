//
// Created by ShiHe Wang on 2021/10/4.
//

#include "execute.h"

int checkforSpecialCharacter(char * nextString){
    if(strcmp(nextString, "<") == 0 || strcmp(nextString, ">") == 0 || strcmp(nextString, ">>") == 0
        || strcmp(nextString, "|") == 0 || strcmp(nextString, "pwd") == 0 || strcmp(nextString, "cd") == 0){
        return 1;
    }
    return 0;
}

void sortCommand(int cmdNum, char * argv[], int commandLength, char ** cl[1024]) {
    int argvC = 0;
    for(int i = 0; i < cmdNum; i++) {
        char * cmd[1024] = {0};
        printf("current it is %i\n", i);
        for (int tmp = 0; tmp < commandLength; tmp++) {
//            printf("current is %s\n", argv[argvC]);
            if(argv[argvC]){
                if (strcmp(argv[argvC], "|") == 0) {
                    argvC = argvC + 1;
                    cmd[tmp] = NULL;
                    cl[i] = cmd;
//                cl.commandList[i]->command[tmp] = NULL;
                    break;
                }
                else {
//                    printf("current is %s\n", argv[argvC]);
//                    printf("current is %i\n", tmp);
//                cl.commandList[i]->command[tmp] = argv[argvC];
                    cmd[tmp] = argv[argvC];
//                    printf("currentttttt is %s\n", argv[argvC]);
                    argvC = argvC + 1;
                }
            }
        }
        printf("current is %s %s %s %i\n", cmd[0], cmd[1], cmd[2], i);
        cl[i] = cmd;
        printf("current after is %s %s %s %i\n", cl[i][0], cl[i][1], cl[i][2], i);
    }
//    printf("argc is %i\n", argvC);
//    printf("arg is %s\n", cl.commandList[0].command[0]);
//    return cl;
}


void pipeCmd(int cmdNum, char * argv[], int commandLength, char ** redirectionTable, int redTabLength){

    char *redirInFn;
    char *redirOutCreateFn;
    char *redirOutAppendFn;
    int redirIn = 0;
    int redirOutCreate = 0;
    int redirOutAppend = 0;

    if (redirectionTable[0]) {
        for (int k = 0; k < redTabLength; k++) {
            if (strcmp(redirectionTable[k], "<") == 0) {
                k = k + 1;
                redirInFn = redirectionTable[k];
                if(checkforSpecialCharacter(redirInFn)){
                    fprintf(stderr, "syntax error near unexpected token `%s\'\n", redirInFn);
                    return;
                }
                redirIn = redirIn + 1;
            } else if (strcmp(redirectionTable[k], ">") == 0) {
                k = k + 1;
                redirOutCreateFn = redirectionTable[k];
                if(checkforSpecialCharacter(redirOutCreateFn)){
                    fprintf(stderr, "syntax error near unexpected token `%s\'\n", redirOutCreateFn);
                    return;
                }
                redirOutCreate = redirOutCreate + 1;
            } else if (strcmp(redirectionTable[k], ">>") == 0) {
                k = k + 1;
                redirOutAppendFn = redirectionTable[k];
                if(checkforSpecialCharacter(redirOutAppendFn)){
                    fprintf(stderr, "syntax error near unexpected token `%s\'\n", redirOutAppendFn);
                    return;
                }
                redirOutAppend = redirOutAppend + 1;
            }
        }
    }

    if(redirIn > 1){
        fprintf(stderr, "error: duplicated input redirection\n");
        return;
    }
    if(redirOutCreate > 1 || redirOutAppend > 1 || (redirOutCreate + redirOutAppend) > 1){
        fprintf(stderr, "error: duplicated output redirection\n");
        return;
    }

    int status = 0;

    int prein = dup(0);
    int preout = dup(1);

    int fdin;

    if(redirIn){
        if((fdin = open(redirInFn, O_RDONLY)) < 0){
            if(errno == ENOENT) {
                fprintf(stderr, "%s: No such file or directory\n", redirInFn);
            }
            return;
        }
    }
    else{
        fdin = dup(prein);
    }

    pid_t pid;
    int fdout;
    int j = 0;
    int argvC = 0;

    int pipeid[1024];

    for(int i = 0; i < cmdNum; i++) {
        j = 0;
        dup2(fdin, 0);
        close(fdin);
        if (i == cmdNum - 1) {
            if (redirOutCreate) {
                fdout = creat(redirOutCreateFn, 0644);
                if(fdout < 0){
                    if(errno == EPERM || errno==EROFS){
                        fprintf(stderr, "%s: Permission denied\n", redirOutCreateFn);
                    }
                    return;
                }
            }
            else if (redirOutAppend){
                fdout = open(redirOutAppendFn, O_CREAT | O_RDWR | O_APPEND, 0644);
                if(fdout < 0){
                    if(errno == EPERM || errno==EROFS){
                        fprintf(stderr, "%s: Permission denied\n", redirOutAppendFn);
                    }
                    return;
                }
            }
            else {
                fdout = dup(preout);
            }
        }
        else {
            int fdpipe[2];
            pipe(fdpipe);
            fdout = fdpipe[1];
            fdin = fdpipe[0];
        }

        dup2(fdout, 1);
        close(fdout);

        char * cmd[1024] = {0};
        while(argvC < commandLength){
            if(strcmp(argv[argvC], "|") == 0){
                argvC = argvC + 1;
                break;
            }
            else{
                cmd[j] = argv[argvC];
                j = j + 1;
            }
            argvC = argvC + 1;
        }
//        printf("%s %s \n",cmd[0], cmd[1]);

        pid = fork();
        pipeid[i] = pid;
        if (pid == 0) {
            signal(SIGINT, SIG_DFL);

            if(strcmp(cmd[0], "pwd") == 0){
                char pwd[1024];
                if(getcwd(pwd, sizeof(pwd)) != NULL){
                    printf("%s\n", pwd);
                }
                exit(0);
            }

            if(execvp(cmd[0], cmd) < 0){
                //TODO: what if wrong  argument
                fprintf(stderr, "%s: command not found\n", cmd[0]);
            }
            exit(1);
        }
    }

    dup2(prein, 0);
    dup2(preout, 1);
    close(prein);
    close(preout);
    for(int i = 0; i < cmdNum; i++) {
        waitpid(pipeid[i], &status, WUNTRACED);
    }
}
