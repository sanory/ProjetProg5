
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "endian.h"

int display_section(FILE* fichier, int nbSections, Elf32_Shdr ** SecHeader, Elf32_Ehdr *header){


//a ajouter la conversiont du sh_name dnas la table des entetes de sections
//	printf("Section %s\n",(char *) SecHeader[nbSections]->sh_name);

	if(nbSections==0 || nbSections>header->e_shnum)
		return 2;

	if (SecHeader[nbSections]->sh_type==SHT_NOBITS)
		return 1;

	 char * SectNames = NULL;

	SectNames = malloc(SecHeader[header->e_shstrndx]->sh_size);
	fseek(fichier, SecHeader[header->e_shstrndx]->sh_offset, SEEK_SET);
  	fread(SectNames, 1, SecHeader[header->e_shstrndx]->sh_size, fichier);

	printf("Vidange hexadécimale de la section «%s»:\n",SectNames + SecHeader[nbSections]->sh_name);
	printf("addr |                data");
	printf("\n------------------------------------------");
	int i, k = 0;
	int nbbits=4;//nombre de bits par paquets a l'ecran
	unsigned int hex = 0x0;


		fseek(fichier,SecHeader[nbSections]->sh_offset,SEEK_SET);
		
			
			//on lit tout le contenue de la section
	for (i=SecHeader[nbSections]->sh_offset; i <SecHeader[nbSections]->sh_offset+SecHeader[nbSections]->sh_size;i=i+nbbits){
		if (k%nbbits==0){
			printf("\n%04x | ", i);
		}
		k++;
		fread(&hex, nbbits, 1, fichier);
		          
		printf("%08x ", hex);
		           //hex=0;
	}
		//}
	printf("\n------------------------------------------\n");
	printf("addr |      data");
	printf("\n\n");
	
	return 0;	
}

int display_section_nom(FILE* fichier, char * nom, Elf32_Shdr ** SecHeader, Elf32_Ehdr *header){


	char * SectNames = NULL;

	SectNames = malloc(SecHeader[header->e_shstrndx]->sh_size);
	fseek(fichier, SecHeader[header->e_shstrndx]->sh_offset, SEEK_SET);
  	fread(SectNames, 1, SecHeader[header->e_shstrndx]->sh_size, fichier);
	
	int i=0;

	while(i<(header->e_shnum)-1 && strcmp(SectNames+SecHeader[i]->sh_name,nom)){
		i=i+1;
	}
	if (!strcmp(SectNames+SecHeader[i]->sh_name,nom))
		return display_section(fichier, i, SecHeader, header);
	return 2;
}
