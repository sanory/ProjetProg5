#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <elf.h>
#include "disp_header.h"
#include "display_section.h"
#include "display_section_header.h"
#include "disp_table_symbole.h"

#include "elfFile.h"

FILE *ouverture_lecture_seule_avec_verif(char *nomFich){
	FILE *fich;
	fich=fopen(nomFich,"r");
	if(fich == NULL){
		fprintf(stderr,"Erreur lors de l'ouverture du fichier %s\n",nomFich);
		exit(3);
	}
	return fich;
}

void lire_et_remplir(char *nomFich, fichierElf *f){
	FILE *fich;
	int succesLecture;
	fich=fopen(nomFich,"r");
	if(fich == NULL){
		fprintf(stderr,"Erreur lors de l'ouverture du fichier %s\n",nomFich);
		exit(3);
	}
	succesLecture=read_elfFile(fich,f);
	switch(succesLecture) {
	case 0:
		break;
	case 1:
		fprintf(stderr, "Erreur. Impossible d'allouer SectNames\n");
		break;
	case 2:
		fprintf(stderr, "Erreur. Ce n'est pas un fichier ELF\n");
		break;
	case 3:
		fprintf(stderr, "Errreur. Ce n'est pas un fichier 32bits\n");
		break;
	case 4:
		fprintf(stderr, "Erreur allocation du pointeur de table des Sections\n");
		break;
	case 5:
		fprintf(stderr, "Erreur allocation du pointeur de table de Symboles\n");
		break;
	case 6:
		fprintf(stderr, "Erreur allocation de la table RelSections\n");
		break;
	case 7:
		fprintf(stderr, "Erreur allocation de la table RelaSections\n");
		break;
	case 8:
		fprintf(stderr, "Erreur dans l'allocation de RelTable\n");
		break;
	case 9:
		fprintf(stderr, "Erreur dans l'allocation de la RelaTable\n");
		break;
	case 10:
		fprintf(stderr, "Erreur dans l'allocation de SymbNames\n");
		break;
	default :
		fprintf(stderr, "wtf");
		exit(42);
	}
}

void help(char* commande){
	printf("Aide de la commande %s. A construire\n\n",commande);
	printf("Options:\n   -H : option par défaut, affiche l'aide\n");
	printf("   -a fichier : a construire\n");
	printf("   -h fichier : affiche uniquement le header de fichier du fichier spécifié en argument\n");
	printf("   -S fichier : affiche uniquement le header de section du fichier spécifié en argument\n");
	printf("   -x numeroDeSection/nomDeSection fichier : affiche une section specifique\n");
	printf("   -s fichier : affiche la table des symboles\n");
	printf("   -r fichier : affiche les tables de reallocation\n");
}





int main(int argc, char* argv[]){
	int opt;
	int i;
	int succesLecture;
	//char *argumentTemporaire;

	
	struct option longopts[] = {
		{ "afficher_tout", required_argument, NULL, 'a' },
		{ "afficher_header_fichier", required_argument, NULL, 'h' },
		{ "afficher_headers_sections", required_argument, NULL, 'S' },
		{ "afficher_une_section", required_argument, NULL, 'x' },
		{ "afficher_table_symboles", required_argument, NULL, 's' },
		{ "afficher_table_relacations", required_argument, NULL, 'r' },
		{ "help", no_argument, NULL, 'H' },
		{ NULL, 0, NULL, 0 }
	};





	if(argc==1){
		help(argv[0]);
		exit(2);
	}

	FILE *fichierObjet1=NULL;
        
        fichierElf monfichier;

	while ((opt = getopt_long(argc, argv, "a:h:S:x:s:r:H", longopts, NULL)) != -1) {
		switch(opt) {
		case 'a':
			printf("toto");
			//option1 = optarg;
                        fichierObjet1=ouverture_lecture_seule_avec_verif(optarg);
                        printf("\n\n\n\n%d\n",read_elfFile(fichierObjet1,&monfichier));
			display(&monfichier);
			display_section_header(&monfichier);
			display_table_symb(&monfichier);
			fclose(fichierObjet1);
			break;
		case 'h':
			lire_et_remplir(optarg,&monfichier);
			display(&monfichier);
			fclose(fichierObjet1);
			break;
		case 'S':
			fichierObjet1=ouverture_lecture_seule_avec_verif(optarg);
			read_elfFile(fichierObjet1,&monfichier);
			display_section_header(&monfichier);
			fclose(fichierObjet1);
			break;
		case 'x':
			//2 arguments nécessaires, on vérifie le nombre d'arguments
			if(optind-1<argc && argv[optind-1][0]!='-' && optind<argc && argv[optind][0]!='-'){
				fichierObjet1=ouverture_lecture_seule_avec_verif(argv[optind]);
				read_elfFile(fichierObjet1,&monfichier);
			
				i=0;
				while(optarg[i]<='9' && optarg[i]>='0'){
					i++;
				}
				if(optarg[i]=='\0'){
					i=(int)strtol(optarg,(char**)NULL,10);
					succesLecture=display_section(i,&monfichier);
					fclose(fichierObjet1);
				}
				else{
					succesLecture=display_section_nom(optarg,&monfichier);
					fclose(fichierObjet1);
				}
			
				if(succesLecture==1){
					fprintf(stderr,"un truc chelou avec SHT_NOBITS\n");
				}
				else if(succesLecture==2){
					fprintf(stderr,"La section specifiee n'existe pas\n");
				}
			}
			else{
				fprintf(stderr, "Pas assez d'arguments dans l'option %c. Se referer a l'aide (-H ou commande sans option)\n",opt);
				exit(2);
			}
			break;
		case 's':
			fichierObjet1=ouverture_lecture_seule_avec_verif(optarg);
			read_elfFile(fichierObjet1,&monfichier);
			display_table_symb(&monfichier);
			fclose(fichierObjet1);
			break;
		case 'r':
			fichierObjet1=ouverture_lecture_seule_avec_verif(optarg);
			read_elfFile(fichierObjet1,&monfichier);
			fclose(fichierObjet1);
			break;
		case 'H':
			help(argv[0]);
			exit(0);
		default :
			fprintf(stderr, "L'option -%c n'a pas été reconnue\n",optopt);
			exit(1);
		}
	}
	
	/*if(tableSymboles1!=NULL){
		//desalocSymbTable(structureHeaderFichier1,structureHeaderSection1,&tableSymboles1);
	}
	if(structureHeaderSection1!=NULL){
		desalocSecTable(structureHeaderFichier1,&structureHeaderSection1);
	}
	free(structureHeaderFichier1);*/



	return 0;
}
