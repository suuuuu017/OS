//
// Created by ShiHe Wang on 2021/9/17.
//

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    //exit flag
    int notExit = 1;

    //input line setup
    char l[1024];
    char *line = l;
    size_t size = 1024;
    char **line_pointer = &line;

    //pid for executor
    pid_t pid = 0;

    do{
        //just exit special case
        printf("mumsh $ ");
        size_t characters = getline(line_pointer, &size, stdin);
//        printf("The number of characters getline() read: %zu\n", characters);
//        printf("You typed: %s\n",line);
        if (strcmp(line, "exit\n") == 0){
            printf("mumsh $ exit\n");
            notExit = 0;
        }
        else if (strcmp(line, "ls\n") == 0){
            char *cmd = "ls";
            char *argv[3];
            argv[0] = "ls";
            argv[1] = "-la";
            argv[2] = NULL;
            execvp(cmd, argv);
        }
    } while (notExit);
}
