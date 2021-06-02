/// @file client.c
/// @brief Contiene l'implementazione del client.

#include "defines.h"
#include "err_exit.h"

#include<unistd.h>
#include<signal.h>

char buffer[10000];

int main() {
    readFile("InputFiles/F7.csv", buffer);

    sleep(2);

    printf("-------------------------mo parte il signal\n");

    sleep(12);

    printf("porchipid vari\npid1: %d\npid2: %d\npid3: %d\npadre: %d\n", piid.s1, piid.s2, piid.s3, padre);

    //kill(padre, SIGUSR1);

    //sistemo le varie array per poterle scrivere meglio sul file
    saveF7OnStruct(buffer);
    return 0;
}
