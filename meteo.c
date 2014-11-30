#include "global.h"

//Fichier C qui va mettre à jour le fichier texte meteo
void genAtis(void){
    char ATIS[NBMAXATIS][MSG_SIZE];
    int fichierTexte,fichierLock, nbATIS, i;
    
    nbATIS = 2;

    strncpy(ATIS[0], "EBLG 1803 00000KT 0600 FG OVC008 BKN040 PROB40 2024 0300 DZ FG OVC002 BKN040", MSG_SIZE);
    strncpy(ATIS[1], "EBBR 0615 20015KT 8000 RA SCT010 OVC015 TEMPO 0608 5000 RA BKN005 BECMG 0810 9999 NSW BKN025", MSG_SIZE);

    if ((fichierLock = open(FICHIERLOCK, O_CREAT | O_WRONLY | O_SYNC | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP)) == FAIL) {
        printf("Impossible de faire le fichier lock");
        exit(EXIT_FAILURE);
    }

    if ((fichierTexte = open(FICHIERMETEO, O_CREAT | O_WRONLY | O_SYNC | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP)) == FAIL) {
        printf("Impossible d'ouvrir le fichier meteo\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < nbATIS; i++){
        write(fichierTexte, ATIS[i], sizeof(ATIS[0]));
    }

    close(fichierTexte);

    close(fichierLock);
}
