/// @file pipe.h
/// @brief Contiene la definizioni di variabili e
///         funzioni specifiche per la gestione delle PIPE.

#ifndef PIPE_H
#define PIPE_H
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#pragma once
#include "defines.h"


void createPipe(int *pipee);
void readPipe(int *pipe, Msg *es);
void writePipe(int *pipe, Msg *es);

#endif
