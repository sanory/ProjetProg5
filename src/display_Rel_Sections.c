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
                case 0:
                printf("R_ARM_NONE");
                break;

                case 2:
                printf("R_ARM_ABS32");
                break;

                case 28:
                printf("R_ARM_CALL");
                break;

                case 40:
                printf("R_ARM_V4BX");
                break;

                default:
                printf("%i\t",MonfichierElf->RelSections[i].RelTable[j].r_offset);
                break;
            }
            
		printf("\t\t%08x",ELF32_R_SYM(MonfichierElf->symTable[j].st_value));

		//if(ELF32_ST_TYPE(MonfichierElf->symTable[j].st_info)!=3){
                printf("\t%s\n",MonfichierElf->SymbNames + MonfichierElf->symTable[ELF32_R_SYM(MonfichierElf->RelSections[i].RelTable[j].r_info)].st_name);
           //}else{
                /*MonfichierElf->SectNames + MonfichierElf->secHeader[i].sh_name*/
                /*f->symTable[j].st_shndx*/
                //printf("\t%s\n",MonfichierElf->SectNames + MonfichierElf->secHeader[MonfichierElf->symTable[j].st_shndx].sh_name);
                //printf("\tsss\n");
            //}


		//printf("\t%s\n",MonfichierElf->SymbNames + MonfichierElf->symTable[ELF32_R_SYM(MonfichierElf->RelSections[i].RelTable[j].r_info)].st_name);
		//printf("\t\t%d\n",ELF32_R_SYM(MonfichierElf->RelSections[i].RelTable[j].r_info));
        }
    }
    return 1;
}
