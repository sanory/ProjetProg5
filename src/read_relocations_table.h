

int read_relocations_table(FILE *fichier, Elf32_Rel **** RelTable, Elf32_Rela **** RelaTable, Elf32_Shdr ** SecHeader, Elf32_Ehdr * header);

int desalocRelaTable(Elf32_Rela **** RelaTable, Elf32_Ehdr * header, Elf32_Shdr ** SecHeader);

int desalocRelTable(Elf32_Rel **** RelTable, Elf32_Ehdr * header, Elf32_Shdr ** SecHeader);
