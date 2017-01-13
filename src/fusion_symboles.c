#include <stdlib.h>
#include "elfFile.h"
#include <string.h>

//FUSION DE LA TABLE DES SYMBOLES (EN COURS)
int fusion_symboles(fichierElf  *MonfichierElf1, fichierElf  *MonfichierElf2, fichierElf *MonfichierElfresultat, int * deplacementSec, int indiceProchainEnTete){
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
				!=(MonfichierElf2->SymbNames + MonfichierElf2->symTable[i].st_name)){
				//pas de symbole du même nom
				///Ajouter le symbole
				symTableResTmp[nbSymbRes]=(MonfichierElf2->symTable[i]);
				///Ajouter nom du symbole
				symNamesResTmp[nbSymbRes]=(MonfichierElf2->SymbNames[i]);
				nbSymbNamesRes=nbSymbNamesRes+(sizeof(MonfichierElf2->SymbNames[i]));
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
	//emplacement de table des symboles et table des noms des symboles dans le fichier1
	i=0;//emplacement de table des symboles
	j=0;//emplacement des noms des symboles
    while (strcmp(MonfichierElf1->SectNames + MonfichierElf1->secHeader[i].sh_name,".symtab")||
            strcmp(MonfichierElf1->SectNames + MonfichierElf1->secHeader[j].sh_name,".strtab")){
    	if(strcmp(MonfichierElf1->SectNames + MonfichierElf1->secHeader[i].sh_name,".symtab")){i++;}
    	if(strcmp(MonfichierElf1->SectNames + MonfichierElf1->secHeader[j].sh_name,".strtab")){j++;}
    }
    free(origin_fich2);


		MonfichierElfresultat->secHeader[indiceProchainEnTete].sh_name = MonfichierElf1->secHeader[i].sh_name;
		MonfichierElfresultat->secHeader[indiceProchainEnTete+1].sh_name = MonfichierElf1->secHeader[j].sh_name;

		MonfichierElfresultat->secHeader[indiceProchainEnTete].sh_type = MonfichierElf1->secHeader[i].sh_type;
		MonfichierElfresultat->secHeader[indiceProchainEnTete+1].sh_type = MonfichierElf1->secHeader[j].sh_type;

		MonfichierElfresultat->secHeader[indiceProchainEnTete].sh_flags = MonfichierElf1->secHeader[i].sh_flags;
		MonfichierElfresultat->secHeader[indiceProchainEnTete+1].sh_flags = MonfichierElf1->secHeader[j].sh_flags;

		MonfichierElfresultat->secHeader[indiceProchainEnTete].sh_addr = 0;
		MonfichierElfresultat->secHeader[indiceProchainEnTete+1].sh_addr = 0;

		//MonfichierElfresultat->secHeader[indiceProchainEnTete].sh_offset = ;
		//MonfichierElfresultat->secHeader[indiceProchainEnTete+1].sh_offset = ;

		MonfichierElfresultat->secHeader[indiceProchainEnTete].sh_size = sizeof(MonfichierElfresultat->symTable);
		MonfichierElfresultat->secHeader[indiceProchainEnTete+1].sh_size = MonfichierElfresultat->nbSymbNames;

		MonfichierElfresultat->secHeader[indiceProchainEnTete].sh_link = MonfichierElf1->secHeader[i].sh_link;
		MonfichierElfresultat->secHeader[indiceProchainEnTete+1].sh_link = MonfichierElf1->secHeader[j].sh_link;

		MonfichierElfresultat->secHeader[indiceProchainEnTete].sh_info = MonfichierElf1->secHeader[i].sh_info;
		MonfichierElfresultat->secHeader[indiceProchainEnTete+1].sh_info = MonfichierElf1->secHeader[j].sh_info;

		MonfichierElfresultat->secHeader[indiceProchainEnTete].sh_addralign = MonfichierElf1->secHeader[i].sh_addralign;
		MonfichierElfresultat->secHeader[indiceProchainEnTete+1].sh_addralign = MonfichierElf1->secHeader[j].sh_addralign;

		MonfichierElfresultat->secHeader[indiceProchainEnTete].sh_entsize = MonfichierElf1->secHeader[i].sh_entsize;
		MonfichierElfresultat->secHeader[indiceProchainEnTete+1].sh_entsize = MonfichierElf1->secHeader[j].sh_entsize;
		
		
	//valeurs statiques de la section
		MonfichierElfresultat->LesSections[indiceProchainEnTete].longueurSect=MonfichierElfresultat->secHeader[indiceProchainEnTete].sh_entsize;
		MonfichierElfresultat->LesSections[indiceProchainEnTete+1].longueurSect=MonfichierElfresultat->secHeader[indiceProchainEnTete+1].sh_entsize;

		MonfichierElfresultat->LesSections[indiceProchainEnTete].numSect=indiceProchainEnTete;
		MonfichierElfresultat->LesSections[indiceProchainEnTete+1].numSect=indiceProchainEnTete+1;

	//alloctaion de la section 
		MonfichierElfresultat->LesSections[indiceProchainEnTete].contenu = malloc(MonfichierElfresultat->secHeader[indiceProchainEnTete].sh_entsize);
		MonfichierElfresultat->LesSections[indiceProchainEnTete+1].contenu = malloc(MonfichierElfresultat->secHeader[indiceProchainEnTete+1].sh_entsize);

		
		memcpy(MonfichierElfresultat->LesSections[indiceProchainEnTete].contenu,
		MonfichierElfresultat->symTable,
		MonfichierElfresultat->LesSections[i].longueurSect);

		memcpy(MonfichierElfresultat->LesSections[indiceProchainEnTete+1].contenu,
		MonfichierElfresultat->SymbNames,
		MonfichierElfresultat->LesSections[j].longueurSect);
		

return 0;
}