#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elfFile.h"

int display_section_header(fichierElf * elfFile) {
    int i;

    printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage %04x\n\n",
            elfFile->nbSections, elfFile->header.e_shoff);
    printf("En-têtes de section:\n");
    printf("[Nr]\tNom\t\t\tType\t\tAdr\t\tDécala.\tTaille\tES\tFan\tLN\tInf\tAl\n");
    for (i = 0; i < elfFile->nbSections; i++) {
        printf("[%d]\t", i);

        if (strlen((elfFile->SectNames + elfFile->secHeader[i].sh_name)) >= 16) {
            printf("%s\t", elfFile->SectNames + elfFile->secHeader[i].sh_name);
        } else if (strlen(elfFile->SectNames + elfFile->secHeader[i].sh_name) >= 8)
            printf("%s\t\t", elfFile->SectNames + elfFile->secHeader[i].sh_name);
        else
            printf("%s\t\t\t", elfFile->SectNames + elfFile->secHeader[i].sh_name);

        switch (elfFile->secHeader[i].sh_type) { //TYPE

            case SHT_NULL:
                printf("NULL\t\t");
                break;

            case SHT_PROGBITS:
                printf("PROGBITS\t");
                break;

            case SHT_SYMTAB:
                printf("SYMTAB\t\t");
                break;

            case SHT_STRTAB:
                printf("STRTAB\t\t");
                break;

            case SHT_RELA:
                printf("RELA\t\t");
                break;

            case SHT_HASH:
                printf("HASH\t\t");
                break;

            case SHT_DYNAMIC:
                printf("DYNAMIC\t\t");
                break;

            case SHT_NOTE:
                printf("NOTE\t\t");
                break;

            case SHT_NOBITS:
                printf("NOBITS\t\t");
                break;

            case SHT_REL:
                printf("REL\t\t");
                break;

            case SHT_SHLIB:
                printf("SHLIB\t\t");
                break;

            case SHT_DYNSYM:
                printf("DYNSYM\t\t");
                break;


            case SHT_LOPROC:
                printf("LOPROC\t\t");
                break;

            case SHT_HIPROC:
                printf("HIPROC\t\t");
                break;

            case SHT_LOUSER:
                printf("LOUSER\t\t");
                break;

            case SHT_HIUSER:
                printf("HIUSER\t\t");
                break;

            case SHT_ARM_ATTRIBUTES:
                printf("ARM_ATTRIBUTES\t");
                break;

            default:
                printf("unknowntype\t");
        }

        printf("%08x\t", elfFile->secHeader[i].sh_addr); //Addr
        printf("%06x\t", elfFile->secHeader[i].sh_offset); //Decala.
        printf("%06x\t", elfFile->secHeader[i].sh_size); //taille
        printf("%02x\t", elfFile->secHeader[i].sh_entsize); //ES

        if ((elfFile->secHeader[i].sh_flags & 0x1) == 0x1) { //FAN
            printf("W");
        }
        if ((elfFile->secHeader[i].sh_flags & 0x2) == 0x2) {
            printf("A");
        }
        if ((elfFile->secHeader[i].sh_flags & 0x4) == 0x4) {
            printf("X");
        }
        if ((elfFile->secHeader[i].sh_flags & 0xf0000000) == 0xf0000000) {
            printf("M");
        }
        printf("\t");

        printf("%d\t", elfFile->secHeader[i].sh_link); //LN
        printf("%d\t", elfFile->secHeader[i].sh_info); //INF
        printf("%d\t\n", elfFile->secHeader[i].sh_addralign); //AL

    }

    return 0;

}
