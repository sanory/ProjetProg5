
/*
 * 0 ok
 * 1 pas ok
 */
int desalocSymbTable(Elf32_Ehdr * header, Elf32_Shdr **SecHeader, Elf32_Sym *** SymbTab);


/*
 * 0 ok
 * 1 pas ok erreur avec les pointeurs
 * 2 pas de sections de symboles trouvee
 */
int readSymbTable(Elf32_Shdr ** SecHeader,Elf32_Ehdr * header, FILE *fichier,Elf32_Sym *** Symbtab);
