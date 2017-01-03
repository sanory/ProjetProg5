
#include <elf.h>
#include <stdlib.h>
#include <stdio.h>

int read(int octet, int endian, FILE* fichier){
	int i, estlu=0,temp=0;
	if (endian ==0){
		for (i=0;i<octet;i++){
			fread(&temp,sizeof(char),1, fichier);
			estlu=estlu | temp <<8*(octet -1 -i);	
		}
	} else {
		for (i=0;i<octet;i++){
			fread(&temp,sizeof(char),1,fichier);
			estlu=estlu | temp <<8 * 1;
		}
	}
	return estlu;
}


int read_header(FILE* fichier, Elf32_Ehdr* header){ 
	int i, endian=0;
	
	//lecture du magic cookies
	
	unsigned char elf[3] = {'E', 'L', 'F'};
	
	unsigned char ident[16];

	for (i=0; i<16;i++){
		ident[i]=read(i,endian,fichier);
	}

	//verification du type de fichier ce n'est pas un fichier ELF
	if (elf[0]!=ident[1] && elf[1]!=ident[2] && elf[2]!=ident[3])
		return 1;

	
	for (i=0;i<16;i++)
		header->e_ident[i]=ident[i];

	return 0;
}
