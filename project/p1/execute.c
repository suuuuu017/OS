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


void pipeCmd(int cmdNum, char * argv[], int commandLength){

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

    int status = 0;
    int redirIn = 0;
    int redirOut = 0;

    int prein = dup(0);
    int preout = dup(1);

    int fdin;

    if(redirIn){

    }
    else{
        fdin = dup(prein);
    }

    pid_t pid;
    int fdout;
    int j = 0;
    int argvC = 0;

    for(int i = 0; i < cmdNum; i++) {
        j = 0;
        dup2(fdin, 0);
        close(fdin);
        if (i == cmdNum - 1) {
            if (redirOut) {
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

        pid = fork();
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
            execvp(cmd[0], cmd);
            _exit(1);
        }
    }

    dup2(prein, 0);
    dup2(preout, 1);
    close(prein);
    close(preout);
    waitpid(pid, &status, WUNTRACED);
}
