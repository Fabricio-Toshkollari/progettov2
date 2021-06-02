/// @file receiver_manager.c
/// @brief Contiene l'implementazione del receiver_manager.

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"
#include <sys/sem.h>

#define BUFFER_SZ 100

#define REQUEST      0
#define DATA_READY   1


void R1();

void R2();

void R3();


char intestazioneF8[] = "Sender,Pid\n";
char intestazione123[] = "Id,Message,Id Sender,Id Receiver,Time arrival,Time dept\n";
char processo1[100] = "R1,";
char processo2[100] = "\nR2,";
char processo3[100] = "\nR3,";


int create_sem_set(key_t semkey) {
    // Create a semaphore set with 2 semaphores
    int semid = semget(semkey, 2, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (semid == -1)
        errExit("semget failed");

    // Initialize the semaphore set
    union semun arg;
    unsigned short values[] = {0, 0};
    arg.array = values;

    if (semctl(semid, 0, SETALL, arg) == -1)
        errExit("semctl SETALL failed");

    return semid;
}


void copy_file(const char *pathname, char *buffer) {
    // open in read only mode the file
    printf("--------- reciver messaggio shared: %s\n", pathname);
}

int main(int argc, char *argv[]) {

    //inizio shared
    key_t shmKey = 11;
    key_t semkey = 10;

    // allocate a shared memory segment
    int shmidServer = alloc_shared_memory(shmKey, sizeof(struct Request));
    // attach the shared memory segment
    struct Request *request = (struct Request *) get_shared_memory(shmidServer, 0);
    // create a semaphore set
    int semid = create_sem_set(semkey);
    // wait for a Request
    semOp(semid, REQUEST, -1);
    // allocate a shered memory segment
    int shmidClient = alloc_shared_memory(request->shmKey, sizeof(char) * BUFFER_SZ);
    // attach the shared memory segment
    char *buffer = (char *) get_shared_memory(shmidClient, 0);
    // copy file into the shared memory
    copy_file(request->msgreq.message, buffer);
    // notify that data was stored into client's shared memory
    semOp(semid, DATA_READY, 1);
    // detach the shared memory segment
    free_shared_memory(buffer);
    // detach the shared memory segment
    free_shared_memory(request);
    // remove the shared memory segment
    remove_shared_memory(shmidServer);
    //fine shared




    R1();
    R2();
    R3();


    if (existFile("OutputFiles/F9.csv") == 0) {
        createFile("OutputFiles/F9.csv", intestazioneF8);

    } else {
        truncFile("OutputFiles/F9.csv");
        writeInFileZano("OutputFiles/F9.csv", intestazioneF8);

    }
    printf("--------------------------------------------------------------------------porchipid vari\npid1: %d\npid2: %d\npid3: %d\npadre: %d\n", piid.s1, piid.s2, piid.s3, padre);



    int prc = 1;
    pid_t pid;
    int status = 0;
    while ((pid = wait(&status)) != -1) {
        if (prc == 1) {
            processIdcorrect(processo1, pid);
            writeInFileZano("OutputFiles/F9.csv", processo1);
        }
        if (prc == 2) {
            processIdcorrect(processo2, pid);
            writeInFileZano("OutputFiles/F9.csv", processo2);
        }
        if (prc == 3) {
            processIdcorrect(processo3, pid);
            writeInFileZano("OutputFiles/F9.csv", processo3);
        }
        prc++;
        printf("Child %d exited, status=%d\n", pid, WEXITSTATUS(status));
    }


    return 0;
}


void R1() {

    pid_t pid = fork();
    if (pid == -1)
        printf("S1 creation error");
    if (pid == 0) {

        if (existFile("OutputFiles/F6.cvs") == 0) {
            createFile("OutputFiles/F6.csv", intestazione123);
        } else {
            truncFile("OutputFiles/F6.csv");
            writeInFileZano("OutputFiles/F.csv", intestazione123);
        }

        printf("PID: %d , PPID: %d. Exit code: %d\n",
               getpid(), getppid(), 1);


        sleep(1);
        exit(1);
    }

}

void R2() {
    pid_t pid = fork();
    if (pid == -1)
        printf("S1 creation error");
    if (pid == 0) {

        int msqid = -1;
        msqid = msgget(23, S_IRUSR | S_IWUSR);


        if (existFile("OutputFiles/F5.cvs") == 0) {
            createFile("OutputFiles/F5.csv", intestazione123);
        } else {
            truncFile("OutputFiles/F5.csv");
            writeInFileZano("OutputFiles/F5.csv", intestazione123);
        }

        Msg message;

        int caso = 0;


        while (1) {
            // read a message from the message queue
            size_t mSize = sizeof(struct Msg) - sizeof(long);
            if (msgrcv(msqid, &message, mSize, 0, 0) == -1) {
                errExit("queue fail\n");
            }
            printf("++++++++++++++++++++reciver messaggio queue %s\n", message.message);
        }

        printf("PID: %d , PPID: %d. Exit code: %d\n",
               getpid(), getppid(), 1);


        sleep(2);
        exit(2);
    }

}

void R3() {

    pid_t pid = fork();
    if (pid == -1)
        printf("S1 creation error");
    if (pid == 0) {



        if (existFile("OutputFiles/F4.cvs") == 0) {
            createFile("OutputFiles/F4.csv", intestazione123);
        } else {
            truncFile("OutputFiles/F4.csv");
            writeInFileZano("OutputFiles/F4.csv", intestazione123);
        }
    //TODO: sincronizzare fifo e errexit

        sleep(10);

        int fd;
        char *myfifo = "OutputFiles/my_fifo.txt";
        char buf[100];

        Msg messagefifo;

        /* open, read, and display the message from the FIFO */
        fd = open(myfifo, O_RDONLY);
        read(fd, &messagefifo, 1000);
        printf("$$$$$$$$$$$$$$$$$$$$$$ reciver messaggio fifo: %s\n", messagefifo.message);
        close(fd);

        printf("PID: %d , PPID: %d. Exit code: %d\n",
               getpid(), getppid(), 1);


        sleep(3);
        exit(3);
    }

}
