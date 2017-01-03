#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <elf.h>
#include "read_header.h"


void help(char* commande){
	printf("Aide de la commande %s. A construire\n\n",commande);
	printf("Options:\n   -h : option par défaut, affiche l'aide\n   -a [argument] : affiche le header du fichier spécifié en argument\n");
}

int main(int argc, char* argv[]){
	int opt;
	//char *option1, *option2;
	char *afficher_header;

	
	struct option longopts[] = {
		{ "afficher_header", required_argument, NULL, 'a' },
		{ "option1", required_argument, NULL, '1' },
		{ "option2", required_argument, NULL, '2' },
		{ "help", no_argument, NULL, 'h' },
		{ NULL, 0, NULL, 0 }
	};


	//option1 = NULL;
	//option2 = NULL;
	afficher_header = NULL;

	if(argc==1){
		help(argv[0]);
		exit(2);
	}

	FILE *objet1;
	Elf32_Ehdr *structObjet1;
	structObjet1=malloc(sizeof(Elf32_Ehdr));

	while ((opt = getopt_long(argc, argv, "1:2:a:h", longopts, NULL)) != -1) {
		switch(opt) {
		case '1':
			printf("toto");
			//option1 = optarg;
			break;
		case '2':
			//option2 = optarg;
			printf("toto");
			break;
		case 'h':
			help(argv[0]);
			free(structObjet1);
			exit(0);
		case 'a':
			afficher_header = optarg;
			objet1=fopen(afficher_header,"r");
			read_header(objet1,structObjet1);
			free(structObjet1);
			fclose(objet1);
			break;
		default:
			fprintf(stderr, "Unrecognized option %c\n", opt);
			free(structObjet1);
			exit(1);
		}
	}
	
	

	return 0;
}
