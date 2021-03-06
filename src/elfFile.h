#ifndef ELFFILE_H
#define ELFFILE_H

#include <elf.h>
#include <stdio.h>

typedef struct {
    unsigned int numSection;
    unsigned int RelTableSize;
    Elf32_Rel * RelTable;
} SectionRel;

typedef struct {
    unsigned int numSection;
    unsigned int RelaTableSize;
    Elf32_Rela * RelaTable;
} SectionRela;

typedef struct {
    unsigned char * contenu; //contient la section a prorepment parler
    unsigned int longueurSect; //contient la longeur de la section
    unsigned int numSect; //contient le numero de la section
} ContenuSection;

typedef struct {
    //fichier
    FILE * fichier;
    //entete ELF du fichier
    Elf32_Ehdr header;
    //entete de sections
    unsigned int nbSections; //si 0 pas alloue
    Elf32_Shdr * secHeader;
    //contenu des sections
    ContenuSection * LesSections; //contient les sections
    //table des noms de sections
    unsigned int nbSectNames;
    char * SectNames;
    //table des noms des symboles .strtab
    unsigned int nbSymbNames;
    char * SymbNames;
    //tables des Symboles
    unsigned int nbSymb;
    Elf32_Sym * symTable; //si 0 pas alloue
    //table des Rel
    unsigned int nbRelSection; //si 0 pas alloue
    SectionRel * RelSections;
    //table des Rel
    unsigned int nbRelaSection; //si 0 pas alloue
    SectionRela * RelaSections;
} fichierElf;

/*
 * 0 tout est ok
 * 1 impossible d'allouer SectNames
 * 2 ce n'est pas un fichier ELF
 * 3 ce n'est pas un fichier ELF 32bits
 * 4 erreur allocation du pointeur de table des sections
 * 5 erreur allocation du pointeur de table de Symboles
 * 6 erreur allocation de la table RelSections
 * 7 erreur allocation de la table RelaSections
 * 8 erreur dans l'allocation de RelTable
 * 9 erreur dans l'allocation de la RelaTable
 * 10 erreur dans l'allocation de SymbNames
 * 11 erreur lors de la copie des sections dans la structure
 * 12 erreur lors de la creation du pointeur de contenu d'une section
 ----------------------------------------------------------------------
 * 13 erreur lors de la creation du nouveau secHeader
 * 14 erreur lors de la creation du nouveau pointeur de section
 * 15 erreur lors de la creation du nouveau pointeur de contenu d'une section
 */
int read_elfFile(FILE * fichier, fichierElf * monfichier);

int desaloc_elfFilsStruct(fichierElf * MonfichierElf);
#endif // ELFFILE_H

