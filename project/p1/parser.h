//
// Created by ShiHe Wang on 2021/9/18.
//

#ifndef VE482_2021FA_PARSER_H
#define VE482_2021FA_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


typedef struct{
    char ** commandTable;
    char ** redirectionTable;
    int commandLength;
    int redTabLength;
    int redirIn;
    int redirOutCreate;
    int redirOutAppend;
    int redirInH;
    int redirCreateH;
    int redirAppendH;
} parserTable;

parserTable * returnCommandTable(char **line_pointer, int cL, int rL);

void redir(char ** redirectionTable, int redTabLength);

char * addspace(char * line, long length);

#endif //VE482_2021FA_PARSER_H

