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
} parserTable;

parserTable * returnCommandTable(char **line_pointer, size_t length, int cL, int rL);

#endif //VE482_2021FA_PARSER_H
