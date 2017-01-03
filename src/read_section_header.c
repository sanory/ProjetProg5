
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>



int read_section_header(FILE *fichier, Elf32_Ehdr *header, Elf32_Shdr **SecHeader){
	int i;
if (header->e_shnum==0) {
	//ya pas de table return 2
	SecHeader = (Elf32_Shdr **) malloc(header->e_shnum*sizeof(Elf32_Shdr)); 
	return 2;
} else {
	//ya au moins une section
//	uint32_t addrsht=SEEK_END-(header->e_shnum*header->e_shentsize)
	SecHeader = (Elf32_Shdr **) malloc(header->e_shnum*sizeof(Elf32_Shdr)); 
	if (SecHeader == NULL)
		return 1;
	for (i=0; i < header->e_shnum; i++) {
		SecHeader[i]=(Elf32_Shdr*) malloc(sizeof(Elf32_Shdr));
		if (SecHeader[i]==NULL)
			return 1;	
	}
	
	for (i=header->e_shnum; i>0 ; i=-header->e_shnum) {
		fread(header,sizeof(Elf32_Ehdr),SEEK_END-(header->e_shnum*header->e_shentsize),fichier);	
	}

	return 0;
	}
return 1;
}
