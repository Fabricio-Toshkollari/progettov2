/// @file receiver_manager.c
/// @brief Contiene l'implementazione del receiver_manager.

#include <stdlib.h>

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"

#include "fifo.h"
#include "pipe.h"
#include <unistd.h>
#include <fcntl.h>


#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include<unistd.h>
#include<signal.h>

#define REQUEST      0
#define BUFFER_SZ 100
#define DATA_READY   1

void S1(int pipe1[]);

void S2(int pipe1[], int pipe2[]);

void S3(int pipe2[]);

char intestazioneF8[] = "Sender,Pid\n";
char intestazione123[] = "Id;Message;Id Sender;Id Receiver;Time arrival;Time dept\n";
char processo1[10] = "S1;";
char processo2[10] = "\nS2;";
char processo3[10] = "\nS3;";
char readBUffer[1000];

void sigHandler(int sig) {
    if (sig == SIGUSR1) {
        printf("\n***************************ho dormito\n");
        sleep(5);
        printf("\n***************************ho dormito\n");
    }
}

key_t shmKeyServer = 11;
key_t shmKeyClient = 12;
key_t semkey = 10;
int shmidClient = -1;
int semid = -1;
struct Request *request;

int msqid = -1;

int main(int argc, char *argv[]) {

    // get the server's shared memory segment
    int shmidServer = alloc_shared_memory(shmKeyServer, sizeof(struct Request));

    // attach the shared memory segment
    request = (struct Request *) get_shared_memory(shmidServer, 0);

    //msgqueue
    msqid = msgget(23, IPC_CREAT | S_IRUSR | S_IWUSR);

    //creaazione della pipe1
    int pipe1[2], pipe2[2];
    createPipe(pipe1);
    createPipe(pipe2);

    // set of signals. Not initialized!
    sigset_t mySet;

    // initialize mySet to contain all signals
    sigfillset(&mySet);

    // remove SIGUSR2 to the list of blocked signals
    sigdelset(&mySet, SIGALRM);
    sigdelset(&mySet, SIGINT);
    sigdelset(&mySet, SIGUSR1);
    sigdelset(&mySet, SIGUSR2);

    // blocking all signals
    sigprocmask(SIG_SETMASK, &mySet, NULL);

    if (signal(SIGALRM, sigHandler) == SIG_ERR)
        errExit("<child1> change signal handler failed");

    if (signal(SIGUSR1, sigHandler) == SIG_ERR)
        errExit("<child1> change signal handler failed");

    if (signal(SIGUSR2, sigHandler) == SIG_ERR)
        errExit("<child1> change signal handler failed");


    S1(pipe1);
    S2(pipe1, pipe2);
    S3(pipe2);


    if (existFile("OutputFiles/F8.csv") == 0) {
        createFile("OutputFiles/F8.csv", intestazioneF8);

    } else {
        truncFile("OutputFiles/F8.csv");
        writeInFileZano("OutputFiles/F8.csv", intestazioneF8);
    }


    int prc = 1;
    pid_t pid;
    int status = 0;
    while ((pid = wait(&status)) != -1) {
        if (prc == 1) {
            processIdcorrect(processo1, pid);
            writeInFileZano("OutputFiles/F8.csv", processo1);
        }
        if (prc == 2) {
            processIdcorrect(processo2, pid);
            writeInFileZano("OutputFiles/F8.csv", processo2);
        }
        if (prc == 3) {
            processIdcorrect(processo3, pid);
            writeInFileZano("OutputFiles/F8.csv", processo3);
        }
        prc++;
        printf("Child %d exited, status=%d\n", pid, WEXITSTATUS(status));
    }
    printf("porchipid vari\npid1: %d\npid2: %d\npid3: %d\npadre: %d\n", piid.s1, piid.s2, piid.s3, padre);

    return 0;
}


void S1(int pipe1[]) {

    pid_t pid = fork();
    if (pid == -1)
        errExit("S1 Error");

    if (pid == 0) {


        //legge un file e salva il MyMs:contenuto in un buffer
        readFile("InputFiles/F0.csv", readBUffer);

        if (existFile("OutputFiles/F1.csv") == 0) {
            createFile("OutputFiles/F1.csv", intestazione123);
        } else {
            truncFile("OutputFiles/F1.csv");
            writeInFileZano("OutputFiles/F1.csv", intestazione123);
        }

        //salva f0 in una struct
        saveF0OnStruct(readBUffer);

        close(pipe1[0]);
        struct Msg message;

        for (int i = 0; i < 3; i++) {
            switch (fork()) {
                case 0: {
                    myTime();
                    char time[10] = "";
                    strcat(time, timeNow);

                    message = f0[i];
                    sleep(atoi(message.delS1));

                    saveStructInFile(message, "OutputFiles/F1.csv", time);

                    //write(pipe1[1], &message, sizeof(message));
                    writePipe(pipe1, &message);


                    printf("write s1: %s\n", message.message);

                    exit(0);
                }
            }

            //scrive il messggio nella pipe1 per S2

            /*
            if (kill(pid2, SIGALRM) == -1)
              errExit("kill failed");
            */

        }
        close(pipe1[1]);
        piid.s1 = getpid();

        printf("PID: %d , PPID: %d. Exit code: %d\n",
               getpid(), getppid(), 1);

        pid_t pid;
        int status = 0;
        while ((pid = wait(&status)) != -1);

        printf("\n");
        printf("\n");
        printf("sono S1 e ho finito il mio job, ciao.\n");
        printf("\n");
        printf("\n");

        exit(1);

    }
    else {
        piid.s1 = pid;
    }
}

