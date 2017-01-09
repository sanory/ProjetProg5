
#include <stdlib.h>
#include "elfFile.h"
#include <string.h>

int read_elfFile(FILE* fichier, fichierElf * MonfichierElf) {
    int i, indcourant;

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
            || MonfichierElf->header.e_ident[EI_MAG1] != ELFMAG1
            || MonfichierElf->header.e_ident[EI_MAG2] != ELFMAG2
            || MonfichierElf->header.e_ident[EI_MAG3] != ELFMAG3)
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

    MonfichierElf->SectNames = malloc(MonfichierElf->nbSectNames);
    if (MonfichierElf->SectNames == NULL)
        return 1;

    fseek(MonfichierElf->fichier,
            MonfichierElf->secHeader[MonfichierElf->header.e_shstrndx].sh_offset,
            SEEK_SET);

    fread(MonfichierElf->SectNames, 1, MonfichierElf->nbSectNames,
            MonfichierElf->fichier);

    //--------------------------------------------------------------------------
    //creation de la table des noms de symboles
    i = 0;
    while (i < (MonfichierElf->nbSections) - 1 && strcmp(
            MonfichierElf->SectNames + MonfichierElf->secHeader[i].sh_name,
            ".strtab"))
        i++;

    MonfichierElf->nbSymbNames = MonfichierElf->secHeader[i].sh_size;

    MonfichierElf->SymbNames = malloc(MonfichierElf->nbSymbNames);
    if (MonfichierElf->SymbNames == NULL)
        return 10;

    fseek(MonfichierElf->fichier,
            MonfichierElf->secHeader[i].sh_offset, SEEK_SET);

    fread(MonfichierElf->SymbNames, 1, MonfichierElf->nbSymbNames,
            MonfichierElf->fichier);

    //--------------------------------------------------------------------------
    //creation de la table des symboles

    //ajout du nolmbre de symboles dans la structure
    for (i = 0; i < MonfichierElf->nbSections; i++)
        if (MonfichierElf->secHeader[i].sh_type == SHT_SYMTAB) {
            MonfichierElf->nbSymb =
                    MonfichierElf->secHeader[i].sh_size / sizeof (Elf32_Sym);
            //decallage dans le fichier pour aller a la bonne section
            fseek(MonfichierElf->fichier,
                    MonfichierElf->secHeader[i].sh_offset, SEEK_SET);
        }

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
    if (MonfichierElf->nbRelSection != 0) {
        if (MonfichierElf->RelSections == NULL)
            return 6;
    } else
        MonfichierElf->RelSections = NULL;

    //--------------------------------------------------------------------------
    //creation des sections de type SectionRela

    //ajout du nombre de section de type Rela dans la structure
    MonfichierElf->nbRelaSection = 0;
    for (i = 0; i < MonfichierElf->nbSections; i++)
        if (MonfichierElf->secHeader[i].sh_type == SHT_RELA)
            MonfichierElf->nbRelaSection++;

    //allocation de la table SectionRela
    MonfichierElf->RelaSections =
            malloc(MonfichierElf->nbRelaSection * sizeof (SectionRela));
    if (MonfichierElf->nbRelaSection != 0) {
        if (MonfichierElf->RelaSections == NULL)
            return 7;
    } else
        MonfichierElf->RelaSections = NULL;

    //--------------------------------------------------------------------------
    //chargement de SectionRel
    indcourant = 0;
    for (i = 0; i < MonfichierElf->nbSections; i++) {
        if (MonfichierElf->secHeader[i].sh_type == SHT_REL) {

            MonfichierElf->RelSections[indcourant].numSection = i;
            MonfichierElf->RelSections[indcourant].RelTableSize =
                    MonfichierElf->secHeader[i].sh_size / sizeof (Elf32_Rel);

            //allocation de la RelTable
            MonfichierElf->RelSections[indcourant].RelTable =
                    malloc(MonfichierElf->secHeader[i].sh_size);
            if (MonfichierElf->RelSections[indcourant].RelTableSize == 0)
                MonfichierElf->RelSections[indcourant].RelTable = NULL;
            else {
                if (MonfichierElf->RelSections[indcourant].RelTable == NULL) {
                    return 8;
                }
            }

            //deplacement au debut de la section
            fseek(fichier, MonfichierElf->secHeader[i].sh_offset, SEEK_SET);
            //lecture du contenu de la table
            fread(MonfichierElf->RelSections[indcourant].RelTable,
                    MonfichierElf->secHeader[i].sh_size, 1, fichier);
            indcourant++;
        }

    }

    //--------------------------------------------------------------------------
    //chargement de SectionRela
    indcourant = 0;
    for (i = 0; i < MonfichierElf->nbSections; i++) {
        if (MonfichierElf->secHeader[i].sh_type == SHT_RELA) {

            MonfichierElf->RelaSections[indcourant].numSection = i;
            MonfichierElf->RelaSections[indcourant].RelaTableSize =
                    MonfichierElf->secHeader[i].sh_size / sizeof (Elf32_Rela);

            //allocation de la RelaTable
            MonfichierElf->RelaSections[indcourant].RelaTable =
                    malloc(MonfichierElf->secHeader[i].sh_size);
            if (MonfichierElf->RelaSections[indcourant].RelaTableSize == 0)
                MonfichierElf->RelaSections[indcourant].RelaTable = NULL;
            else {
                if (MonfichierElf->RelaSections[indcourant].RelaTable == NULL) {
                    return 8;
                }
            }

            //deplacement au debut de la section
            fseek(fichier, MonfichierElf->secHeader[i].sh_offset, SEEK_SET);
            //lecture du contenu de la table
            fread(&(MonfichierElf->RelaSections[indcourant].RelaTable),
                    MonfichierElf->secHeader[i].sh_size, 1, fichier);
            indcourant++;
        }

    }
    return 0;
}

