#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>
#include "elfFile.h"

int display_section(int nbSections, fichierElf * elfFile) {

    if (nbSections > elfFile->nbSections)
        return 2;

    if (elfFile->secHeader[nbSections].sh_type == SHT_NOBITS) {
        fprintf(stderr, "la section \" %s \" n'a pas pu etre vidange\n\n",
                elfFile->SectNames + elfFile->secHeader[nbSections].sh_name);
        return 1;
    }

    if (elfFile->secHeader[nbSections].sh_size == 0) {
        fprintf(stderr, "la section \" %s \" n'a pas pu etre vidange\n\n",
                elfFile->SectNames + elfFile->secHeader[nbSections].sh_name);
        return 3;
    }

    printf("Vidange hexadécimale de la section «%s»:\n",
            elfFile->SectNames + elfFile->secHeader[nbSections].sh_name);
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
    printf("\n\n");

    return 0;

}

int display_section_nom(char * nom, fichierElf * elfFile) {

    int i = 0;

    while (i < (elfFile->nbSections) - 1 &&
            strcmp(elfFile->SectNames + elfFile->secHeader[i].sh_name, nom)) {
        i++;
    }

    if (!strcmp(elfFile->SectNames + elfFile->secHeader[i].sh_name, nom))
        return display_section(i, elfFile);
    return 2;
}
