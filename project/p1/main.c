//
// Created by ShiHe Wang on 2021/9/17.
//
#include "main.h"

//TODO: fix the copying of long jump
static sigjmp_buf env;
//static volatile sig_atomic_t jump_active = 0;
char **commandTable = {0};
char **redirectionTable = {0};
parserTable *parsTab;

void sigint_handler() {
//    if (!jump_active) {
//        if(commandTable){
//            free(commandTable);
//            commandTable = NULL;
//        }
//        if(redirectionTable){
//            free(redirectionTable);
//            redirectionTable = NULL;
//        }
//        if(parsTab){
//            free(parsTab);
//            parsTab = NULL;
//        }
//        return;
//    }
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
//    struct sigaction s;
//    s.sa_handler = sigint_handler;
//    sigemptyset(&s.sa_mask);
//    s.sa_flags = SA_RESTART;
//    sigaction(SIGINT, &s, NULL);
    //pid for executor
//    pid_t pid = 0;

//    int tmp = 1024;
//    while(fgets(line, tmp, stdin)){
//        printf("line is %s", line);
//    }
//    fgets(line, tmp, stdin);
//    fgets(line, tmp, stdin);
//    length = getline(line_pointer, &size, stdin);
//    printf("line is %s\n", line);
//    printf("lengt is %zu\n", length);
//    line = addspace(line, length);

//    printf("line is %s\n", line);
//    printf("lengt is %zu\n", length);
    while(notExit){
        //just exit special case
        //TODO: wrong waiting prompt
//        printf(" length is %i\n", length);
//        signal(SIGINT, sigint_handler);
        if (sigsetjmp(env, 1) == 42) {
//            printf("hre $ ");
        }
//        jump_active = 1;
        printf("mumsh $ ");
        fflush(stdout);
//        getline(line_pointer, &size, stdin);
//        printf("line is %s", line);

        length = getline(line_pointer, &size, stdin);
        if (length == 1) {
            continue;
        }
        //TODO: does this exit have to be followed by a new line?
        if (strcmp(line, "exit\n") == 0) {
            printf("exit\n");
            notExit = 0;
            return 0;
        }
        else if (length == -1) {
            // TODO: if ls -a then enter then ^D, it prints nothing
            printf("exit\n");
            notExit = 1;
            return 0;
        }
        else {
//            int status = 0;
//            pid = fork();
//            if (pid == 0) {
            int commandLength;
            int redTabLength;
            //TODO: weird behaviour when add two parameters
            //TODO: free up the
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

            //TODO: weird processing order when input redirection is involved
            if (redirectionTable[0]) {
//                redir(redirectionTable, redTabLength);
            }

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


            if(strcmp(commandTable[0], "pwd") == 0){
                int fn = 1;
                if (redirectionTable[0]) {
                    fn = redir(redirectionTable, redTabLength);
                }
                char pwd[1024];
                if(getcwd(pwd, sizeof(pwd)) != NULL){
                    printf("%s\n", pwd);
                }
                if (redirectionTable[0]) {
                    dup2(fn, 1);
                    close(fn);
                }
                free(commandTable);
                free(redirectionTable);
                free(parsTab);
                continue;
            }

//            if(strcmp(commandTable[0], "pwd") == 0){
//                if (redirectionTable[0]) {
//                    redir(redirectionTable, redTabLength);
//                }
//                char pwd[1024];
//                if(getcwd(pwd, sizeof(pwd)) == NULL){
//                    perror("pwd error");
//                }
//                else{
//                    printf("%s\n", pwd);
//                }
//                dup(1);
//                continue;
//            }

//                printf("now command tab is %s\n", commandTable[0]);
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
            //TODO: error if empty argv
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
//            pid = fork();
//            if (pid == 0) {
//                pipeCmd(cmdNum, argv, commandLength, pid);
//                if (execvp(argv[0], argv) == -1) {
//                    printf("false\n");
//                }
//            }
//            else {
//                do {
//                    waitpid(pid, &status, WUNTRACED);
//                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
//            }
        }
    }
//    }while(notExit && ((length = getline(line_pointer, &size, stdin))));
}
