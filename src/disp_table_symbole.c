#include <stdio.h>
#include <elf.h>
#include "elfFile.h"
#include <string.h>

int display_table_symb(fichierElf *f) {
    int j, ok = 0;

    //CALCUL NOMBRE D'ENTREES

    printf("\nTable de symboles « .symtab » contient %i entrées:\n", f->nbSymb);

    printf("   Num:\tValeur\t\tTail\tType\tLien\tVis\tNdx\tNom\n");

    //POUR CHAQUE ENTREE

    //f->nbSymb = nombre d'entrées
    for (j = 0; j < f->nbSymb; j++) {

        //Affichage numéro de l'entrée
        printf("   %i:", j);

        //Affichage de la valeur du symbole
        printf("\t%08x", f->symTable[j].st_value);

        //Affichage de la taille
        printf("\t%i", f->symTable[j].st_size);

        //Affichage du type du symbole

        switch (f->symTable[j].st_other) {
            case 0:
                printf("\tNOTYPE");
                break;

            case 2:
                printf("\tFUNC");
                break;

            case 3:
                printf("\tSECTION");
                break;

            case 4:
                printf("\tFILE");
                break;

            default:
                printf("\t");
                break;
        }

        //Affichage de la liaison du symbole
        if (ELF32_ST_BIND(f->symTable[j].st_info) == 0) {
            printf("\tLOCAL");
        } else if (ELF32_ST_BIND(f->symTable[j].st_info) == 1)
            printf("\tGLOBAL");
        else
            printf("\t");

        //Affichage de la visibilité du symbole
        switch (f->symTable[j].st_other) {
            case STV_DEFAULT:
                printf("\tDEFAULT");
                break;

            case STV_INTERNAL:
                printf("\tINTERNAL");
                break;

            case STV_HIDDEN:
                printf("\tHIDDEN");
                break;

            case STV_PROTECTED:
                printf("\tPROTECTED");
                break;

            default:
                printf("\t");
                ok = 1;
        }

        //Affichage de l'indice de la section avec laquelle le symbole est en lien
        if (f->symTable[j].st_shndx == SHN_UNDEF) {
            printf("\tUND");
        } else if (f->symTable[j].st_shndx == 65521)
            printf("\tABS");
        else
            printf("\t%i", f->symTable[j].st_shndx);

        //Affichage du nom du symbole
        printf("\t%s", f->SymbNames + f->symTable[j].st_name);

        //Ligne suivante
        printf("\n");

    }

    return ok;
}