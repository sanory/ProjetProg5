#include <stdlib.h>
#include "elfFile.h"
#include <string.h>

int fusion_elfFile(FILE* fichier, fichierElf  MonfichierElf1, fichierElf  MonfichierElf2, fichierElf * MonfichierElfresultat){
int i,j;
//init
MonfichierElfresultat->nbSections = 0;
MonfichierElfresultat->nbSymb = 0;
//fichier à créer
MonfichierElfresultat->fichier=fichier;
//copie du header (à mettre à jour)
MonfichierElfresultat->header=MonfichierElf1->header;

MonfichierElfresultat->nbSections = MonfichierElf1->nbSections+MonfichierElf2->nbSections;

//calcul du nouveau nombre de sections
for(i=0; i<MonfichierElf1->nbSections; i++){
	for(j=0; j<MonfichierElf2->nbSections; j++){
		if(MonfichierElf1->secHeader[i].sh_type == PROGBITS && MonfichierElf2->secHeader[j].sh_type == PROGBITS && !strcmp(MonfichierElf1->secHeader[i].sh_name+MonfichierElf1->SectNames,MonfichierElf2->secHeader[j].sh_name+MonfichierElf2->SectNames)){
			MonfichierElfresultat->nbSections--;
		}
	}
}

MonfichierElfresultat->header.e_shnum = MonfichierElfresultat->nbSections;

MonfichierElfresultat->secHeader =
            malloc(MonfichierElfresultat->nbSections * sizeof (Elf32_Shdr));
    if (MonfichierElfresultat->secHeader == NULL)
        return 2; //erreur dans la creation du pointeur

return 1;






}