void S2(int pipe1[], int pipe2[]) {

    pid_t pid = fork();
    if (pid == -1)
        errExit("S2 Error");

    if (pid == 0) {
        pid2 = getpid();

        if (existFile("OutputFiles/F2.csv") == 0) {
            createFile("OutputFiles/F2.csv", intestazione123);

        } else {
            truncFile("OutputFiles/F2.csv");
            writeInFileZano("OutputFiles/F2.csv", intestazione123);

        }

        close(pipe1[1]);
        close(pipe2[0]);
        struct Msg message;

        for (int i = 0; i < 3; i++) {

            //read(pipe1[0], &message, sizeof(message) +1 );
            readPipe(pipe1, &message);

            printf("read s2: %s \n", message.message);

            switch (fork()) {
                case 0: {
                    myTime();
                    char time[10] = "";
                    strcat(time, timeNow);

                    sleep(atoi(message.delS2));
                    saveStructInFile(message, "OutputFiles/F2.csv", time);
                    if (strcmp(message.idSender, "S2") == 0) {

                        if (strcmp(message.type, "Q") == 0) {
                            printf("s2 invia tramite message queue\n");

                        } else if (strcmp(message.type, "SH") == 0) {
                            printf("s2 invia tramite shared memory\n");

                            // read a pathname from user
                            request->msgreq = message;

                            // allocate a shared memory segment
                            shmidClient = alloc_shared_memory(shmKeyClient, sizeof(char) * BUFFER_SZ);

                            // attach the shared memory segment
                            char *buffer = (char *) get_shared_memory(shmidClient, 0);

                            // copy shmKeyClient into the server's shared memory segment
                            request->shmKey = shmKeyClient;

                            // get the server's semaphore set
                            semid = semget(semkey, 2, S_IRUSR | S_IWUSR);
                            if (semid > 0) {
                                // unlock the server
                                semOp(semid, REQUEST, 1);
                                // wait for data
                                semOp(semid, DATA_READY, -1);

                                if (buffer[0] == -1)
                                    printf("File %s does not exist\n", request->msgreq.message);
                                else
                                    printf("%s\n", buffer);
                            } else
                                printf("semget failed");

                            // detach the shared memory segment
                            free_shared_memory(buffer);

                            // detach the server's shared memory segment
                            free_shared_memory(request);

                            // remove the shared memory segment
                            remove_shared_memory(shmidClient);

                            // remove the created semaphore set
                            if (semctl(semid, 0 /*ignored*/, IPC_RMID, NULL) == -1)
                                errExit("semctl IPC_RMID failed");


                        }
                    } else {
                        write(pipe2[1], &message, sizeof(message));
                        //writePipe(pipe2, &message);

                    }

                    exit(0);
                }
            }
        }
        close(pipe2[1]);

        printf("PID: %d , PPID: %d. Exit code: %d\n",
               getpid(), getppid(), 2);
        sleep(2);

        pid_t pid;
        int status = 0;
        while ((pid = wait(&status)) != -1);

        printf("\nsono S2 e ho finito il mio job, ciao.\n");

        exit(2);
    }else {
        piid.s2 = pid;
    }

}

void S3(int pipe2[]) {

    pid_t pid = fork();
    if (pid == -1)
        errExit("S2 Error");
    if (pid == 0) {

        struct Msg message;

        size_t len;


        //message.mtype = 1;
        len = strlen(message.message);
        message.message[len - 1] = '\0';

        pid3 = getpid();
        if (existFile("OutputFiles/F3.csv") == 0) {
            createFile("OutputFiles/F3.csv", intestazione123);

        } else {
            truncFile("OutputFiles/F3.csv");
            writeInFileZano("OutputFiles/F3.csv", intestazione123);

        }

        close(pipe2[1]);
        for (int i = 0; i < 2; i++) {
            readPipe(pipe2, &message);

            switch (fork()) {
                case 0: {
                    myTime();
                    char time[10] = "";
                    strcat(time, timeNow);

                    sleep(atoi(message.delS3));
                    saveStructInFile(message, "OutputFiles/F3.csv", time);
                    if (strcmp(message.idSender, "S3") == 0) {

                        if (strcmp(message.type, "Q") == 0) {
                            printf("s3 invia tramite message queue\n");


                            size_t mSize = sizeof(struct Msg) - sizeof(long);
                            msgsnd(msqid, &message, mSize, 0);

                        } else if (strcmp(message.type, "SH") == 0) {
                            printf("s3 invia tramite shared memory\n");
                        } else if (strcmp(message.type, "FIFO") == 0) {
                            printf("s3 invia tramite fifo\n");
                            int fd;
                            char *myfifo = "OutputFiles/my_fifo.txt";
                            // TODO: sistemare fifo
                            /* create the FIFO (named pipe) */
                            mkfifo(myfifo, 0666);

                            /* write "Hi" to the FIFO */
                            fd = open(myfifo, O_WRONLY);
                            write(fd, &message, sizeof(message));
                            close(fd);

                            /* remove the FIFO */
                            unlink(myfifo);

                        }
                    }
                    exit(0);
                }
            }
        }
        close(pipe2[0]);

        //salva il msg nel reg (f3)
        printf("PID: %d , PPID: %d. Exit code: %d\n",
               getpid(), getppid(), 3);

        sleep(3);

        pid_t pid;
        int status = 0;
        while ((pid = wait(&status)) != -1);

        exit(3);

    }else {
        piid.s3 = pid;
    }
}
      
  