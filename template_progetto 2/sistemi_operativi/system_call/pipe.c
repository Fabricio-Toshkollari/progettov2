/// @file pipe.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche per la gestione delle PIPE.

#include "pipe.h"
#include "defines.h"
#include "err_exit.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>


void createPipe(int *pipee) {
    if (pipe(pipee) == -1)
        errExit("pipe");
}

//legge dalla pipe
void readPipe(int *pipe, Msg *es) {
    if(read(pipe[0], es, sizeof(*es)) == -1)
        errExit("read pipe");
}

//scrive nella pipe
void writePipe(int *pipe,  Msg *es) {
    if(write(pipe[1], es, sizeof(*es)) == -1)
        errExit("write pipe");
}

