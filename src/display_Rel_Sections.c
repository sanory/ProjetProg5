#include "elfFile.h"
#include <stdio.h>

int display_rel_section(fichierElf * MonfichierElf) {

    int i, j;

    for (i = 0; i < MonfichierElf->nbRelSection; i++) {
        printf("\nSection de readressage '%s' a l'adresse de decalage 0x%x contient %u entrees:\n",
                MonfichierElf->SectNames + MonfichierElf->secHeader[MonfichierElf->RelSections[i].numSection].sh_name,
                MonfichierElf->secHeader[MonfichierElf->RelSections[i].numSection].sh_offset,
                MonfichierElf->RelSections[i].RelTableSize);


        printf("Decalage\tInfo\t\ttype\t\t\tVal.-sym\tNoms-symboles\n");
        for (j = 0; j < MonfichierElf->RelSections[i].RelTableSize; j++) {

            printf("%08x\t%08x \t",MonfichierElf->RelSections[i].RelTable[j].r_offset,
                    MonfichierElf->RelSections[i].RelTable[j].r_info);

            switch(ELF32_R_TYPE(MonfichierElf->RelSections[i].RelTable[j].r_info)){
                case 2:
                //printf("");
                break;

                default:
                printf("\t");
                break;
            }
            

            printf("\t\t\t%d\n",
                    ELF32_R_SYM(MonfichierElf->RelSections[i].RelTable[j].r_info));
        }
    }
    return 1;
}