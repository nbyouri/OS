#include "global.h"

//Fichier C qui va mettre à jour le fichier texte meteo

void delete(const char * pathname);
bool exists(const char * pathname);

static int fichierTexte = -1;
static int fichierLock = -1;
static bool cont = true;

char *ATIS[] = {

    "1ONE EBLG 1803 00000KT 0600 FG OVC008 BKN040 PROB40 2024 0300 DZ FG OVC002 BKN040",
    "2TOW EBBR 0615 20015KT 8000 RA SCT010 OVC015 TEMPO 0608 5000 RA BKN005 BECMG 0810 9999 NSW BKN025",
    "3TRHE METAR VHHH 231830Z 06008KT 7000 FEW010SCT022 20/17 Q1017 NOSIG 5000 RA BKN005",
    "4FRUO 20015KT 8000 RA SCT010 OVC015 2024 0300 DZ FG 0810 9999 NSW BKN025",
    "5VEFI KT 7000 FEW010SCT02 EMPO 0608 5000 RA BKN005 EMPO 0608 5000 RA BKN005"

};

void openLock(void) {
    if ((fichierLock = open(FICHIERLOCK, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP)) == FAIL) {
        printf("Unable to create the lock file\n");
        exit(EXIT_FAILURE);
    }
}

void openMeteo(void) {
    if ((fichierTexte = open(FICHIERMETEO, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP)) == FAIL) {
        printf("Unable to open/create the meteo file\n");
        exit(EXIT_FAILURE);
    }
}

void closeLock(void) {
    if (exists(FICHIERLOCK)) {
        if (close(fichierLock) == FAIL) {
            printf("Unable to close the lock file\n");
            exit(EXIT_FAILURE);
        }
    }
}

void closeMeteo(void) {
    if (exists(FICHIERMETEO)) {
        if (close(fichierTexte) == FAIL) {
            printf("Unable to close the meteo file : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

void pilotCleanup(int state) {
    if (state == EXIT_SUCCESS || state == SIGINT) {
        cont = checkyesno("Shutdown meteo");
    } else if (state == EXIT_FAILURE) {
        cont = false;
    }

    if (!cont) {
        delete(FICHIERMETEO);
        if (exists(FICHIERLOCK)) {
            delete(FICHIERLOCK);
        }
    }
}

int genAtis(void){

    while (cont) {

        int msg = 0;

        unsigned int nbATIS = sizeof(ATIS) / sizeof(ATIS[0]);

        if (nbATIS > 0) {
            msg = rand() % nbATIS;
        } else {
            return EXIT_FAILURE;
        }

        printf("ATIS Create : %s\n", ATIS[msg]);

        openLock();

        openMeteo();

        if (write(fichierTexte, ATIS[msg], strlen(ATIS[msg])) == FAIL) {
            return EXIT_FAILURE;
        }


        closeMeteo();

        closeLock();
        delete(FICHIERLOCK);

        sleep(WAIT_TIME);

    }

    return EXIT_SUCCESS;
}

int main(void) {
    signal(SIGINT, &pilotCleanup);

    return genAtis();
}
