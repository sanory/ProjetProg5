#ifndef __FUSION_SYMBOLES_H__
#define __FUSION_SYMBOLES_H__

#include <elf.h>

int fusion_symboles(fichierElf  *MonfichierElf1, fichierElf  *MonfichierElf2, fichierElf *MonfichierElfresultat, int * deplacementSec, int indiceProchaineEnTete);

#endif