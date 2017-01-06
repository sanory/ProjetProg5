#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>
#include "elfFile.h"

int display_section(int nbSections, fichierElf * elfFile){


//a ajouter la conversiont du sh_name dnas la table des entetes de sections
//	printf("Section %s\n",(char *) secHeader[nbSections]->sh_name);

	if(nbSections==0 || nbSections>elfFile->nbSections)
		return 2;

	if (elfFile->secHeader[nbSections].sh_type==SHT_NOBITS)
		return 1;

	printf("Vidange hexadécimale de la section «%s»:\n",elfFile->SectNames + elfFile->secHeader[nbSections].sh_name);
	printf("addr |                data");
	printf("\n------------------------------------------");
	int i, k = 0;
	int nbbits=4;//nombre de bits par paquets a l'ecran
	unsigned int hex = 0x0;


		fseek(elfFile->fichier,elfFile->secHeader[nbSections].sh_offset,SEEK_SET);
		
			
			//on lit tout le contenue de la section
	for (i=elfFile->secHeader[nbSections].sh_offset; i < elfFile->secHeader[nbSections].sh_offset+elfFile->secHeader[nbSections].sh_size;i=i+nbbits){
		if (k%nbbits==0){
			printf("\n%04x | ", i);
		}
		k++;
		fread(&hex, nbbits, 1, elfFile->fichier);///A VERIFIER          
		printf("%08x ", hex);
	}
	printf("\n------------------------------------------\n");
	printf("addr |      data");
	printf("\n\n");
	
	return 0;
		
}

int display_section_nom(char * nom, fichierElf * elfFile){

	int i=0;

	while(i<(elfFile->nbSections)-1 && strcmp(elfFile->SectNames+elfFile->secHeader[i].sh_name,nom)){
		i=i+1;
	}
	if (!strcmp(elfFile->SectNames+elfFile->secHeader[i].sh_name,nom))
		return display_section(i, elfFile); /// A VERIFIER
	return 2;
}
