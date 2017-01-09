#include <stdlib.h>
#include "elfFile.h"
#include <string.h>

int fusion_elfFile(FILE* fichier, fichierElf  MonfichierElf1, fichierElf  MonfichierElf2, fichierElf * MonfichierElfresultat){

MonfichierElfresultat->nbSections = 0;
MonfichierElfresultat->nbSymb = 0;
MonfichierElfresultat->fichier=fichier;

return 1;

}
