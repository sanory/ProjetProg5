#ifndef ELFFILE_H
#define ELFFILE_H

#include <elf.h>
#include <stdio.h>

typedef struct {
    int nbSection;
    int relTableSize;
    Elf32_Rel * RelTable;
} SectionRel;

typedef struct {
    int nbSection;
    int RelaTableSize;
    Elf32_Rela * RelaTable;
} SectionRela;

typedef struct {
    //fichier
    FILE * fichier;
    //entete ELF du fichier
    Elf32_Ehdr header;
    //entete de sections
    int nbSections; //si 0 pas alloue
    Elf32_Shdr * secHeader;
    //table des noms de sections
    int nbSectNames;
    char * SectNames;
    //tables des Symboles
    int nbSymb;
    Elf32_Sym * symTable; //si 0 pas alloue
    //table des Rel
    int nbRelSection; //si 0 pas alloue
    SectionRel * RelSections;
    //table des Rel
    int nbRelaSection; //si 0 pas alloue
    SectionRela * RelaSections;
} fichierElf;

/*
 * 0 tout est ok
 * 1 impossible d'allouer SectNames
 * 2 ce n'est pas un fichier ELF
 * 3 ce n'est pas un fichier ELF 32bits
 * 4 erreur allocation du pointeur de table des ections
 * 5 erreur allocation du pointeur de table de Symboles
 * 6 erreur allocation de la table RelSections
 * 7 erreur allocation de la table RelaSections
 * 8 erreur dans l'allocation de RelTable
 * 9 erreur dans l'allocation de la RelaTable
 */
int read_elfFile(FILE * fichier, fichierElf * monfichier);
#endif // ELFFILE_H

