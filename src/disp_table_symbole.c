#include <stdio.h>
#include <elf.h>
#include "elfFile.h"

int display_table_symb(fichierElf *f){
//display_table_symb(FILE* fichier,Elf32_Sym **s, Elf32_Shdr ** SecHeader, Elf32_Ehdr *h){
	int ok=0;
	
	//CALCUL NOMBRE D'ENTREES

	printf("\nTable de symboles « .symtab » contient %i entrées:\n",f->nbSymb);

	printf("   Num:\tValeur\t\tTail\tType\tLien\tVis\tNdx\tNom\n");

	//POUR CHAQUE ENTREE

	int j=0;
	while(j<f->nbSymb){		//f->nbSymb = nombre d'entrées

		//Affichage numéro de l'entrée
		printf("   %i:",j);

		//Affichage de la valeur du symbole
		printf("\t%08x",f->symTable[j].st_value);

		//Affichage de la taille
		printf("\t%i",f->symTable[j].st_size);

		//Affichage du type du symbole
		if(ELF32_ST_TYPE(f->symTable[j].st_info)==0){
			printf("\tNOTYPE");
		/*}else if(ELF32_ST_TYPE(s[j]->st_info)==1){
			printf("\t???");*/
		}else if(ELF32_ST_TYPE(f->symTable[j].st_info)==2){
			printf("\tFUNC");
		}else if(ELF32_ST_TYPE(f->symTable[j].st_info)==3){
			printf("\tSECTION");
		}else if(ELF32_ST_TYPE(f->symTable[j].st_info)==4){
			printf("\tFILE");
		}else{
			printf("\t");
		}

		//printf("\t%i",ELF32_ST_TYPE(f->symTable[j].st_info));
		/*switch(f->symTable[j].st_info){

			case STT_NOTYPE:
				printf("\tNOTYPE");
			break;
			
			case STT_OBJECT:
				printf("\tOBJECT");
			break;

			case STT_FUNC:
				printf("\tFUNC");
			break;

			case STT_SECTION:
				printf("\tSECTION");
			break;

			case STT_FILE:
				printf("\tFILE");
			break;

			case STT_LOPROC:
				printf("\t%i",(ELF32_ST_TYPE(f->symTable[j].st_info)));/////////réservé au processeur je ne sais pas à quoi cela correspond//et c'est un int d'après le compilateur
			break;

			case STT_HIPROC:
				printf("\t%i",(ELF32_ST_TYPE(f->symTable[j].st_info)));/////////réservé au processeur je ne sais pas à quoi cela correspond//et c'est un int d'après le compilateur
			break;

			default:
				printf("\t");
				printf("default");
				ok=1;
		}*/
		//Affichage de la liaison du symbole
		if(ELF32_ST_BIND(f->symTable[j].st_info)==0){
			printf("\tLOCAL");
		}else if(ELF32_ST_BIND(f->symTable[j].st_info)==1){
			printf("\tGLOBAL");
		}else{
			printf("\t");
		}
		
		

		//Affichage de la visibilité du symbole
		switch(f->symTable[j].st_other){
			case STV_DEFAULT:
				printf("\tDEFAULT");
			break;

			case STV_INTERNAL:
				printf("\tINTERNAL");
			break;

			case STV_HIDDEN:
				printf("\tHIDDEN");
			break;

			case STV_PROTECTED:
				printf("\tPROTECTED");
			break;

			default:
				printf("\t");
				ok=1;
		}

		//Affichage de l'indice de la section avec laquelle le symbole est en lien
		if(f->symTable[j].st_shndx==SHN_UNDEF){
			printf("\tUND");
		}else if(f->symTable[j].st_shndx==65521){
			printf("\tABS");
		}else{
			printf("\t%i",f->symTable[j].st_shndx);
		}
		

		//Affichage du nom du symbole
		printf("\t%i",f->symTable[j].st_name);

		/*char * SectNames = NULL;
		SectNames = malloc(sh[h->e_shstrndx]->sh_size);


		fseek(fichier, sh[h->e_shstrndx]->sh_offset, SEEK_SET);
		fread(SectNames, 1, sh[h->e_shstrndx]->sh_size, fichier);
    	printf("%s\t\t\t",SectNames + sh[i]->sh_name);
		*/


		//Ligne suivante
		printf("\n");
		j=j+1;
	}



	return ok;
}