/// @file defines.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche del progetto.

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

#include "defines.h"
#include "err_exit.h"

#define MAX_READ 1000
#define MAX_STRING 100
#define BUFFER_SZ 100




//converte il tempo in stringa e la mette nella variabile globale time now
void myTime() {
    time_t s;
    time(&s);

    int h, m, sec;
    char temp[2];

    h = s / 3600 % 24;
    m = (s / 60) % 60;
    sec = s % 60;

    if (h / 10 == 0)
        strcat(timeNow, "0");

    sprintf(timeNow, "%d", h);
    strcat(timeNow, ":");

    if (m / 10 == 0)
        strcat(timeNow, "0");

    sprintf(temp, "%d", m);
    strcat(timeNow, temp);
    strcat(timeNow, ":");

    if (sec / 10 == 0)
        strcat(timeNow, "0");

    sprintf(temp, "%d", sec);
    strcat(timeNow, temp);
}

//salva la struct in un file f1 f2 f3
void saveStructInFile(Msg s, char fileName[], char time[]) {
    char result[MAX_READ] = "";
    strcat(result, s.id);
    strcat(result, ";");
    strcat(result, s.message);
    strcat(result, ";");
    strcat(result, s.idSender);
    strcat(result, ";");
    strcat(result, s.idReceiver);
    strcat(result, ";");
    strcat(result, time);
    strcat(result, ";");
    myTime();
    strcat(result, timeNow);
    strcat(result, "\n");

    writeInFileZano(fileName, result);
}

//salva f0 in una struct
void saveF0OnStruct(char file[]) {

    int i, j = 1, z = 0, flag = 0;
    for (i = 0; i <= strlen(file); i++) {

        if ((file[i] == ';' || file[i] == '\n') && flag == 1) {
            j++;
            z = 0;
        } else if (file[i] == '\n' && flag == 0) {
            flag = 1;
        } else if (flag == 1) {

            if (j % 8 == 1) {
                f0[j / 8].id[z] = file[i];
            }

            if (j % 8 == 2) {
                f0[j / 8].message[z] = file[i];
            }

            if (j % 8 == 3) {
                f0[j / 8].idSender[z] = file[i];
            }

            if (j % 8 == 4) {
                f0[j / 8].idReceiver[z] = file[i];
            }

            if (j % 8 == 5) {
                f0[j / 8].delS1[z] = file[i];
            }

            if (j % 8 == 6) {
                f0[j / 8].delS2[z] = file[i];
            }

            if (j % 8 == 7) {
                f0[j / 8].delS3[z] = file[i];
            }

            if (j % 8 == 0) {
                f0[(j / 8) - 1].type[z] = file[i];
            }

            z++;
        }
    }
}

//salva f7 in una struct
void saveF7OnStruct(char file[]) {

    int i, j = 1, z = 0, flag = 0;
    for (i = 0; i <= strlen(file); i++) {

        if ((file[i] == ';' || file[i] == '\n') && flag == 1) {
            j++;
            z = 0;

        } else if (file[i] == '\n' && flag == 0) {
            flag = 1;

        } else if (flag == 1) {
            if (j % 4 == 1) {
                f7[j / 4].id[z] = file[i];

            }
            if (j % 4 == 2) {
                f7[j / 4].delay[z] = file[i];
            }
            if (j % 4 == 3) {
                f7[j / 4].target[z] = file[i];
            }
            if (j % 4 == 0) {
                f7[(j / 4) - 1].action[z] = file[i];
            }
            z++;
        }
    }
}

//controlla se siste un file TODO: errexit
int existFile(char *fileName) {
    if (access(fileName, F_OK) == -1) {
        return 0;
    }
    return 1;
}

//crea un file con l'intestazione TODO: errexit
void createFile(char *fileName, char *arr) {
    int file;
    file = open(fileName, O_CREAT | O_EXCL | O_RDWR | S_IRWXU);
    write(file, arr, strlen(arr) + 1);
    close(file);
}

//tronca un file TODO: errexit
void truncFile(char *name) {
    int file;
    file = open(name, O_TRUNC | O_RDWR, S_IRWXU);
    close(file);
}

//cambaire nome TODO: ERREXIT WRITE
void writeInFileZano(char fileName[], char string[]) {
    //apre e controlla con err_exit
    int file = open(fileName, O_WRONLY);
    if (file == -1) {
        errExit("open");
    }
    lseek(file, 0, SEEK_END);
    //scrive su file aperto e poi controllo
    ssize_t wrt = write(file, string, strlen(string));
    close(file);
}

//sistema il file per essere scritto
void processIdcorrect(char * p,pid_t pid){
    char tmp[100];
    sprintf(tmp, "%d",pid);
    strcat(p,tmp);
}

//legge un file TODO: errexit
void readFile(char *name, char *buff) {
    int file;
    file = open(name, O_RDONLY);
    read(file, buff, 10000);
    close(file);
}



    
    
    


