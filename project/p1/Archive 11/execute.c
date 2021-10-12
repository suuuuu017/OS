//
// Created by ShiHe Wang on 2021/10/4.
//

#include "execute.h"

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

//    char * cmd1[2] = {0};
//    cmd1[0] = "ls";
//    cmd1[1] = NULL;
//
//    char * cmd2[3] = {0};
//    cmd2[0] = "grep";
//    cmd2[1] = "mum";
//    cmd2[2] = NULL;
//    cmd2[3] = NULL;
//
//    char ** cll[2];
//    cll[0] = cmd1;
//    cll[1] = cmd2;
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
                redirIn = 1;
            } else if (strcmp(redirectionTable[k], ">") == 0) {
                k = k + 1;
                redirOutCreateFn = redirectionTable[k];
                redirOutCreate = 1;
            } else if (strcmp(redirectionTable[k], ">>") == 0) {
                k = k + 1;
                redirOutAppendFn = redirectionTable[k];
                redirOutAppend = 1;
            }
        }
    }

    int status = 0;

    int prein = dup(0);
    int preout = dup(1);

    int fdin;

    if(redirIn){
        fdin = open(redirInFn, O_RDONLY);
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
            }
            else if (redirOutAppend){
                fdout = open(redirOutAppendFn, O_CREAT | O_RDWR | O_APPEND, 0644);
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
//            if(i == 0){
//                execvp(cmd1[0], cmd1);
//            }
//            else if(i == 1){
//                execvp(cmd2[1], &cmd2[1]);
//            }
//            printf("current commmmmand is %s\n", cmd[0]);
//            execvp(cl[i][0], cl[i]);
//            printf("hi from child\n");
//            printf("%s %s \n",cmd[0], cmd[1]);
            signal(SIGINT, SIG_DFL);
//            if(strcmp(cmd[0], "pwd") == 0){
//                char pwd[1024];
//                if(getcwd(pwd, sizeof(pwd)) == NULL){
//                    perror("pwd error");
//                }
//                else{
//                    printf("%s\n", pwd);
//                }
//            }
//            else{
//                execvp(cmd[0], cmd);
//            }
            if(execvp(cmd[0], cmd) < 0){
                printf("non-exist: command not found\n");
            }
            _exit(1);
        }
    }

    dup2(prein, 0);
    dup2(preout, 1);
    close(prein);
    close(preout);
    for(int i = 0; i < cmdNum; i++) {
        waitpid(pipeid[i], &status, WUNTRACED);
    }
//    waitpid(pid, &status, WUNTRACED);
}
