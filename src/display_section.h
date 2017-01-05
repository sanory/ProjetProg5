
//reste a creeer la conversion nom de section
/*
* 0 ok
* 1 erreur le type de la section est SHT_NOBITS
* 2 erreur le numero ou le nom de la section est invalide
*/
int display_section(FILE* fichier, int nbSections, Elf32_Shdr ** SecHeader, Elf32_Ehdr *header);
int display_section_nom(FILE* fichier, char * nom, Elf32_Shdr ** SecHeader, Elf32_Ehdr *header);
