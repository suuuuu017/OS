//
// Created by ShiHe Wang on 2021/9/17.
//

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int notExit = 1;
    char s[1024];
    char *string = s; // character array pointer
    size_t size = 1024; // initial size of char array
    char **string_pointer = &string; // double pointer to char array
    do{
        printf("mumsh $ ");
        size_t characters = getline(string_pointer,&size,stdin);
//        printf("The number of characters getline() read: %zu\n", characters);
//        printf("You typed: %s\n",string);
        if(strcmp(string, "exit\n") == 0){
            printf("mumsh $ exit\n");
            notExit = 0;
        }
    } while (notExit);
}
