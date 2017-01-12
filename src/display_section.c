#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>
#include "elfFile.h"

int display_section(int nbSections, fichierElf * elfFile) {

    if (nbSections > elfFile->nbSections)
        return 2; //Erreur, si le nombre donné est incorrect (plus grand que le nombre maximal de section)

    if (elfFile->secHeader[nbSections].sh_type == SHT_NOBITS) {
        printf("la section \" %s \" n'a pas pu etre vidange\n\n",
                elfFile->SectNames + elfFile->secHeader[nbSections].sh_name);//nom de la section
        return 1; //Erreur, si il n'y a rien a afficher
    }

    if (elfFile->secHeader[nbSections].sh_size == 0) {
        printf("la section \" %s \" n'a pas pu etre vidange\n\n",
                elfFile->SectNames + elfFile->secHeader[nbSections].sh_name);//nom de la section
        return 3;// Erreur, si il n'y a pas de section.
    }

    printf("Vidange hexadécimale de la section «%s»:\n",
            elfFile->SectNames + elfFile->secHeader[nbSections].sh_name); //nom de la section
    printf("    addr    |                data");
    printf("\n--------------------------------------------------");
    int i;
    unsigned int hex = 0x0;


    //on lit tout le contenue de la section
    for (i = 0; i < elfFile->LesSections[nbSections].longueurSect; i = i + 1) {
        if (i % 16 == 0) {
            printf("\n  0x%08x ", i);
        }
        hex = elfFile->LesSections[nbSections].contenu[i];
        printf("%02x", hex);
        if ((i + 1) % 4 == 0) {
            printf(" ");
        }
    }
    printf("\n--------------------------------------------------\n");
    printf("    addr    |                data");
    printf("\n");

    return 0;

}

int display_section_nom(char * nom, fichierElf * elfFile) {
	//Retrouve le numéro de section a partir de son nom, puis utilise la fonction précédente en utilisant ce numéro.
    int i = 0;

    while (i < (elfFile->nbSections) - 1 &&
            strcmp(elfFile->SectNames + elfFile->secHeader[i].sh_name, nom)) {//Tant qu'on a pas atteind la fin de la section et tant qu'on a pas trouvé le nom donné
        i++;
    }

    if (!strcmp(elfFile->SectNames + elfFile->secHeader[i].sh_name, nom))
        return display_section(i, elfFile);// Si il y a un nom correspondant
    return 2;//Erreur, on a atteind la fin sans rien trouver, la section n'existe pas dans le fichier.
}
