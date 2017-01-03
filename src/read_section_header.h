
int desalocSecTable(Elf32_Shdr **SecHeader);


int read_section_header(FILE *fichier, Elf32_Ehdr *header, Elf32_Shdr **SecHeader);
