#include "global.h"

//Fichier C qui va mettre à jour le fichier texte meteo

static int fichierTexte = -1;
static int fichierLock = -1;
char *ATIS[] = {

    "1ONE EBLG 1803 00000KT 0600 FG OVC008 BKN040 PROB40 2024 0300 DZ FG OVC002 BKN040",
    "2TOW EBBR 0615 20015KT 8000 RA SCT010 OVC015 TEMPO 0608 5000 RA BKN005 BECMG 0810 9999 NSW BKN025",
    "3TRHE METAR VHHH 231830Z 06008KT 7000 FEW010SCT022 20/17 Q1017 NOSIG 5000 RA BKN005",
    "4FRUO 20015KT 8000 RA SCT010 OVC015 2024 0300 DZ FG 0810 9999 NSW BKN025",
    "5VEFI KT 7000 FEW010SCT02 EMPO 0608 5000 RA BKN005 EMPO 0608 5000 RA BKN005"

};

void genLock(void) {
    if ((fichierLock = open(FICHIERLOCK, O_CREAT | O_WRONLY | O_SYNC | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP)) == FAIL) {
        printf("Unable to create the lock file");
        exit(EXIT_FAILURE);
    }
}

void openMeteo(void) {
    if ((fichierTexte = open(FICHIERMETEO, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP)) == FAIL) {
        printf("Unable to open/create the meteo file");
        exit(EXIT_FAILURE);
    }
}

void deleteLock(void) {
    close(fichierLock);
    unlink(FICHIERLOCK);
}

int genAtis(void){

    while (true) {

        int msg = 0;

        unsigned int nbATIS = sizeof(ATIS) / sizeof(ATIS[0]);

        if (nbATIS > 0) {
            msg = rand() % nbATIS;
        } else {
            return EXIT_FAILURE;
        }

        printf("ATIS Create : %s\n", ATIS[msg]);

        genLock();

        openMeteo();

        if(write(fichierTexte, ATIS[msg], strlen(ATIS[msg])) < 0) {
            exit(EXIT_FAILURE);
        }

        close(fichierTexte);

        deleteLock();

        sleep(WAIT_TIME);

    }

    return EXIT_SUCCESS;
}

int main(void) {
    return genAtis();
}

/*
 * Init a meteo.txt file, filled
 *
 * Enter the loop
 * # Create Lock file
 * # Gen a new atis on the meteo.txt
 * # Close the meteo.txt and remove lock file
 *
*/