int desaloc_elfFilsStruct(fichierElf * MonfichierElf) {
    int i;

    //ne fait pas correctement le free a tester et a debeugeurer
    //desaloc rela
    if (MonfichierElf->RelaSections != NULL) {
        for (i = 0; i < MonfichierElf->RelaSections[i].RelaTableSize; i++) {
            if (MonfichierElf->RelaSections[i].RelaTable != NULL) {
                free(MonfichierElf->RelaSections[i].RelaTable);
            }
        }
    }
    free(MonfichierElf->RelaSections);


    //ne fait pas correctement le free a tester et a debeugeurer
    //--------------------------------------------------------------------------
    //desaloc rel
    if (MonfichierElf->RelSections != NULL) {
        for (i = 0; i < MonfichierElf->RelSections[i].RelTableSize; i++) {
            if (MonfichierElf->RelSections[i].RelTable != NULL) {
                free(MonfichierElf->RelSections[i].RelTable);
            }
        }
    }
    free(MonfichierElf->RelSections);

    //--------------------------------------------------------------------------
    //desaloc symtable
    if (MonfichierElf->symTable != NULL && MonfichierElf->nbSymb != 0) {
        free(MonfichierElf->symTable);
    }

    //--------------------------------------------------------------------------
    //desaloc de la table des noms de symboles
    if (MonfichierElf->SymbNames != NULL && MonfichierElf->nbSymbNames != 0) {
        free(MonfichierElf->SymbNames);
    }

    //--------------------------------------------------------------------------
    //desaloc de la table des noms de sections
    if (MonfichierElf->SectNames != NULL && MonfichierElf->nbSectNames != 0) {
        free(MonfichierElf->SectNames);
    }


    //--------------------------------------------------------------------------
    //desaloc de la table des section
    if (MonfichierElf->secHeader != NULL && MonfichierElf->nbSections != 0) {
        free(MonfichierElf->secHeader);
    }




    return 0;
}
