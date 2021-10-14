//
// Created by ShiHe Wang on 2021/10/4.
//

#ifndef VE482_2021FA_EXECUTE_H
#define VE482_2021FA_EXECUTE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

typedef struct{
    char * command[1024];
} command;

typedef struct{
    command commandList[1024];
} commandList;

int checkforSpecialCharacter(char * nextString);

void sortCommand(int cmdNum, char * argv[], int commandLength, char ** cl[1024]);

void pipeCmd(int cmdNum, char * argv[], int commandLength, char ** redirectionTable, int redTabLength);


#endif //VE482_2021FA_EXECUTE_H
