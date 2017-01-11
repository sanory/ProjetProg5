#include <stdlib.h>
#include "elfFile.h"
#include <string.h>

int fusion_elfFile(FILE* fichier, fichierElf  *MonfichierElf1, fichierElf  *MonfichierElf2, fichierElf *MonfichierElfresultat){
int i,j;
int * T;
int arretBoucle = 0;
int nbFusions = 0;
int indiceCorrect;
Elf32_Off decalage; // décalage par rapport au fichier 1 et non pas au début du fichier
//init

//1)copie du header (à mettre à jour)

MonfichierElfresultat->nbSections = 0;
MonfichierElfresultat->nbSymb = 0;
//fichier à créer
MonfichierElfresultat->fichier=fichier;

MonfichierElfresultat->header=MonfichierElf1->header;

MonfichierElfresultat->nbSections = MonfichierElf1->nbSections+MonfichierElf2->nbSections;

//calcul du nouveau nombre de sections
for(i=0; i<MonfichierElf1->nbSections; i++){
	for(j=0; j<MonfichierElf2->nbSections; j++){
		if(MonfichierElf1->secHeader[i].sh_type == SHT_PROGBITS && MonfichierElf2->secHeader[j].sh_type == SHT_PROGBITS && !strcmp(MonfichierElf1->secHeader[i].sh_name+MonfichierElf1->SectNames,MonfichierElf2->secHeader[j].sh_name+MonfichierElf2->SectNames) ){
			MonfichierElfresultat->nbSections--;
		}
	}
}
/* pour les etapes suivantes
for(j=0; j<MonfichierElf2->nbSections; j++){
	if(MonfichierElf2->secHeader[j].sh_type != SHT_PROGBITS)
		MonfichierElfresultat->nbSections--;
}
*/

MonfichierElfresultat->header.e_shnum = MonfichierElfresultat->nbSections;


//(e_shoff (debut en tete de sections) n'aura pas la bonne valeur au début, recherche char* a la fin)
//je sais pas encore comment faire pour e_shstrndx (debut de la stringtable des noms d'en tete de section)


//2) Remplissage des informations des en tetes de sections fusionnées dans une structure +char * à fusionner

MonfichierElfresultat->secHeader =
            malloc(MonfichierElfresultat->nbSections * sizeof (Elf32_Shdr));
    if (MonfichierElfresultat->secHeader == NULL)
        return 2; //erreur dans la creation du pointeur

//allocation d'un tableau de bool
T=calloc(MonfichierElf2->nbSections ,sizeof (int));


for(i=0; i<MonfichierElf1->nbSections; i++){
	for(j=0; j<MonfichierElf2->nbSections && !(arretBoucle); j++){
		if(MonfichierElf1->secHeader[i].sh_type == SHT_PROGBITS && MonfichierElf2->secHeader[j].sh_type == SHT_PROGBITS && !strcmp(MonfichierElf1->secHeader[i].sh_name + MonfichierElf1->SectNames, MonfichierElf2->secHeader[j].sh_name+MonfichierElf2->SectNames)){	
			MonfichierElfresultat->secHeader[i].sh_name = MonfichierElf1->secHeader[i].sh_name;
			MonfichierElfresultat->secHeader[i].sh_type = MonfichierElf1->secHeader[i].sh_type;
			MonfichierElfresultat->secHeader[i].sh_flags = (MonfichierElf1->secHeader[i].sh_flags & MonfichierElf2->secHeader[j].sh_flags);
			MonfichierElfresultat->secHeader[i].sh_addr = MonfichierElf1->secHeader[i].sh_addr+decalage;
			MonfichierElfresultat->secHeader[i].sh_offset = MonfichierElf1->secHeader[i].sh_offset+decalage;
			MonfichierElfresultat->secHeader[i].sh_size = MonfichierElf1->secHeader[i].sh_size + MonfichierElf2->secHeader[j].sh_size;
			MonfichierElfresultat->secHeader[i].sh_link = MonfichierElf1->secHeader[i].sh_link;
			MonfichierElfresultat->secHeader[i].sh_info = MonfichierElf1->secHeader[i].sh_info;
			MonfichierElfresultat->secHeader[i].sh_addralign = MonfichierElf1->secHeader[i].sh_addralign;
			MonfichierElfresultat->secHeader[i].sh_entsize = MonfichierElf1->secHeader[i].sh_entsize;
			arretBoucle=1;
			printf("fusion %d\n",MonfichierElfresultat->secHeader[i].sh_type);
			//valeurs statiques de la section
			MonfichierElfresultat->ContenuSection[i].longueurSect=MonfichierElf1->ContenuSection[i].longueurSect+MonfichierElf2->ContenuSection[j].longueurSect;
			MonfichierElfresultat->ContenuSection[i].numSect=i;
			//concatenation des sections dans le char * 
			decalage = decalage + MonfichierElf2->secHeader[j].sh_size;
			T[j] = 1;
		}
	}
	//SI CA A PAS ETE FUSIONNE, INSERER LA SECTION ET SON EN TETE QUAND MEME, [ICI]
	if(!arretBoucle){
		MonfichierElfresultat->secHeader[i] = MonfichierElf1->secHeader[i];
		MonfichierElfresultat->secHeader[i].sh_addr = MonfichierElf1->secHeader[i].sh_addr+decalage;
		MonfichierElfresultat->secHeader[i].sh_offset = MonfichierElf1->secHeader[i].sh_offset+decalage;
		printf("assignement 1\n");
		//copie des sections dans le char * 
	}
	arretBoucle = 0;
}

for(i=0; i<MonfichierElf2->nbSections; i++){
	if(T[i]==1 /*|| MonfichierElf2->secHeader[i].sh_type != SHT_PROGBITS */ ){
		nbFusions++;
		

		
	}
	else{
		printf("assignement 2\n");
		indiceCorrect=i+MonfichierElf1->nbSections-nbFusions;
		MonfichierElfresultat->secHeader[indiceCorrect] = MonfichierElf2->secHeader[i];
		MonfichierElfresultat->secHeader[indiceCorrect].sh_addr = MonfichierElf2->secHeader[i].sh_addr+decalage;
		MonfichierElfresultat->secHeader[indiceCorrect].sh_offset = MonfichierElf2->secHeader[i].sh_offset+decalage;
		//copie des sections dans le char * 
	}
}









//penser à e_shoff apres avoir display les sections


//FUSION DE LA TABLE DES SYMBOLES (EN COURS)
/*
	//unsigned int nbSymb;
    //Elf32_Sym * symTable;

	//!\\vérifier qu'il n'y a pas de symboles globaux définis portants le même nom, si cela arrive retourne -1 : échec définitif de la fusion
	int i=0;
	int j;
	int nbSymbRes=0;//Calcul taille nouvelle table des symboles
	//Elf32_Sym * symTableResTmp=malloc(MonfichierElf1->symTable.sh_entsize + MonfichierElf2->symTable.sh_entsize);//alloue une taille trop grande
	Elf32_Sym * symTableResTmp=malloc(sizeof(Elf32_Sym)*(MonfichierElf2->nbSymb + MonfichierElf2->nbSymb));//alloue une taille trop grande

	//compte +traite les symboles locaux du fichier 1
	while(i<(MonfichierElf1->nbSymb)){
		if((MonfichierElf1->symTable[i].st_info)==0){//LOCAL
			nbSymbRes++;
			///
			///Ajouter le symbole
			///
		}
		i=i+1;
	}

	//compte +traite les symboles locaux du fichier 2
	while(i<(MonfichierElf2->nbSymb)){
		if((MonfichierElf2->symTable[i].st_info)==0){//LOCAL
			nbSymbRes++;
			///
			///Ajouter le symbole
			///
		}
		i=i+1;
	}

	////compte +traite les symboles globaux
	i=0;
	while(i<(MonfichierElf1->nbSymb)){
		j=0;
		while(((MonfichierElf1->symTable[j].st_info)!=0) && j<(MonfichierElf2->nbSymb)){//Seulement si symb fich 1 est global
			//chercher symbole de même nom
			if((MonfichierElf1->symbNames + f->symTable[i].st_name)==(MonfichierElf2->symbNames + f->symTable[j].st_name)){
				//un des deux défini?

				//les deux définis?

				//aucun défini?
				
			}else if(j=(MonfichierElf2->nbSymb)-1){//pas de symbole du même nom
				nbSymbRes++;
				///
				///Ajouter le symbole
				///
			}
			j=j+1;
		}
		i=i+1;
	}

	free(symTableResTmp);

*/

return 0;
}
