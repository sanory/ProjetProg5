#include "elfFile.h"
#include <stdio.h>

int display_rel_section(fichierElf * MonfichierElf) {

    int i,j;

    for (i = 0; i < MonfichierElf->nbRelSection; i++) {
        printf("%d num section\n",MonfichierElf->RelSections[i].nbSection);
        printf("Section de readressage '%s' a l'adresse de decalage 0x%x contient %u entrees:\n",
                MonfichierElf->SectNames + MonfichierElf->secHeader[MonfichierElf->RelSections[i].nbSection].sh_name,
                MonfichierElf->secHeader[MonfichierElf->RelSections[i].nbSection].sh_offset,
                MonfichierElf->RelSections[i].nbSection+1);
        
        
        printf("Decalage\tInfoþtype\t\t\tVal.-sym\tNoms-symboles\n");
        for (j = 0; j < MonfichierElf->RelSections[i].RelTableSize; j++) {
            printf("%08x\n",MonfichierElf->RelSections[i].RelTable[j].r_offset);
        }
    }
    return 1;
}