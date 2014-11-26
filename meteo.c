#include "global.h"

//Fichier C qui va mettre à jour le fichier texte meteo
int gen_atis(void){
    char ATIS[NBMAXATIS][MSG_SIZE];
    int fichierTexte,fichierLock, nbATIS, i;
    nbATIS = 2;

    strncpy(ATIS[0], "EBLG 1803 00000KT 0600 FG OVC008 BKN040 PROB40 2024 0300 DZ FG OVC002 BKN040", MSG_SIZE);
    strncpy(ATIS[1], "EBBR 0615 20015KT 8000 RA SCT010 OVC015 TEMPO 0608 5000 RA BKN005 BECMG 0810 9999 NSW BKN025", MSG_SIZE);

    if((fichierLock = open(FICHIERLOCK, O_WRONLY | O_CREAT)) == FAIL){
		printf("Impossible de faire le fichier lock");
        return FAIL;
    }

    //ouverture, ectriture et fermeture du fichier meteo.txt
    if((fichierTexte = open(FICHIERMETEO, O_WRONLY | O_CREAT)) == FAIL){
		printf("Impossible d'ouvrir le fichier meteo\n");
        return FAIL;
    }
    
    for(i=0; i<nbATIS; i++){
        write(fichierTexte,ATIS[i], sizeof ATIS[0]);
    }
    
    printf("\nLe fichier meteo a correctement ete ecrit!");
    
    close(fichierTexte);
    
    close(fichierLock);

    /*if(unlink(FICHIERLOCK) == FAIL){
			fatal("Impossible de detruire le fichier lock");
    }*/

    return SUCCESS;
}
