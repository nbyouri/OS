#include "global.h"

static int meteo = -1;
static int lock = -1;
static bool cont = true;

static void openLock(void);
static void openMeteo(void);
static void closeLock(void);
static void closeMeteo(void);
static void pilotCleanup(int);
static int genAtis(void);

static char ATIS[][MSG_SIZE] = {
    "ATIS 1ONE EBLG 1803 00000KT 0600 FG OVC008 BKN040 PROB40 2024 0300 DZ FG OVC002 BKN040",
    "ATIS 2TOW EBBR 0615 20015KT 8000 RA SCT010 OVC015 TEMPO 0608 5000 RA BKN005 BECMG 0810 NSW BKN025",
    "ATIS 3TRHE METAR VHHH 231830Z 06008KT 7000 FEW010SCT022 20/17 Q1017 NOSIG 5000 RA BKN005",
    "ATIS 4FRUO 20015KT 8000 RA SCT010 OVC015 2024 0300 DZ FG 0810 9999 NSW BKN025",
    "ATIS 5VEFI KT 7000 FEW010SCT02 EMPO 0608 5000 RA BKN005 EMPO 0608 5000 RA BKN005"
};

static void openLock(void) {
    if ((lock = open(FICHIERLOCK, O_CREAT | O_WRONLY, S_IRUSR |
                    S_IWUSR | S_IRGRP)) == FAIL) {
        printf("Unable to create the lock file\n");
        exit(EXIT_FAILURE);
    }
}

static void openMeteo(void) {
    if ((meteo = open(FICHIERMETEO, O_CREAT | O_WRONLY |
                    O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP)) == FAIL) {
        printf("Unable to open/create the meteo file\n");
        exit(EXIT_FAILURE);
    }
}

static void closeLock(void) {
    if (exists(FICHIERLOCK)) {
        if (close(lock) == FAIL) {
            printf("Unable to close the lock file\n");
            exit(EXIT_FAILURE);
        }
    }
}

static void closeMeteo(void) {
    if (exists(FICHIERMETEO)) {
        if (close(meteo) == FAIL) {
            printf("Unable to close the meteo file : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

static void pilotCleanup(int state) {
    if (state == EXIT_SUCCESS || state == SIGINT) {
        cont = checkyesno("Shutdown meteo");
    } else if (state == EXIT_FAILURE) {
        cont = false;
    }
    if (!cont) {
        closeMeteo();
        delete(FICHIERMETEO);
        if (exists(FICHIERLOCK)) {
            delete(FICHIERLOCK);
        }
    }
}

static int genAtis(void){

    while (cont) {
        unsigned int msg = 0;
        unsigned int nATIS = sizeof(ATIS) / sizeof(ATIS[0]);
        if (nATIS > 0) {
            msg = (unsigned int)rand() % nATIS;
        } else {
            return EXIT_FAILURE;
        }
        printf("ATIS Create : %s\n", ATIS[msg]);
        openLock();
        openMeteo();
        if (write(meteo, ATIS[msg], sizeof(ATIS[msg])) == FAIL) {
            return EXIT_FAILURE;
        }
        // virtual write wait time, to make the 
        // meteo transmission latency more realistic :)
        sleep(WRITE_TIME);
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
