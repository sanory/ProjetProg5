
#include <elf.h>
#include <stdio.h>

int read_header(FILE* fichier, Elf32_Ehdr* header){ 

	fread(header,sizeof(Elf32_Ehdr),1,fichier);

	//verification que il s'agit biens d'un fichier ELF 	
	if (ELFMAG0!=header->e_ident[EI_MAG0] && ELFMAG1!=header->e_ident[EI_MAG1] && ELFMAG2!=header->e_ident[EI_MAG2] && ELFMAG3!=header->e_ident[EI_MAG3])
		return 1;
	
	//verification qu'il s'agit bien d'un fichier 32bit
	if (header->e_ident[EI_CLASS]!=ELFCLASS32)
		return 1;

	return 0;
}



