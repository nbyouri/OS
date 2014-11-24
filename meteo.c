#include "global.h"

#define nbMaxATIS 20
#define nbMaxCaraATIS 150

//Fichier C qui va mettre à jour le fichier texte meteo
int gen_atis(void){
    char ATIS[nbMaxATIS][nbMaxCaraATIS];
    int fichierTexte, nbATIS, i;
    nbATIS = 2;

    strcpy(ATIS[0], "EBLG 1803 00000KT 0600 FG OVC008 BKN040 PROB40 2024 0300 DZ FG OVC002 BKN040");
    strcpy(ATIS[1], "EBBR 0615 20015KT 8000 RA SCT010 OVC015 TEMPO 0608 5000 RA BKN005 BECMG 0810 9999 NSW BKN025");

    //ouverture, ectriture et fermeture du fichier meteo.txt
    if((fichierTexte = open("meteo.txt", O_WRONLY)) == -1){
			printf("Impossible d'ouvrir le fichier meteo");
			return -1;
    }
    for(i=0; i<nbATIS; i++){
        write(fichierTexte,ATIS[i],sizeof ATIS[0]);
    }
    printf("\nLe fichier meteo a correctement ete ecrit!");
    close(fichierTexte);
    return 0;
}
