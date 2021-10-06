//
// Created by ShiHe Wang on 2021/9/17.
//
#include "main.h"

void sigint_handler() {
//    printf("Caught SIGINT %i\n", signo);
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

    signal(SIGINT, SIG_IGN);

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
        } else if (length == -1) {
            // TODO: if ls -a then enter then ^D, it prints nothing
            printf("exit\n");
            notExit = 1;
            return 0;
        } else {
//            int status = 0;
//            pid = fork();
//            if (pid == 0) {
            parserTable *parsTab;
            int commandLength;
            int redTabLength;
            //TODO: weird behaviour when add two parameters
            //TODO: free up the
            line = addspace(line, length);
            parsTab = returnCommandTable(line_pointer, 0, 0);
            char **commandTable = {0};
            commandTable = parsTab->commandTable;
            char **redirectionTable = {0};
            redirectionTable = parsTab->redirectionTable;
            commandLength = parsTab->commandLength;
            redTabLength = parsTab->redTabLength;
            //TODO: weird processing order when input redirection is involved
            if (redirectionTable[0]) {
//                redir(redirectionTable, redTabLength);
            }
//                printf("now command tab is %s\n", commandTable[0]);
            char *argv[1024] = {0};
            int cmdNum = 1;
            for (int tmp = 0; tmp < commandLength; tmp++) {
                if (strcmp(commandTable[tmp], "|") == 0) {
                    cmdNum = cmdNum + 1;
                }
//                printf("arg is %s\n", commandTable[tmp]);
                argv[tmp] = commandTable[tmp];
            }
//            printf("cmdnum is %i\n", cmdNum);
//            printf("cmdLength is %i\n", commandLength);
            //TODO: error if empty argv
//            sortCommand(cmdNum, argv, commandLength);
            pipeCmd(cmdNum, argv, commandLength, redirectionTable, redTabLength);
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
