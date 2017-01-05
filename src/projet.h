#ifndef ___PROJET___
#define ___PROJET___

/* lire_header_fonction mange des pâtes*/
void lire_header_fichier(FILE *fichierObjet, Elf32_Ehdr *structureHeaderFichier);

/* lire_header_section se sert de fonctions de "read_section_header" pour charger les en-têtes des sections du fichier [fichierObjet] dans la structure [structureHeaderSection] (allouée dans la fonction). Une structure [structureHeaderFichier] contenant l'en-tête du fichier est nécessaire*/
void lire_header_section(FILE *fichierObjet, Elf32_Ehdr* structureHeaderFichier, Elf32_Shdr **structureHeaderSection);


/*La fonction ouverture_lecture_seule_avec_verif s'occupe d'ouvrir le fichier de nom [nomFich] en lecture seule. Si le fichier a rencontré une erreur lors de son ouverture, elle affiche un message et s'occupe de quitter le programme*/
FILE *ouverture_lecture_seule_avec_verif(char *nomFich);

/* La fonction help affiche l'aide de la commande*/
void help(char* commande);





#endif
