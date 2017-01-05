#include <elf.h>
#include <stdio.h>
#include <stdlib.h>


int desalocSymbTable(Elf32_Ehdr * header, Elf32_Shdr **SecHeader, Elf32_Sym *** SymbTab){
	int i, j;
	if ((*SymbTab) == NULL){
		free(*SymbTab);
		return 0;
	}
	for (i=0;i<header->e_shnum; i++){
		if(SecHeader[i]->sh_type == SHT_SYMTAB){
			for (j=0;j<SecHeader[i]->sh_size/sizeof(Elf32_Sym); j++){
				if ((*SymbTab)[j]!=NULL)
					free((*SymbTab)[j]);
			}
		}
	}
	free (*SymbTab);
	if ((*SymbTab) == NULL){
		return 0;
	} else {
		return 1;
	}
}

//Atention &le pointeur Elf32_Sym
int readSymbTable(Elf32_Shdr ** SecHeader, Elf32_Ehdr * header,  FILE *fichier, Elf32_Sym *** SymbTab){
	int i, j, k;

	
	//pour rechercher la section contenant les symboles il faut ballayer toute les sections jusqu'a trouver cellle de type SHT_SYMTAB
	

	for( i = 0 ; i < header->e_shnum ; i++ ) {
		if (SecHeader[i]->sh_type == SHT_SYMTAB){
			//on a trouvee une table des symboles
			
			fseek(fichier,SecHeader[i]->sh_offset,SEEK_SET);
			
			
			//creation du pointeur
			//on utilise la taille de la section car toute la section est de ce type
			(*SymbTab) = (Elf32_Sym **) malloc(SecHeader[i]->sh_size);
			
			if(SymbTab == NULL) {
				free (*SymbTab);
				return 1;
			}
			for (j=0; j < SecHeader[i]->sh_size/sizeof(Elf32_Sym); j++){
				(*SymbTab)[j]=(Elf32_Sym*) malloc(sizeof(Elf32_Sym));
				
				//on verifie que ca c'est bien passsee
				if ((*SymbTab)[j]==NULL){
					//si pas ok on free le pointeur et on envoie 1
					for (k=j;k>=0;k--)
						free((*SymbTab)[k]);
					free(*SymbTab);
					return 1;
				} else {
					//c'est ok donc on lit le fichier
					fread((*SymbTab)[j],sizeof(Elf32_Sym),1,fichier);
				}
			}

			return 0;
		}
	} 
return 1;	
}
