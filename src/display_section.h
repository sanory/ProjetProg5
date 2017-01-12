#ifndef ___DISP_SECTION___
#define ___DISP_SECTION___
#include "elfFile.h"
//reste a creeer la conversion nom de section
/*
 * 0 ok
 * 1 erreur le type de la section est SHT_NOBITS
 * 2 erreur le numero ou le nom de la section est invalide
 * 3 impossible de vidanger la section
 */
int display_section(int nbSections, fichierElf * elfFile);
int display_section_nom(char * nom, fichierElf * elfFile);

#endif
