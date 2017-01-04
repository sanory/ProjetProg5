#include <elf.h>
#include <stdio.h>

int display_section_header(Elf32_Shdr **sh, Elf32_Ehdr *h,  FILE * fichier){
	int i;
	int ok=0;
	int addrfin;
	fseek(fichier,0,SEEK_END);
	addrfin = ftell(fichier);

	printf("il y a %d en-têtes de section, débutant à l'adresse de décalage %04x\n\n", h->e_shnum, addrfin-(h->e_shnum*h->e_shentsize) ); 
	printf("En-tête de section:");
	printf("[Nr]\tNom\tType\tAdr\tDécala.\tTaille\tES\tFan\tLN\tInf\tAl\n");
	for(i=0;i< h->e_shnum;i++){
		printf("[%d]\t",i);
		printf("fh[i]->sh_name\t"); //NOM
		switch (sh[i]->sh_type) { //TYPE
			
			case SHT_NULL :
		 	printf("NULL\t");
		 	break;

			case SHT_PROGBITS :
		 	printf("PROGBITS\t");
		 	break;
			
			case SHT_SYMTAB :
		 	printf("SYMTAB\t");
		 	break;

			case SHT_STRTAB :
		 	printf("STRTAB\t");
		 	break;
			
			case SHT_RELA :
		 	printf("RELA\t");
		 	break;

			case SHT_HASH :
		 	printf("HASH\t");
		 	break;
			
			case SHT_DYNAMIC :
		 	printf("DYNAMIC\t");
		 	break;

			case SHT_NOTE :
		 	printf("NOTE\t");
		 	break;
			
			case SHT_NOBITS :
		 	printf("NOBITS\t");
		 	break;

			case SHT_REL :
		 	printf("REL\t");
		 	break;
			
			case SHT_SHLIB :
		 	printf("SHLIB\t");
		 	break;

			case SHT_DYNSYM :
		 	printf("DYNSYM\t");
		 	break;

			case SHT_LOPROC :
		 	printf("LOPROC\t");
		 	break;

			case SHT_HIPROC :
		 	printf("HIPROC\t");
		 	break;

			case SHT_LOUSER :
		 	printf("LOUSER\t");
		 	break;

			case SHT_HIUSER :
		 	printf("HIUSER\t");
		 	break;

			default:
				ok=1;
		}
	
		printf("%08x\t",sh[i]->sh_addr); //Addr
		printf("%06x\t",sh[i]->sh_offset); //Decala.
		printf("%06x\t",sh[i]->sh_size); //taille
		printf("%02x\t",sh[i]->sh_entsize); //ES


		if(sh[i]->sh_flags==SHF_WRITE){ //FAN
			printf("W");
		}
		if(sh[i]->sh_flags==SHF_ALLOC){
			printf("A");
		}
		if(sh[i]->sh_flags==SHF_EXECINSTR){
			printf("X");
		}
		if(sh[i]->sh_flags==SHF_MASKPROC){
			printf("M");
		}
		printf("\t");
			

		printf("%d\t",sh[i]->sh_link);//LN
		printf("%d\t",sh[i]->sh_info);//INF
		printf("%d\t",sh[i]->sh_addralign);//AL

	}

return ok;

}
