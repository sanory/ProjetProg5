#include "elfFile.h"
#include <stdio.h>
#include <string.h>

int display_rel_section(fichierElf * MonfichierElf) {

	int i, j;
	for (i = 0; i < MonfichierElf->nbRelSection; i++){ //Un tour de boucle par section
        printf("\nSection de readressage '%s' a l'adresse de decalage 0x%x contient %u entrees:\n",
                MonfichierElf->SectNames +
                MonfichierElf->secHeader[MonfichierElf->RelSections[i].numSection].sh_name,//Nom de la section
                MonfichierElf->secHeader[MonfichierElf->RelSections[i].numSection].sh_offset, //Adresse de décalage
                MonfichierElf->RelSections[i].RelTableSize);//Nombre d'entrées


        printf("Decalage\tInfo\t\ttype\t\t\tVal.-sym\tNoms-symboles\n");
        for (j = 0; j < MonfichierElf->RelSections[i].RelTableSize; j++) { //un tour de boucle pas entrée de section (ligne)

            printf("%08x\t%08x \t", MonfichierElf->RelSections[i].RelTable[j].r_offset, //décalage
                    MonfichierElf->RelSections[i].RelTable[j].r_info); //info

            switch (ELF32_R_TYPE(MonfichierElf->RelSections[i].RelTable[j].r_info)) { //Type (donné en fonction des deux dernier chiffres donnés dans info
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
                    printf("%i\t", MonfichierElf->RelSections[i].RelTable[j].r_offset);
                    break;
            }

            printf("\t\t%08x", ELF32_R_SYM(MonfichierElf->symTable[j].st_value));//Valeur

			//Trouver le nom en utilisant les troisième et quatrième chiffres de info pour retrouver la correspondance dans la table de symbole.
            if (ELF32_ST_TYPE(MonfichierElf->symTable[ELF32_R_SYM(MonfichierElf->RelSections[i].RelTable[j].r_info)].st_info) == 3)//verifier si le type est Section
                printf("\t%s\n", MonfichierElf->SectNames + MonfichierElf->secHeader[MonfichierElf->symTable[ELF32_R_SYM(MonfichierElf->RelSections[i].RelTable[j].r_info)].st_shndx].sh_name); //Dans ce cas on cherche, grâce au "NDX" de la table des symboles, le numéro de la section correspondante dans la table de section et on affiche son nom
			
            else//si le type n'est pas section, on affiche le nom du symbole correspondant dans la table des symboles.
                printf("\t%s\n", MonfichierElf->SymbNames + MonfichierElf->symTable[ELF32_R_SYM(MonfichierElf->RelSections[i].RelTable[j].r_info)].st_name);

        }
    }
    return 1;
}
