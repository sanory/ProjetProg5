
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>

int desalocSecTable(Elf32_Ehdr *header, Elf32_Shdr ***SecHeader){
	int i;
	if ((*SecHeader) == NULL){
		free(*SecHeader);
		return 0;
	}
	for (i=0; i < header->e_shnum; i++) {
		if ((*SecHeader)[i]!=NULL){
				free((*SecHeader)[i]);
		}
	}
	free(*SecHeader);
	if ((*SecHeader) == NULL) {
		return 0;
	} else  {
		return 1;
	}
	
}

int read_section_header(FILE *fichier, Elf32_Ehdr *header, Elf32_Shdr ***SecHeader){
	int i,j;
if (header->e_shnum==0) {
	//ya pas de table return 2
	//SecHeader = (Elf32_Shdr **) malloc(header->e_shnum*sizeof(Elf32_Shdr)); 
	//pas de section mais desalocation du pointeurs des tables de sections
	return 2;
} else {
	//ya au moins une section
//	uint32_t addrsht=SEEK_END-(header->e_shnum*header->e_shentsize)
	(*SecHeader) = (Elf32_Shdr **) malloc(header->e_shnum*sizeof(Elf32_Shdr)); 
	if (SecHeader == NULL){
		free(*SecHeader);
		return 1;
	}
	for (i=0; i < header->e_shnum; i++) {
		(*SecHeader)[i]=(Elf32_Shdr*) malloc(sizeof(Elf32_Shdr));
		if ((*SecHeader)[i]==NULL){
			for (j=i; j>=0; j--)
				free((*SecHeader)[j]);
			free(*SecHeader);
			return 1;
		}
	}
	
	//printf("shoff=%d\n",header->e_shoff);
	
	fseek(fichier,header->e_shoff,SEEK_SET);
	
	for (i=0; i< header->e_shnum; i++) {
		
		
	//fseek(fichier,-((header->e_shnum - i )*header->e_shentsize),SEEK_END);
		
	//	int test= ftell(fichier);
		
		fread((*SecHeader)[i],sizeof(Elf32_Shdr),1,fichier);		
	/*	
		printf("posfichier=%d\n",test);
		printf("numSess=%d\n",i);
		printf("shtype=%04x\n",(*SecHeader)[i]->sh_type);
		printf("shflags=%04x\n",(*SecHeader)[i]->sh_flags);
		printf("shaddr=%04x\n",(*SecHeader)[i]->sh_addr);
		printf("shoffset=%04x\n",(*SecHeader)[i]->sh_offset);
		printf("shsize=%04x\n",(*SecHeader)[i]->sh_size);
		printf("shlink=%04x\n",(*SecHeader)[i]->sh_link);
		printf("shinfo=%04x\n\n",(*SecHeader)[i]->sh_info);
	*/	
	}

	return 0;
	}
}
