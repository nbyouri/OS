#include "global.h"

//Fichier C qui va mettre à jour le fichier texte meteo

static int fichierTexte = -1;
static int fichierLock = -1;

void genLock(void) {
    if ((fichierLock = open(FICHIERLOCK, O_CREAT | O_WRONLY | O_SYNC | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP)) == FAIL) {
        printf("Impossible de faire le fichier lock");
        exit(EXIT_FAILURE);
    }
}

void openMeteo(void) {
    if ((fichierTexte = open(FICHIERMETEO, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP)) == FAIL) {
        printf("Impossible d'ouvrir le fichier meteo\n");
        exit(EXIT_FAILURE);
    }
}

void deleteLock(void) {
    close(fichierLock);
    unlink(FICHIERLOCK);
}

int genAtis(void){
    
    int msg = 0;
    
    char *ATIS[] = {
        
            "EBLG 1803 00000KT 0600 FG OVC008 BKN040 PROB40 2024 0300 DZ FG OVC002 BKN040",
            "EBBR 0615 20015KT 8000 RA SCT010 OVC015 TEMPO 0608 5000 RA BKN005 BECMG 0810 9999 NSW BKN025",
            "METAR VHHH 231830Z 06008KT 7000 FEW010SCT022 20/17 Q1017 NOSIG 5000 RA BKN005",
            "20015KT 8000 RA SCT010 OVC015 2024 0300 DZ FG 0810 9999 NSW BKN025"
        
    };
    
    unsigned int nbATIS = sizeof(ATIS) / sizeof(ATIS[0]);

    if (nbATIS > 0) {
        msg = rand() % nbATIS;
    } else {
        return EXIT_FAILURE;
    }
    
    genLock();
    
    openMeteo();
    
    write(fichierTexte, ATIS[msg], strlen(ATIS[msg]));

    close(fichierTexte);

    deleteLock();
    
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