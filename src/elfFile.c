#include <stdio.h>
#include <stdlib.h>
#include "elfFile.h"

int read_elfFile(FILE* fichier, fichierElf * MonfichierElf) {
    int i, j;

    //lecture de l'entete de fichier
    //on suppose que on travaillera par la suite sur des fichiers ELF32bits
    //--------------------------------------------------------------------------
    //initialisation
    MonfichierElf->nbSections = 0;
    MonfichierElf->nbSymb = 0;

    //--------------------------------------------------------------------------
    //chargement depuis le fichier de l'entete de fichier
    fread(&(MonfichierElf->header), sizeof (Elf32_Ehdr), 1, fichier);

    //verification qu'il s'agit bien d'un fichier ELF
    if (MonfichierElf->header.e_ident[EI_MAG0] != ELFMAG0
            && MonfichierElf->header.e_ident[EI_MAG1] != ELFMAG1
            && MonfichierElf->header.e_ident[EI_MAG2] != ELFMAG2
            && MonfichierElf->header.e_ident[EI_MAG3] != ELFMAG3)
        return 2; //ce n'est ps un fichier ELF

    //verification qu'il sagit bien d'un fichier 32bit
    if (MonfichierElf->header.e_ident[EI_CLASS] != ELFCLASS32)
        return 3; //ce n'est pas un fichier 32bit

    //--------------------------------------------------------------------------
    //ajout du fichier dans la structure
    MonfichierElf->fichier = fichier;

    //--------------------------------------------------------------------------
    //creation de la table des section

    //ajout du nombre de sections dans la structure
    MonfichierElf->nbSections = MonfichierElf->header.e_shnum;

    //allocation de la table des sections
    MonfichierElf->secHeader =
            malloc(MonfichierElf->nbSections * sizeof (Elf32_Shdr));
    if (MonfichierElf->secHeader == NULL)
        return 4; //erreur dans la creation du pointeur

    //deplacement au debut des entete de sections
    fseek(fichier, MonfichierElf->header.e_shoff, SEEK_SET);

    //chargement de la table des sections
    for (i = 0; i < MonfichierElf->nbSections; i++)
        fread(&(MonfichierElf->secHeader[i]), sizeof (Elf32_Shdr), 1, fichier);

    //--------------------------------------------------------------------------
    //creation et ajout de la table des noms de sections
    MonfichierElf->nbSectNames =
            MonfichierElf->secHeader[MonfichierElf->header.e_shstrndx].sh_size;

    MonfichierElf->SectNames =
            malloc(MonfichierElf->nbSectNames);
    if (MonfichierElf->SectNames == NULL)
        return 1;

    fseek(MonfichierElf->fichier,
            MonfichierElf->secHeader[MonfichierElf->header.e_shstrndx].sh_offset,
            SEEK_SET);

    fread(MonfichierElf->SectNames, 1, MonfichierElf->nbSectNames,
            MonfichierElf->fichier);

    //--------------------------------------------------------------------------
    //creation de la table des symboles

    //ajout du nolmbre de symboles dans la structure
    for (i = 0; i < MonfichierElf->nbSections; i++)
        if (MonfichierElf->secHeader[i].sh_type == SHT_SYMTAB)
            MonfichierElf->nbSymb =
                MonfichierElf->secHeader[i].sh_size / sizeof (Elf32_Sym);

    //allocation de la table des symboles
    MonfichierElf->symTable = malloc(MonfichierElf->nbSymb * sizeof (Elf32_Sym));
    if (MonfichierElf->nbSymb == 0)
        MonfichierElf->symTable = NULL;
    else
        if (MonfichierElf->symTable == NULL)
        return 5; //table des symboles non alloue

    //chargement de la table de symboles
    for (i = 0; i < MonfichierElf->nbSymb; i++)
        fread(&(MonfichierElf->symTable[i]), sizeof (Elf32_Sym), 1, fichier);

    //--------------------------------------------------------------------------
    //creation des sections de type SectionRel

    //ajout du nombre de section de type Rel dans la structure
    MonfichierElf->nbRelSection = 0;
    for (i = 0; i < MonfichierElf->nbSections; i++)
        if (MonfichierElf->secHeader[i].sh_type == SHT_REL)
            MonfichierElf->nbRelSection++;

    //allocation de la table SectionRel
    MonfichierElf->RelSections =
            malloc(MonfichierElf->nbRelSection * sizeof (SectionRel));
    if (MonfichierElf->nbRelSection == 0)
        MonfichierElf->RelSections = NULL;
    else
        if (MonfichierElf->RelSections == NULL)
        return 6;

    //--------------------------------------------------------------------------
    //creation des sections de type SectionRela

    //ajout du nombre de section de type Rela dans la structure
    MonfichierElf->RelaSections = 0;
    for (i = 0; i < MonfichierElf->nbSections; i++)
        if (MonfichierElf->secHeader[i].sh_type == SHT_RELA)
            MonfichierElf->nbRelaSection++;

    //allocation de la table SectionRela
    MonfichierElf->RelaSections =
            malloc(MonfichierElf->nbRelaSection * sizeof (SectionRela));
    if (MonfichierElf->nbRelaSection == 0)
        MonfichierElf->RelaSections = NULL;
    else
        if (MonfichierElf->RelaSections == NULL)
        return 7;

    //--------------------------------------------------------------------------
    //chargement de SectionRel
    for (i = 0; i < MonfichierElf->nbSections; i++)
        if (MonfichierElf->secHeader[i].sh_type == SHT_REL) {
            MonfichierElf->RelSections[i].nbSection = i;
            MonfichierElf->RelSections[i].relTableSize =
                    MonfichierElf->secHeader[i].sh_size / sizeof (Elf32_Rel);
            //allocation de la RelTable
            MonfichierElf->RelSections[i].RelTable =
                    malloc(MonfichierElf->RelSections[i].relTableSize * sizeof (Elf32_Rel));
            if (MonfichierElf->RelSections[i].relTableSize == 0)
                MonfichierElf->RelSections[i].RelTable = NULL;
            else
                if (MonfichierElf->RelSections[i].RelTable == NULL) {
                return 8;
            }
            //deplacement au debut de la section
            fseek(fichier, MonfichierElf->secHeader[i].sh_offset, SEEK_SET);
            //lecture du contenu de la table
            for (j = 0; j < MonfichierElf->RelSections[i].relTableSize; j++)
                fread(&(MonfichierElf->RelSections[i].RelTable[j]),
                    sizeof (Elf32_Rel), 1, fichier);
        }

    //--------------------------------------------------------------------------
    //chargepment de SectionRela
    for (i = 0; i < MonfichierElf->nbSections; i++)
        if (MonfichierElf->secHeader[i].sh_type == SHT_RELA) {
            MonfichierElf->RelaSections[i].nbSection = i;
            MonfichierElf->RelaSections[i].RelaTableSize =
                    MonfichierElf->secHeader[i].sh_size / sizeof (Elf32_Rela);
            //allocation de la RelaTable
            MonfichierElf->RelaSections[i].RelaTable =
                    malloc(MonfichierElf->RelaSections[i].RelaTableSize * sizeof (Elf32_Rela));
            if (MonfichierElf->RelaSections[i].RelaTableSize == 0)
                MonfichierElf->RelaSections[i].RelaTable = NULL;
            else
                if (MonfichierElf->RelaSections[i].RelaTable == NULL) {
                return 9;
            }
            //deplacement au debut de la section
            fseek(fichier, MonfichierElf->secHeader[i].sh_offset, SEEK_SET);
            //lecture du contenu de la table
            for (j = 0; j < MonfichierElf->RelaSections[i].RelaTableSize; j++)
                fread(&(MonfichierElf->RelaSections[i].RelaTable[j]),
                    sizeof (Elf32_Rela), 1, fichier);

        }
    return 0;
}

int desaloc_desaloc_elfFilsStruct(fichierElf * MonfichierElf) {


    return 1;
}