
#include <elf.h>
#include <stdio.h>
#include "endian.h"

int display_section(FILE* fichier, int nbSections, Elf32_Shdr ** SecHeader, Elf32_Ehdr *header){



		printf("shtype=%d\n",SecHeader[nbSections]->sh_type);
		printf("shflags=%d\n",SecHeader[nbSections]->sh_flags);
		printf("shaddr=%d\n",SecHeader[nbSections]->sh_addr);
		printf("shoffset=%d\n",SecHeader[nbSections]->sh_offset);
		printf("shsize=%d\n",SecHeader[nbSections]->sh_size);
		printf("shlink=%d\n",SecHeader[nbSections]->sh_link);
		printf("shinfo=%d\n\n",SecHeader[nbSections]->sh_info);


//a ajouter la conversiont du sh_name dnas la table des entetes de sections
//	printf("Section %s\n",(char *) SecHeader[nbSections]->sh_name);
        printf("addr |                data");
        printf("\n------------------------------------------\n");

	int i, k = 0;
	int nbbits=4;//nombre de bits par paquets a l'ecran
	unsigned int hex = 0x0;
	//a tester mais normalement le boutisme na aucune espece d'importance dans cette section
	/*
	//verification du boutisme
	if (header->e_ident[EI_DATA]==ELFDATA2LSB){
		//presence de petit boutisme conversion obligatoire
		fseek(fichier,htobe32(SecHeader[nbSections]->sh_offset), SEEK_SET);
		//ont lit la section tout en formatant l'affichage
		for (i=htobe32(SecHeader[nbSections]->sh_offset); i <htobe32(SecHeader[nbSections]->sh_offset)+htobe32(SecHeader[nbSections]->sh_size);i+=nbbits){
			if (k%nbbits==0){
				printf("\n%04x | ", i);	
			}
			k++;
			fread(&hex, 4, 1, fichier);
			hex=htobe32(hex);
			printf("%08x ", hex);
			hex=0;
		}
	}else {
	*/
		//pas de conversion a faire
		
		//if (SecHeader[nbSections]->sh_type==SHT_NOBITS)
			//return 1;
		
		
		
		
		
		
			
		//on lit tout le contenue de la section
		printf("%d\n",SecHeader[nbSections]->sh_offset);
		for (i=SecHeader[nbSections]->sh_offset; i >SecHeader[nbSections]->sh_offset+SecHeader[nbSections]->sh_size;i+=nbbits){
                        if (k%nbbits==0){
				printf("\n%04x | ", i);
                        }
                        k++;
                        fread(&hex, nbbits, 1, fichier);
			printf("%08x ", hex);
                        hex=0;
        }
	//}
	printf("\n------------------------------------------\n");
        printf("addr |      data");
        printf("\n\n");
	
	return 0;	
}
