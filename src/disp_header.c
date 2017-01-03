#include <elf.h>
#include <stdio.h>
//stdlib


int display(Elf32_Ehdr *h){
	int ok=1;
	printf("En-tête ELF:\nMagique:\t");
	//NOMBRE MAGIQUE
		if((h->e_ident[EI_MAG0])==ELFMAG0) {	
	 	printf("%02x ",ELFMAG0);
	 	}else{
			ok=0;
		}

		if((h->e_ident[EI_MAG1])==ELFMAG1) {	
	 	printf("%02x ",ELFMAG1);
	 	}else{
			ok=0;
		}

		if((h->e_ident[EI_MAG2])==ELFMAG2) {	
	 	printf("%02x ",ELFMAG2);
	 	}else{
			ok=0;
		}

		if((h->e_ident[EI_MAG3])==ELFMAG3) {	
	 	printf("%02x ",ELFMAG3);
	 	}else{
			ok=0;
		}

		int i=4;
		while(0){
			i=i+1;

		}

/*
		switch (h->e_ident[EI_CLASS]) {
			
		case ELFCLASSNONE :
	 	/////
	 	break;

		case ELFCLASS32 :
	 	////
	 	break;

		case ELFCLASS64 :
	 	///
	 	break;

		default:
			ok=0;
		}


*/


	return ok;

}