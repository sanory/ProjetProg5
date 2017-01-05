
#include <elf.h>
#include <stdlib.h>
#include <stdio.h>


int desalocRelaTable(Elf32_Rela **** RelaTable, Elf32_Ehdr * header, Elf32_Shdr ** SecHeader){

if ((*RelaTable)==NULL)
	return 0;

int i,j, nb_rela=0; 

for( i = 0 ; i < header->e_shnum ; i++ ) {
	if (SecHeader[i]->sh_type == SHT_RELA)
		nb_rela++;
}

for (i=0;i<nb_rela; i++){
	if ((*RelaTable)[i]!=NULL){
		for (j=0; j<SecHeader[i]->sh_size/sizeof(Elf32_Rela);j++)
			free((*RelaTable)[i][j]);
	}
	free((*RelaTable)[i]);
}
free(*RelaTable);
return 0;
}

int desalocRelTable(Elf32_Rel **** RelTable, Elf32_Ehdr * header, Elf32_Shdr ** SecHeader){

if ((*RelTable)==NULL)
	return 0;
	
int i,j, nb_rel=0; 

for( i = 0 ; i < header->e_shnum ; i++ ) {
	if (SecHeader[i]->sh_type == SHT_REL)
		nb_rel++;
}

for (i=0;i<nb_rel; i++){
	if ((*RelTable)[i]!=NULL){
		for (j=0; j<SecHeader[i]->sh_size/sizeof(Elf32_Rel);j++)
			free((*RelTable)[i][j]);
	}
	free((*RelTable)[i]);
}
free(*RelTable);
return 0;
}


//attention a donner les tables avec le & pour les ***
int read_relocations_table(FILE *fichier, Elf32_Rel **** RelTable, Elf32_Rela **** RelaTable, Elf32_Shdr ** SecHeader, Elf32_Ehdr * header){
int i,j,k,l, nb_rel=0, nb_rela=0, nb_done=0;

for( i = 0 ; i < header->e_shnum ; i++ ) {
	if (SecHeader[i]->sh_type == SHT_REL)
		nb_rel++;
}
//On alloue le nb de table rel trouvées
(*RelTable)=(Elf32_Rel ***) malloc(sizeof(Elf32_Rel*)*nb_rel);
if(RelTable == NULL) {
	free (*RelTable);
	return 1;
}
//On cherche chaque table rel
for( i =0 ; i < header->e_shnum && nb_done<nb_rel ; i++ ) {
	if (SecHeader[i]->sh_type == SHT_REL){
		//on a trouvee une table des rel
		fseek(fichier,SecHeader[i]->sh_offset,SEEK_SET);
		(*RelTable)[i]=(Elf32_Rel **) malloc(SecHeader[i]->sh_size/sizeof(Elf32_Rel));	
		if ((*RelTable)[i]==NULL){
			//si pas ok on free le pointeur et on envoie 1
			for (k=nb_done;k>=0;k--)
				free((*RelTable)[k]);
			free(*RelTable);
			return 1;
		} 
		else {
			//Sinon on continue
						
			//pour chaque symbole de cette table
			for (j=0; j<SecHeader[i]->sh_size/sizeof(Elf32_Rel); j++){
				(*RelTable)[i][j]=(Elf32_Rel *) malloc(sizeof(Elf32_Rel));
				if ((*RelTable)[i][j]==NULL){
					//si pas ok on free le pointeur et on envoie 1
					for(l=i;l>=0;l--){
						for (k=j;k>=0;k--)
							free((*RelTable)[l][k]);
						free((*RelTable)[l]);	
					}	
					free(*RelTable);
					return 1;
				} 
				else {
					fread((*RelTable)[i][j],sizeof(Elf32_Rel),1,fichier);
				}
						
			}
			nb_done++;	
		}	
	}
}	
		

//---------------------------------------------------------------------------------------------------------------------------------------------
nb_done=0;
for( i = 0 ; i < header->e_shnum ; i++ ) {
	if (SecHeader[i]->sh_type == SHT_RELA)
		nb_rela++;
}
//On alloue le nb de table rela trouvées
(*RelaTable)=(Elf32_Rela ***) malloc(sizeof(Elf32_Rela*)*nb_rel);
if(RelaTable == NULL) {
	free (*RelaTable);
	return 1;
}
//On cherche chaque table rela
for( i =0 ; i < header->e_shnum && nb_done<nb_rel ; i++ ) {
	if (SecHeader[i]->sh_type == SHT_RELA){
		//on a trouvee une table des rela
		fseek(fichier,SecHeader[i]->sh_offset,SEEK_SET);
		(*RelaTable)[i]=(Elf32_Rela **) malloc(SecHeader[i]->sh_size/sizeof(Elf32_Rela));	
		if ((*RelaTable)[i]==NULL){
			//si pas ok on free le pointeur et on envoie 1
			for (k=nb_done;k>=0;k--)
				free((*RelaTable)[k]);
				free(*RelaTable);
				return 1;
		} 
		else {
		//Sinon on continue
						
		//pour chaque symbole de cette table
			for (j=0; j<SecHeader[i]->sh_size/sizeof(Elf32_Rela); j++){
				(*RelaTable)[i][j]=(Elf32_Rela *) malloc(sizeof(Elf32_Rela));
				if ((*RelaTable)[i][j]==NULL){
					//si pas ok on free le pointeur et on envoie 1
					for(l=i;l>=0;l--){
						for (k=j;k>=0;k--)
							free((*RelTable)[l][k]);
						free((*RelTable)[l]);	
					}	
					free(*RelaTable);
					return 1;
				} 
				else {
					fread((*RelaTable)[i][j],sizeof(Elf32_Rela),1,fichier);
								
								
				}
						
			}
			nb_done++;	
		}	
	}
}	


return 1;
}
