#include <elf.h>
#include <stdio.h>
#include <stdlib.h>

int display_section_header(Elf32_Shdr **sh, Elf32_Ehdr *h,  FILE * fichier){
	int i=0;
	int ok=0;
	
		 char * SectNames = NULL;

	SectNames = malloc(sh[h->e_shstrndx]->sh_size);
	fseek(fichier, sh[h->e_shstrndx]->sh_offset, SEEK_SET);
  	fread(SectNames, 1, sh[h->e_shstrndx]->sh_size, fichier);


	printf("il y a %d en-tête de section, débutant à l'adresse de décalage %04x\n\n", h->e_shnum, h->e_shoff ); 
	printf("En-tête de section:\n");
	printf("[Nr]\t\t\tNom\t\t\tType\t\t\tAdr\t\tDécala.\tTaille\tES\tFan\tLN\tInf\tAl\n");
	for(i=0;i< h->e_shnum;i++){
		printf("[%d]\t\t\t",i);

		//printf("%d\t\t\t",sh[i]->sh_name); //NOM (h->e_shstrndx)<- section des chaine de caractères

		printf("%s\t\t\t",SectNames + sh[i]->sh_name);


		switch (sh[i]->sh_type) { //TYPE
			
			case SHT_NULL :
		 	printf("NULL\t\t\t");
		 	break;

			case SHT_PROGBITS :
		 	printf("PROGBITS\t\t");
		 	break;
			
			case SHT_SYMTAB :
		 	printf("SYMTAB\t\t\t");
		 	break;

			case SHT_STRTAB :
		 	printf("STRTAB\t\t\t");
		 	break;
			
			case SHT_RELA :
		 	printf("RELA\t\t\t");
		 	break;

			case SHT_HASH :
		 	printf("HASH\t\t\t");
		 	break;
			
			case SHT_DYNAMIC :
		 	printf("DYNAMIC\t\t\t");
		 	break;

			case SHT_NOTE :
		 	printf("NOTE\t\t\t");
		 	break;
			
			case SHT_NOBITS :
		 	printf("NOBITS\t\t\t");
		 	break;

			case SHT_REL :
		 	printf("REL\t\t\t");
		 	break;
			
			case SHT_SHLIB :
		 	printf("SHLIB\t\t\t");
		 	break;

			case SHT_DYNSYM :
		 	printf("DYNSYM\t\t\t");
		 	break;


			case SHT_LOPROC :
		 	printf("LOPROC\t\t\t");
		 	break;

			case SHT_HIPROC :
		 	printf("HIPROC\t\t\t");
		 	break;

			case SHT_LOUSER :
		 	printf("LOUSER\t\t\t");
		 	break;

			case SHT_HIUSER :
		 	printf("HIUSER\t\t\t");
		 	break;

			default:
			printf("unknowntype\t\t");
		}
	
		printf("%08x\t",sh[i]->sh_addr); //Addr
		printf("%06x\t",sh[i]->sh_offset); //Decala.
		printf("%06x\t",sh[i]->sh_size); //taille
		printf("%02x\t",sh[i]->sh_entsize); //ES

		
		if((sh[i]->sh_flags&0x1)==0x1){ //FAN
			printf("W");
		}
		if((sh[i]->sh_flags&0x2)==0x2){
			printf("A");
		}
		if((sh[i]->sh_flags&0x4)==0x4){
			printf("X");
		}
		if((sh[i]->sh_flags&0xf0000000)==0xf0000000){
			printf("M");
		}
		printf("\t");
			

		printf("%d\t",sh[i]->sh_link);//LN
		printf("%d\t",sh[i]->sh_info);//INF
		printf("%d\t\n",sh[i]->sh_addralign);//AL
		

	}

return ok;

}
