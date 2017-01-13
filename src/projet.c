#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <elf.h>
#include "disp_header.h"
#include "display_section.h"
#include "display_section_header.h"
#include "disp_table_symbole.h"
#include "display_Rel_Sections.h"
#include "display_Rela_Sections.h"
#include "fusionelf.h"
#include "elfFile.h"

FILE *lire_et_remplir(char *nomFich, fichierElf *f) {
    FILE *fich;
    int succesLecture;
    fich = fopen(nomFich, "r");
    if (fich == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", nomFich);
        exit(3);
    }
    succesLecture = read_elfFile(fich, f);
    switch (succesLecture) {
        case 0:
            return fich;
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
            fprintf(stderr, "Salut\n");
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
        case 11:
            fprintf(stderr, "erreur lors de la copie des sections dans la structure");
            break;
        case 12:
            fprintf(stderr, "erreur lors la creation du pointeur de contenu d'une section");
            break;
        default:
            fprintf(stderr, "wtf");
            exit(42);
	    break;
    }
    exit(1);
    return fich;
}

void help(char* commande) {
    printf("Aide de la commande %s. A construire\n\n", commande);
    printf("Options:\n   -H : option par défaut, affiche l'aide\n");
    printf("   -a fichier : a construire\n");
    printf("   -h fichier : affiche uniquement le header de fichier du fichier spécifié en argument\n");
    printf("   -S fichier : affiche uniquement le header de section du fichier spécifié en argument\n");
    printf("   -x numeroDeSection/nomDeSection fichier : affiche une section specifique\n");
    printf("   -s fichier : affiche la table des symboles\n");
    printf("   -r fichier : affiche les tables de reallocation\n");
    printf("   -f fichier : fusionne les deux fichiers passés en argument\n");
}

int main(int argc, char* argv[]) {
    int opt;
    int i;
    int succesLecture;
    int succesFusion;

    struct option longopts[] = {
        { "afficher_tout", required_argument, NULL, 'a'},
        { "afficher_header_fichier", required_argument, NULL, 'h'},
        { "afficher_headers_sections", required_argument, NULL, 'S'},
        { "afficher_une_section", required_argument, NULL, 'x'},
        { "afficher_table_symboles", required_argument, NULL, 's'},
        { "afficher_table_relacations", required_argument, NULL, 'r'},
        { "fusionner_les_fichiers", required_argument, NULL, 'f'},
        { "help", no_argument, NULL, 'H'},
        { NULL, 0, NULL, 0}
    };

    if (argc == 1) {
        help(argv[0]);
        exit(2);
    }

    FILE *fichierObjet1 = NULL;
    FILE *fichierObjet2 = NULL;
    FILE *fichierObjetResultat = NULL;

    fichierElf monFichierElf1;
    fichierElf monFichierElf2;
    fichierElf monFichierElfResultat;

    if ((opt = getopt_long(argc, argv, "a:h:S:x:s:r:f:H", longopts, NULL)) != -1) {
        switch (opt) {
            case 'a':
                fichierObjet1 = lire_et_remplir(optarg, &monFichierElf1);
                display(&monFichierElf1);
                display_section_header(&monFichierElf1);
                display_table_symb(&monFichierElf1);
                for (i = 0; i < monFichierElf1.nbSections; i++)
                    display_section(i, &monFichierElf1);
                display_rel_section(&monFichierElf1);
                display_rela_section(&monFichierElf1);
                fclose(fichierObjet1);
                break;

            case 'h':
                fichierObjet1 = lire_et_remplir(optarg, &monFichierElf1);
                display(&monFichierElf1);
                fclose(fichierObjet1);
                break;

            case 'S':
                fichierObjet1 = lire_et_remplir(optarg, &monFichierElf1);
                display_section_header(&monFichierElf1);
                fclose(fichierObjet1);
                break;

            case 'x':
                //2 arguments nécessaires, on vérifie le nombre d'arguments
                if (optind - 1 < argc && argv[optind - 1][0] != '-' && optind < argc && argv[optind][0] != '-') {
                    fichierObjet1 = lire_et_remplir(argv[optind], &monFichierElf1);
                    i = 0;
                    while (optarg[i] <= '9' && optarg[i] >= '0') {
                        i++;
                    }
                    if (optarg[i] == '\0') {
                        i = (int) strtol(optarg, (char**) NULL, 10);
                        succesLecture = display_section(i, &monFichierElf1);
                        fclose(fichierObjet1);
                    } else {
                        succesLecture = display_section_nom(optarg, &monFichierElf1);
                        fclose(fichierObjet1);
                    }
                    
                    if (succesLecture == 2) {
                        fprintf(stderr, "La section specifiee n'existe pas\n");
                    }
                } else {
                    fprintf(stderr, "Pas assez d'arguments dans l'option %c. Se referer a l'aide (-H ou commande sans option)\n", opt);
                    exit(2);
                }
                break;

            case 's':
                fichierObjet1 = lire_et_remplir(optarg, &monFichierElf1);
                display_table_symb(&monFichierElf1);
                fclose(fichierObjet1);
                break;

            case 'r':
                fichierObjet1 = lire_et_remplir(optarg, &monFichierElf1);
                display_rel_section(&monFichierElf1);
                display_rela_section(&monFichierElf1);
                fclose(fichierObjet1);
                break;

            case 'f':
                if (optind - 1 < argc && argv[optind - 1][0] != '-' && optind < argc && argv[optind][0] != '-') {
                    fichierObjet1 = lire_et_remplir(optarg, &monFichierElf1);
                    fichierObjet2 = lire_et_remplir(argv[optind], &monFichierElf2);

                } else {
                    fprintf(stderr, "Pas assez d'arguments dans l'option %c. Se referer a l'aide (-H ou commande sans option)\n", opt);
                    exit(2);
                }
                succesFusion = fusion_elfFile(fichierObjetResultat, &monFichierElf1, &monFichierElf2, &monFichierElfResultat);
                fclose(fichierObjet1);
                fclose(fichierObjet2);
                //display(&monFichierElf1);
                //display(&monFichierElf2);
                //display(&monFichierElfResultat);
                display_section_header(&monFichierElfResultat);
                if (succesFusion == 0)printf("Youpi\n");
                break;

            case 'H':
                help(argv[0]);
                exit(0);

            default:
                fprintf(stderr, "L'option -%c n'a pas été reconnue\n", optopt);
                exit(1);
        }
    }

    //desallocation de la structures.
    //les tests pour savoit si il y a besoin de desallouer tel ou tel pointeurs 
    //sont dans la fonctions
    desaloc_elfFilsStruct(&monFichierElf1);

    return 0;
}
