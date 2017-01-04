

/*
Permet de charger l'entete de fichier ELF
0 tout est ok
1 erreur
2 fichier 64 bits
*/
int read_header(FILE *fichier, Elf32_Ehdr *header);


