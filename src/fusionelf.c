#include <stdlib.h>
#include "elfFile.h"
#include <string.h>

int fusion_elfFile(FILE* fichier, fichierElf  *MonfichierElf1, fichierElf  *MonfichierElf2, fichierElf * MonfichierElfresultat){
int i,j;
int * T;
int arretBoucle = 0;
Elf32_Off decalage; // décalage par rapport au fichier 1 et non pas au début du fichier
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
		if(MonfichierElf1->secHeader[i].sh_type == SHT_PROGBITS && MonfichierElf2->secHeader[j].sh_type == SHT_PROGBITS && !strcmp(MonfichierElf1->secHeader[i].sh_name+MonfichierElf1->SectNames,MonfichierElf2->secHeader[j].sh_name+MonfichierElf2->SectNames)){
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
//allocation d'un tableau de bool
T=calloc(MonfichierElf2->nbSections ,sizeof (int));

for(i=0; i<MonfichierElf1->nbSections; i++){
	for(j=0; j<MonfichierElf2->nbSections && !(arretBoucle); j++){
		if(MonfichierElf1->secHeader[i].sh_type == SHT_PROGBITS && MonfichierElf2->secHeader[j].sh_type == SHT_PROGBITS && !strcmp(MonfichierElf1->secHeader[i].sh_name + MonfichierElf1->SectNames, MonfichierElf2->secHeader[j].sh_name+MonfichierElf2->SectNames)){	
			MonfichierElfresultat->secHeader[i].sh_name = MonfichierElf1->secHeader[i].sh_name;
			MonfichierElfresultat->secHeader[i].sh_type = MonfichierElf1->secHeader[i].sh_type;
			MonfichierElfresultat->secHeader[i].sh_flags = (MonfichierElf1->secHeader[i].sh_flags & MonfichierElf2->secHeader[j].sh_flags);
			MonfichierElfresultat->secHeader[i].sh_addr = MonfichierElf1->secHeader[i].sh_addr+decalage;
			MonfichierElfresultat->secHeader[i].sh_offset = MonfichierElf1->secHeader[i].sh_offset+decalage;
			MonfichierElfresultat->secHeader[i].sh_size = MonfichierElf1->secHeader[i].sh_size + MonfichierElf2->secHeader[j].sh_size;
			MonfichierElfresultat->secHeader[i].sh_link = MonfichierElf1->secHeader[i].sh_link;
			MonfichierElfresultat->secHeader[i].sh_info = MonfichierElf1->secHeader[i].sh_info;
			MonfichierElfresultat->secHeader[i].sh_addralign = MonfichierElf1->secHeader[i].sh_addralign;
			MonfichierElfresultat->secHeader[i].sh_entsize = MonfichierElf1->secHeader[i].sh_entsize;
			arretBoucle=1;
			
			// a faire fwrite la concatenation des deux sections
			decalage = decalage + MonfichierElf2->secHeader[j].sh_size;
			T[j] = 1;
		}
	}
	//SI CA A PAS ETE FUSIONNE, INSERER LA SECTION ET SON EN TETE QUAND MEME, [ICI]
	if(!arretBoucle){
		MonfichierElfresultat->secHeader[i] = MonfichierElf1->secHeader[i];
		MonfichierElfresultat->secHeader[i].sh_addr = MonfichierElf1->secHeader[i].sh_addr+decalage;
		MonfichierElfresultat->secHeader[i].sh_offset = MonfichierElf1->secHeader[i].sh_offset+decalage;
		// a faire fwrite la section
	}
	arretBoucle = 0;
}

for(i=0; i<MonfichierElf2->nbSections; i++){
	if(T[i]==0){
		MonfichierElfresultat->secHeader[i] = MonfichierElf2->secHeader[i];
		MonfichierElfresultat->secHeader[i].sh_addr = MonfichierElf2->secHeader[i].sh_addr+decalage;
		MonfichierElfresultat->secHeader[i].sh_offset = MonfichierElf2->secHeader[i].sh_offset+decalage;
		// a faire fwrite la section
	}
}
//penser à e_shoff apres avoir display les sections


}
