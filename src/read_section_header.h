/*
code erreur: 0-> fonctionnement normal   		desalloc SecHeader à réaliser avant celle de header
			 1-> erreur d'allocation     		desalloc déjà effectuée fin de programme
			 2-> absence de table de section 	pas de désalloc à réaliser pas de fin du programme
*/

int desalocSecTable(Elf32_Shdr **SecHeader);

int read_section_header(FILE *fichier, Elf32_Ehdr *header, Elf32_Shdr **SecHeader);

