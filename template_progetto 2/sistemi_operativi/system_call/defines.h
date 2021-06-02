/// @file defines.h
/// @brief Contiene la definizioni di variabili
///         e funzioni specifiche del progetto.

#ifndef DEF_H
#define DEF_H
#define MAX_READ 1000
#define MAX_STRING 100
#define MAX_BUF 1024

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/stat.h>

#pragma once

pid_t pid1, pid2, pid3, padre;

struct Msg {
    char id[5];
    char message[MAX_STRING];
    char idSender[5];
    char idReceiver[5];
    char delS1[5];
    char delS2[5];
    char delS3[5];
    char type[20];
    //long mtype;
};

struct Request {
    struct Msg msgreq;
    key_t shmKey;
};

struct Hackler {
    char id[20];
    char delay[20];
    char target[20];
    char action[20];
};

struct pid {
    int s1;
    int s2;
    int s3;
    int padre;
};

struct pid piid;

typedef struct Msg Msg;

struct Msg f0[3];

struct Hackler f7[4];

char timeNow[10];



void writeInFileZano(char fileName[], char string[]);
void readFile(char * name, char * buff);
int existFile(char * name);
void truncFile(char * name);
void createFile(char * name, char * arr);
void processIdcorrect(char * p, pid_t pid);
void saveF0OnStruct(char file[]);
void readAFile(char fileName[], char *buffer);
void saveStructInFile(Msg s, char fileName[],char time[]);
void myTime();
void saveF7OnStruct(char file[]);


#endif