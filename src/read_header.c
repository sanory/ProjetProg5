
#include <elf.h>
#include <stdlib.h>
#include <stdio.h>

int read_header(FILE* fichier, Elf32_Ehdr* header){ 

	fread(&header,sizeof(Elf32_Ehdr),1,fichier);
	
	if ('E'!=header->e_ident[1] && 'L'!=header->e_ident[2] && 'F'!=header->e_ident[3])
		return 1;
	

	return 0;
}
