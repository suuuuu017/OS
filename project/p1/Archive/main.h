//
// Created by ShiHe Wang on 2021/9/17.
//

#ifndef VE482_2021FA_MAIN_H
#define VE482_2021FA_MAIN_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <setjmp.h>
#include <errno.h>

#include "parser.h"
#include "execute.h"

typedef void (*sighandler_t)();

#endif //VE482_2021FA_MAIN_H
