#include <stdlib.h>
#include "elfFile.h"
#include <string.h>

//FUSION DE LA TABLE DES SYMBOLES (EN COURS)
int fusion_symboles(fichierElf  *MonfichierElf1, fichierElf  *MonfichierElf2, fichierElf *MonfichierElfresultat, int * deplacementSec, int indiceProchaineEnTete){
//!\\vérifier qu'il n'y a pas de symboles globaux définis portants le même nom, si cela arrive retourne -1 : échec définitif de la fusion

	int i=0, j;
	int *origin_fich2=malloc(sizeof(Elf32_Sym)*(MonfichierElf2->nbSymb + MonfichierElf2->nbSymb)); //indice = indice du symbole dans la table résultat, 
																							//si originaire du fichier 2 vaut 1, sinon 0
	
	//Table des symboles
	int nbSymbRes=0;//Calcul nb symboles dans nouvelle table des symboles
	Elf32_Sym * symTableResTmp=malloc(sizeof(Elf32_Sym)*(MonfichierElf2->nbSymb + MonfichierElf2->nbSymb));//alloue une taille trop grande
	//Table des noms de symboles
	int nbSymbNamesRes=0;//Calcul taille nouvelle table des noms des symboles
	char * symNamesResTmp=malloc(sizeof(char)*(MonfichierElf2->nbSymbNames + MonfichierElf2->nbSymbNames));//alloue une taille trop grande

	//compte +traite les symboles locaux du fichier 1
	while(i<(MonfichierElf1->nbSymb)){
		if((MonfichierElf1->symTable[i].st_info)==0){//LOCAL
			///
			///Ajouter le symbole
			symTableResTmp[nbSymbRes]=MonfichierElf1->symTable[i];
			///Ajouter nom du symbole
			symNamesResTmp[nbSymbRes]=MonfichierElf1->SymbNames[i];
			nbSymbNamesRes=nbSymbNamesRes+(sizeof(MonfichierElf1->SymbNames[i]));
			origin_fich2[nbSymbRes]=0;
			///
			nbSymbRes++;
		}
		i=i+1;
	}

	//compte +traite les symboles locaux du fichier 2
	while(i<(MonfichierElf2->nbSymb)){
		if((MonfichierElf2->symTable[i].st_info)==0){//LOCAL
			///
			///Ajouter le symbole
			symTableResTmp[nbSymbRes]=MonfichierElf2->symTable[i];
			///Ajouter nom du symbole
			symNamesResTmp[nbSymbRes]=MonfichierElf2->SymbNames[i];
			nbSymbNamesRes=nbSymbNamesRes+(sizeof(MonfichierElf2->SymbNames[i]));
			origin_fich2[nbSymbRes]=1;
			///
			nbSymbRes++;
		}
		i=i+1;
	}

	////compte +traite les symboles globaux
	i=0;
	while(i<(MonfichierElf1->nbSymb)){
		if(MonfichierElf1->symTable[j].st_info!=0){//Seulement si symb fich 1 est global
			j=0;
			while(j<(MonfichierElf2->nbSymb)){
				//chercher symbole de même nom
					if((MonfichierElf1->SymbNames + MonfichierElf1->symTable[i].st_name)==(MonfichierElf2->SymbNames + MonfichierElf2->symTable[j].st_name)){
						if((MonfichierElf1->symTable[j].st_shndx!=SHN_UNDEF) && (MonfichierElf2->symTable[j].st_shndx!=SHN_UNDEF)){
							//les deux sont définis
							free(symTableResTmp);
							free(symNamesResTmp);
							free(origin_fich2);
							printf("ERREUR fusion table des symboles");
							return -1;//ERREUR
							///
						}else if((MonfichierElf1->symTable[i].st_shndx==SHN_UNDEF) || (MonfichierElf2->symTable[j].st_shndx==SHN_UNDEF)){
							//un des deux est défini
							if(MonfichierElf1->symTable[i].st_shndx==SHN_UNDEF){
								///
								///Ajouter le symbole
								symTableResTmp[nbSymbRes]=MonfichierElf1->symTable[i];
								///Ajouter nom du symbole
								symNamesResTmp[nbSymbRes]=MonfichierElf1->SymbNames[i];
								nbSymbNamesRes=nbSymbNamesRes+(sizeof(MonfichierElf1->SymbNames[i]));
								origin_fich2[nbSymbRes]=0;
								///
								nbSymbRes++;
							}
							if(MonfichierElf2->symTable[j].st_shndx==SHN_UNDEF){
								///
								///Ajouter le symbole
								symTableResTmp[nbSymbRes]=MonfichierElf2->symTable[j];
								///Ajouter nom du symbole
								symNamesResTmp[nbSymbRes]=MonfichierElf2->SymbNames[j];
								nbSymbNamesRes=nbSymbNamesRes+(sizeof(MonfichierElf2->SymbNames[j]));
								origin_fich2[nbSymbRes]=1;
								///
								nbSymbRes++;
							}
						}else{
							///aucun défini -> ajoute l'un des deux (ici celui du fichier 1)
							///
							///Ajouter le symbole
							symTableResTmp[nbSymbRes]=MonfichierElf1->symTable[i];
							///Ajouter nom du symbole
							symNamesResTmp[nbSymbRes]=MonfichierElf1->SymbNames[i];
							nbSymbNamesRes=nbSymbNamesRes+(sizeof(MonfichierElf1->SymbNames[i]));
							origin_fich2[nbSymbRes]=0;
							///
							nbSymbRes++;
						}
						
					}else if(j==(MonfichierElf2->nbSymb)-1){
						//pas de symbole du même nom
						///
						///Ajouter le symbole
						symTableResTmp[nbSymbRes]=MonfichierElf1->symTable[i];
						///Ajouter nom du symbole
						symNamesResTmp[nbSymbRes]=MonfichierElf1->SymbNames[i];
						nbSymbNamesRes=nbSymbNamesRes+(sizeof(MonfichierElf1->SymbNames[i]));
						origin_fich2[nbSymbRes]=0;
						///
						nbSymbRes++;
					}
				j=j+1;
			}
		}
		i=i+1;
	}

	
	//recherche symboles globaux n'apparaisant que dans le fichier 2
	i=0;
	while(i<(MonfichierElf2->nbSymb)){
		j=0;
		while(j<(MonfichierElf1->nbSymb)){
			if(j==(MonfichierElf1->nbSymb)-1 
				&& (MonfichierElf1->SymbNames + MonfichierElf1->symTable[j].st_name)
				!=(MonfichierElf2->SymbNames + MonfichierElf2->symTable[i].st_name)
				&& MonfichierElf1->symTable[j].st_info!=0){//Seulement si symb fich 1 est global
				//pas de symbole du même nom
				///Ajouter le symbole
				symTableResTmp[nbSymbRes]=MonfichierElf1->symTable[i];
				///Ajouter nom du symbole
				symNamesResTmp[nbSymbRes]=MonfichierElf1->SymbNames[i];
				nbSymbNamesRes=nbSymbNamesRes+(sizeof(MonfichierElf1->SymbNames[i]));
				origin_fich2[nbSymbRes]=1;
				///
				nbSymbRes++;
			}
			j=j+1;
		}
		i=i+1;
	}


	MonfichierElfresultat->nbSymb=nbSymbRes;
	MonfichierElfresultat->symTable=realloc(symTableResTmp,sizeof(Elf32_Sym)*nbSymbRes);
	free(symTableResTmp);
	MonfichierElfresultat->nbSymbNames=nbSymbNamesRes;
	MonfichierElfresultat->SymbNames=realloc(symNamesResTmp,sizeof(Elf32_Sym)*nbSymbNamesRes);
	free(symNamesResTmp);


	//Mise à jour st_name et st_shndx
	int k=0;//parcours de table symboles et du tableau origin_fich2
	while(k>MonfichierElfresultat->nbSymb){
		MonfichierElfresultat->symTable[k].st_name=k;//indice du nom
		if(origin_fich2[k]==1){
			//alors il faut changer st_shndx
			MonfichierElfresultat->symTable[k].st_shndx=deplacementSec[MonfichierElfresultat->symTable[k].st_shndx];
		}
		k++;
	}

	//Mise à jour secHeader et ContenuSection
	//indiceProchaineEnTete
	/*MonfichierElfresultat->secHeader[i].sh_name = MonfichierElf1->secHeader[i].sh_name;
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
	MonfichierElfresultat->LesSections[i].longueurSect=MonfichierElf1->LesSections[i].longueurSect+MonfichierElf2->LesSections[j].longueurSect;
	MonfichierElfresultat->LesSections[i].numSect=i;
	//alloctaion de la section 
	MonfichierElfresultat->LesSections[i].contenu= malloc(MonfichierElfresultat->LesSections[i].longueurSect);
	if(MonfichierElfresultat->LesSections[i].contenu==NULL)
		return 15;
	memcpy(MonfichierElfresultat->LesSections[i].contenu,MonfichierElf1->LesSections[i].contenu,MonfichierElf1->LesSections[i].longueurSect);
	memcpy(MonfichierElfresultat->LesSections[i].contenu+MonfichierElf1->LesSections[i].longueurSect,MonfichierElf2->LesSections[j].contenu,MonfichierElf2->LesSections[j].longueurSect);
	*/


	free(origin_fich2);

return 0;
}