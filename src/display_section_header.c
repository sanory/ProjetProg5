#include <elf.h>
#include <stdio.h>

int display_section_header(Elf32_Shdr **sh, Elf32_Ehdr *h,  FILE * fichier){
	int i;
	int ok=0;

	printf("il y a %d en-têtes de section, débutant à l'adresse de décalage %04x\n\n", h->e_shnum, h->e_shoff ); 
	printf("En-tête de section:\n");
	printf("[Nr]\t\tNom\t\tType\t\tAdr\t\tDécala.\t\tTaille\t\tES\t\tFan\t\tLN\t\tInf\t\tAl\n");
	for(i=0;i< h->e_shnum;i++){
		printf("[%d]\t\t",i);
		printf("%d\t\t",sh[i]->sh_name); //NOM (section contenant les noms : .shstrtab)
		switch (sh[i]->sh_type) { //TYPE
			
			case SHT_NULL :
		 	printf("NULL\t\t");
		 	break;

			case SHT_PROGBITS :
		 	printf("PROGBITS\t");
		 	break;
			
			case SHT_SYMTAB :
		 	printf("SYMTAB\t\t");
		 	break;

			case SHT_STRTAB :
		 	printf("STRTAB\t\t");
		 	break;
			
			case SHT_RELA :
		 	printf("RELA\t\t");
		 	break;

			case SHT_HASH :
		 	printf("HASH\t\t");
		 	break;
			
			case SHT_DYNAMIC :
		 	printf("DYNAMIC\t\t");
		 	break;

			case SHT_NOTE :
		 	printf("NOTE\t\t");
		 	break;
			
			case SHT_NOBITS :
		 	printf("NOBITS\t\t");
		 	break;

			case SHT_REL :
		 	printf("REL\t\t");
		 	break;
			
			case SHT_SHLIB :
		 	printf("SHLIB\t\t");
		 	break;

			case SHT_DYNSYM :
		 	printf("DYNSYM\t\t");
		 	break;


			case SHT_LOPROC :
		 	printf("LOPROC\t\t");
		 	break;

			case SHT_HIPROC :
		 	printf("HIPROC\t\t");
		 	break;

			case SHT_LOUSER :
		 	printf("LOUSER\t\t");
		 	break;

			case SHT_HIUSER :
		 	printf("HIUSER\t\t");
		 	break;

			default:
				printf("pb\t\t");
		}
	
		printf("%08x\t",sh[i]->sh_addr); //Addr
		printf("%06x\t\t",sh[i]->sh_offset); //Decala.
		printf("%06x\t\t",sh[i]->sh_size); //taille
		printf("%02x\t\t",sh[i]->sh_entsize); //ES

		
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
		printf("\t\t");
			

		printf("%d\t\t",sh[i]->sh_link);//LN
		printf("%d\t\t",sh[i]->sh_info);//INF
		printf("%d\t\t\n",sh[i]->sh_addralign);//AL
		

	}

return ok;

}
