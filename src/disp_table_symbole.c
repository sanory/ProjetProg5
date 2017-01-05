#include <stdio.h>
#include <elf.h>


int display_table(Elf32_Sym **s, Elf32_Shdr ** SecHeader, Elf32_Ehdr *h){
	int ok=0;
	
	//CALCUL NOMBRE D'ENTREES
	int i=0;
	while(i < h->e_shnum) {
    	if (SecHeader[i]->sh_type == SHT_SYMTAB){
      		//on a trouvee une table des symboles
    	}else{
    		i=i+1;
    	}
	}
	int nombre_entree=SecHeader[i]->sh_size/sizeof(Elf32_Sym);

	printf("\nTable de symboles « .symtab » contient %i entrées:",nombre_entree);

	printf("\tNum:\tValeur\tTail\tType\tLien\tVis\tNdx\tNom\n");

	//POUR CHAQUE ENTREE

	int j;
	while(j<nombre_entree){
		//Affichage numéro de l'entrée
		printf("\t%i:",j);

		//Affichage de la valeur du symbole
		printf("\t%08x",s[j]->st_value);

		//Affichage de la taille
		printf("\t%i",s[j]->st_size);

		//Affichage du type du symbole
		switch(s[j]->st_info){

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
				printf("\t%i",(ELF32_ST_TYPE(s[j]->st_info)));/////////réservé au processeur je ne sais pas à quoi cela correspond//et c'est un int d'après le compilateur
			break;

			case STT_HIPROC:
				printf("\t%i",(ELF32_ST_TYPE(s[j]->st_info)));/////////réservé au processeur je ne sais pas à quoi cela correspond//et c'est un int d'après le compilateur
			break;

			default:
				printf("\t");
				ok=1;
		}
		//Affichage de la liaison du symbole
		printf("\t%i",ELF32_ST_BIND(s[j]->st_info));//c'est un int d'après le compilateur
		

		//Affichage de la visibilité du symbole
		switch(s[j]->st_other){
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
		printf("\t%i",s[j]->st_shndx);//c'est un int d'après le compilateur

		//Affichage du nom du symbole
		printf("\t");



		//Ligne suivante
		printf("\n");
		j=j+1;
	}



	return ok;
}