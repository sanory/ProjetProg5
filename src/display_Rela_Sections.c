#include "elfFile.h"
#include <stdio.h>

int display_rela_section(fichierElf * MonfichierElf) {

    int i,j;

    for (i = 0; i < MonfichierElf->nbRelaSection; i++) {
        printf("\nSection de readressage '%s' a l'adresse de decalage 0x%x contient %u entrees:\n",
                MonfichierElf->SectNames + MonfichierElf->secHeader[MonfichierElf->RelaSections[i].numSection].sh_name,
                MonfichierElf->secHeader[MonfichierElf->RelaSections[i].numSection].sh_offset,
                MonfichierElf->RelaSections[i].numSection+1);
        
        
        printf("Decalage\tInfo\ttype\t\t\tVal.-sym\tNoms-symboles\n");
        for (j = 0; j < MonfichierElf->RelaSections[i].RelaTableSize; j++) {
            printf("%08x  %08x \t%d\t\t\t%d\n",MonfichierElf->RelaSections[i].RelaTable[j].r_offset,
                    MonfichierElf->RelaSections[i].RelaTable[j].r_info,
                    ELF32_R_TYPE(MonfichierElf->RelSections[i].RelTable[j].r_info),
                    ELF32_R_SYM(MonfichierElf->RelSections[i].RelTable[j].r_info));
        }
    }
    return 1;
}