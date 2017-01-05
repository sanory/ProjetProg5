#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <elf.h>
#include "read_header.h"
#include "disp_header.h"
#include "read_section_header.h"
#include "display_section.h"
#include "display_section_header.h"
#include "read_symb_table.h"
#include "disp_table_symbole.h"

void lire_header_fichier(FILE *fichierObjet, Elf32_Ehdr *structureHeaderFichier){
	int succesLecture;
	succesLecture=read_header(fichierObjet,structureHeaderFichier);
	if(succesLecture!=0){
		if(succesLecture==2){
			fprintf(stderr,"Fichier 64 bits non supportes");
		}
		else{
			fprintf(stderr,"Erreur lors de la lecture du header de fichier");
		}
		free(structureHeaderFichier);
		fclose(fichierObjet);
		exit(4);
	}
}

void lire_header_section(FILE *fichierObjet, Elf32_Ehdr* structureHeaderFichier, Elf32_Shdr*** structureHeaderSection){
	int succesLecture;
	succesLecture=read_section_header(fichierObjet,structureHeaderFichier,structureHeaderSection);
	if(succesLecture!=0){
		fprintf(stderr,"Erreur lors de la lecture du header de section : ");
		if(succesLecture==1){
			fprintf(stderr,"erreur d'allocation\n");
		free(structureHeaderFichier);
		fclose(fichierObjet);
		exit(6);
		}
		else if(succesLecture==2){
			fprintf(stderr,"absence de table de section\n");
		}
	}
}

void lire_table_symboles(Elf32_Shdr ** structureHeaderSection, Elf32_Ehdr * structureHeaderFichier,  FILE *fichierObjet, Elf32_Sym *** tableSymboles){
	int succesLecture;
	succesLecture=readSymbTable(structureHeaderSection,structureHeaderFichier,fichierObjet,tableSymboles);
	if(succesLecture!=0){
		fprintf(stderr,"erreur d'allocation\n");
		desalocSecTable(structureHeaderFichier,&structureHeaderSection);
		free(structureHeaderFichier);
		fclose(fichierObjet);
		exit(6);
	}
}

void lire_table_relocations(void){
	printf("banane\n");
}

FILE *ouverture_lecture_seule_avec_verif(char *nomFich){
	FILE *fich;
	fich=fopen(nomFich,"r");
	if(fich == NULL){
		fprintf(stderr,"Erreur lors de l'ouverture du fichier %s\n",nomFich);
		exit(3);
	}
	return fich;
}


void help(char* commande){
	printf("Aide de la commande %s. A construire\n\n",commande);
	printf("Options:\n   -H : option par défaut, affiche l'aide\n");
	printf("   -a [argument module c] : a construire\n");
	printf("   -h [argument] : affiche uniquement le header de fichier du fichier spécifié en argument\n");
	printf("   -S [argument] : affiche uniquement le header de section du fichier spécifié en argument\n");
	printf("   -x [argument] : affiche une section specifique\n");
	printf("   -s [argument] : affiche la table des symboles\n");
	printf("   -r [argument] : affiche le mot banane, wow on travaille dur a la fac\n");
}





int main(int argc, char* argv[]){
	int opt;
	//char *option1, *option2;

	
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


	//option1 = NULL;
	//option2 = NULL;

	if(argc==1){
		help(argv[0]);
		exit(2);
	}

	FILE *fichierObjet1=NULL;
	Elf32_Ehdr *structureHeaderFichier1=NULL;
	structureHeaderFichier1=malloc(sizeof(Elf32_Ehdr));
	if(structureHeaderFichier1==NULL){
		fprintf(stderr, "Erreur d'allocation de la structure ELF");
		exit(5);
	}
	Elf32_Shdr **structureHeaderSection1=NULL;
	Elf32_Sym ** tableSymboles1=NULL;

	while ((opt = getopt_long(argc, argv, "a:h:S:x:s:r:H", longopts, NULL)) != -1) {
		switch(opt) {
		case 'a':
			if(optind-1<=argc && argv[optind-1][0]!='-'){
			fputs(argv[optind-1],stdout);
			}
			if(optind<=argc && argv[optind][0]!='-'){
			fputs(argv[optind],stdout);
			}
			//option1 = optarg;
			break;
		case 'h':
			fichierObjet1=ouverture_lecture_seule_avec_verif(optarg);
			lire_header_fichier(fichierObjet1,structureHeaderFichier1);
			display(structureHeaderFichier1);
			fclose(fichierObjet1);
			break;
		case 'S':
			fichierObjet1=ouverture_lecture_seule_avec_verif(optarg);
			lire_header_fichier(fichierObjet1,structureHeaderFichier1);
			lire_header_section(fichierObjet1,structureHeaderFichier1,&structureHeaderSection1);
			display_section_header(structureHeaderSection1,structureHeaderFichier1,fichierObjet1); 
			fclose(fichierObjet1);
			break;
		case 'x':
			fichierObjet1=ouverture_lecture_seule_avec_verif(optarg);
			lire_header_fichier(fichierObjet1,structureHeaderFichier1);
			lire_header_section(fichierObjet1,structureHeaderFichier1,&structureHeaderSection1);
			display_section(fichierObjet1,6,structureHeaderSection1,structureHeaderFichier1);
			fclose(fichierObjet1);
			break;
		case 's':
			fichierObjet1=ouverture_lecture_seule_avec_verif(optarg);
			lire_header_fichier(fichierObjet1,structureHeaderFichier1);
			lire_header_section(fichierObjet1,structureHeaderFichier1,&structureHeaderSection1);
			lire_table_symboles(structureHeaderSection1,structureHeaderFichier1,fichierObjet1,&tableSymboles1);
			display_table_symb(tableSymboles1,structureHeaderSection1,structureHeaderFichier1);
			fclose(fichierObjet1);
			break;
		case 'r':
			fichierObjet1=ouverture_lecture_seule_avec_verif(optarg);
			lire_header_fichier(fichierObjet1,structureHeaderFichier1);
			lire_header_section(fichierObjet1,structureHeaderFichier1,&structureHeaderSection1);
			lire_table_relocations();
			
			fclose(fichierObjet1);
			break;
		case 'H':
			help(argv[0]);
			free(structureHeaderFichier1);
			exit(0);
		default :
			fprintf(stderr, "L'option -%c n'a pas été reconnue\n",optopt);
			free(structureHeaderFichier1);
			exit(1);
		}
	}
	
	if(tableSymboles1!=NULL){
		desalocSymbTable(structureHeaderFichier1,structureHeaderSection1,&tableSymboles1);
	}
	if(structureHeaderSection1!=NULL){
		desalocSecTable(structureHeaderFichier1,&structureHeaderSection1);
	}
	free(structureHeaderFichier1);


	return 0;
}
