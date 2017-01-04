#include <elf.h>
#include <stdio.h>


int display(Elf32_Ehdr *h){
	int ok=0;
	printf("En-tête ELF:\nMagique:\t");
				//E_ident//
	//NOMBRE MAGIQUE
		if((h->e_ident[EI_MAG0])==ELFMAG0) {	
	 	printf("%02x ",ELFMAG0);
	 	}else{
			ok=1;
		}

		if((h->e_ident[EI_MAG1])==ELFMAG1) {	
	 	printf("%02x ",ELFMAG1);
	 	}else{
			ok=1;
		}

		if((h->e_ident[EI_MAG2])==ELFMAG2) {	
	 	printf("%02x ",ELFMAG2);
	 	}else{
			ok=1;
		}

		if((h->e_ident[EI_MAG3])==ELFMAG3) {	
	 	printf("%02x ",ELFMAG3);
	 	}else{
			ok=1;
		}

		int i=4;
		while(i<15){
			printf("%02x ",(h->e_ident[i]));
			i=i+1;
		}
		printf("%02x\n",(h->e_ident[i]));
		//parcours des autres octets


	//CLASSE
		printf("Classe:\t\t");
		switch (h->e_ident[EI_CLASS]) {
			
		case ELFCLASSNONE :
	 	printf("None\n");
	 	break;

		case ELFCLASS32 :
	 	printf("ELF32\n");
	 	break;

		case ELFCLASS64 :
	 	printf("ELF64\n");
	 	break;

		default:
			ok=1;
		}

	//DONNEES
		printf("Données:\t");
		switch (h->e_ident[EI_DATA]) {
			
		case ELFDATANONE :
	 	printf("None\n");
	 	break;

		case ELFDATA2LSB :
	 	printf("complément à 2, système à octets de poids faible d'abord (little endian)\n");
	 	break;

		case ELFDATA2MSB :
	 	printf("complément à 2, système à octets de poids fort d'abord (big endian)\n");
	 	break;

		default:
			ok=1;
		}

	//VERSION
		printf("Version:\t");
		switch (h->e_ident[EI_VERSION]) {
			
		case EV_NONE :
	 	printf("None\n");
	 	break;

		case EV_CURRENT :
	 	printf("1 (current)\n");
	 	break;

		default:
			ok=1;
		}

	//OS/ABI:
		printf("OS/ABI:\t\t");
		switch (h->e_ident[EI_OSABI]) {
			
		/*case ELFOSABI_NONE :
		printf("ABI UNIX System V\n");
	 	break;*/

		case ELFOSABI_SYSV :
	 	printf("ABI UNIX System V\n");
	 	break;

	 	case ELFOSABI_HPUX :
	 	printf("ABI HP-UX\n");
	 	break;

	 	case ELFOSABI_NETBSD :
	 	printf("ABI NetBSD\n");
	 	break;

	 	case ELFOSABI_LINUX :
	 	printf("ABI Linux\n");
	 	break;

	 	case ELFOSABI_SOLARIS :
	 	printf("ABI Solaris\n");
	 	break;

	 	case ELFOSABI_IRIX :
	 	printf("ABI IRIX\n");
	 	break;

	 	case ELFOSABI_FREEBSD :
	 	printf("ABI FreeBSD\n");
	 	break;

	 	case ELFOSABI_TRU64 :
	 	printf("ABI UNIX TRU64\n");
	 	break;

		case ELFOSABI_ARM :
	 	printf("ABI de l'architecture ARM\n");
	 	break;

		case ELFOSABI_STANDALONE :
	 	printf("ABI autonome (embarqué)\n");
	 	break;

		default:
			ok=1;
		}



	//VERSION ABI:
		printf("Version ABI:\t%i\n",(h->e_ident[8]));

				//E_TYPE//
	//TYPE
		printf("Type:\t\t");
		switch (h->e_type) {
			
		case ET_NONE :
	 	printf("Type inconnu\n");
	 	break;

		case ET_REL :
	 	printf("Fichier repositionnable\n");
	 	break;

		case ET_EXEC :
	 	printf("Fichier exécutable\n");
	 	break;

	 	case ET_DYN :
	 	printf("Objet partagé\n");
	 	break;

	 	case ET_CORE :
	 	printf("Fichier core\n");
	 	break;

		default:
			ok=1;
		}

				//E_MACHINE//
	//MACHINE
		printf("Machine:\t");
		switch (h->e_machine) {

		case EM_NONE :
	 	printf("Machine inconnue\n");
	 	break;

		case EM_M32 :
	 	printf("AT&T WE 32100\n");
	 	break;

		case EM_SPARC :
	 	printf("Sun Microsystems SPARC\n");
	 	break;

		case EM_386 :
	 	printf("Intel 80386\n");
	 	break;

		case EM_68K :
	 	printf("Motorola 68000\n");
	 	break;

		case EM_88K :
	 	printf("Motorola 88000\n");
	 	break;

		case EM_860 :
	 	printf("Intel 80860\n");
	 	break;

		case EM_MIPS :
	 	printf("MIPS RS3000 (uniquement gros-boutiste)\n");
	 	break;

		case EM_PARISC :
	 	printf("HP/PA\n");
	 	break;

		case EM_SPARC32PLUS :
	 	printf("SPARC avec jeu d'instructions étendu\n");
	 	break;

		case EM_PPC :
	 	printf("PowerPC\n");
	 	break;

		case EM_PPC64 :
	 	printf("PowerPC 64 bits\n");
	 	break;

	 	case EM_S390 :
	 	printf("IBM S/390\n");
	 	break;

	 	case EM_ARM :
	 	printf("Machines de technologie RISC avancées\n");
	 	break;

	 	case EM_SH :
	 	printf("Renesas SuperH\n");
	 	break;

	 	case EM_SPARCV9 :
	 	printf("SPARC v9 64 bits\n");
	 	break;

	 	case EM_IA_64 :
	 	printf("Intel Itanium\n");
	 	break;

	 	case EM_X86_64 :
	 	printf("AMD x86-64\n");
	 	break;

	 	case EM_VAX :
	 	printf("DEC Vax\n");
	 	break;

		default:
			ok=1;
		}


		//E_VERSION//
	//VERSION
		printf("Version:\t");
		switch (h->e_version) {

		case EV_NONE :
	 	printf("0x%i\n",h->e_version);//Version non valable
	 	break;

		case EV_CURRENT :
	 	printf("0x%i\n",h->e_version);//Version actuelle
	 	break;

		default:
			ok=1;
		}

		//E_ENTRY//
	//ADRESSE DU POINT D'ENTREE
		printf("Adresse du point d'entrée:\t\t\t\t0x%i\n",h->e_entry);
		
		//E_PHOFF//
	//DEBUT DES EN-TETES DE PROGRAMME
		printf("Début des en-têtes de programme:\t\t\t%i (octets dans le fichier)\n",h->e_phoff);

		//E_SHOFF//
	//DEBUT DES EN-TETES DE SECTION
		printf("Début des en-têtes de section:\t\t\t\t%i (octets dans le fichier)\n",h->e_shoff);

		//E_FLAGS//
	//FANIONS
		if(h->e_flags!=0){
			printf("Fanions:\t\t\t\t\t\t0x%04x, Version5 EABI\n",h->e_flags);
		}else{
			printf("Fanions:\t\t\t\t\t\t0x0\n");
		}

		//E_EHSIZE//
	//TAILLE DE L'EN-TETE
		printf("Taille de cet en-tête:\t\t\t\t\t%i (bytes)\n",h->e_ehsize);

		//E_PHENTSIZE x E_PHNUM//
	//TAILLE DE L'EN-TETE DU PROGRAMME
		//multiplication de la taille d'une entréé par le nombre d'entrées
		//en 32bits on arrive jamais au cas phnum>=PN_XNUM(0xffff) qui utilise sh_info
			printf("Taille de l'en-tête du programme:\t\t\t%i (bytes)\n",(h->e_phentsize)*(h->e_phnum));

		
		//E_PHNUM//
	//NOMBRE D'EN-TETE DU PROGRAMME
			printf("Nombre d\'en-tête du programme:\t\t\t\t%i\n",h->e_phnum);


		//E_SHENTSIZE//
	//TAILLE DES EN-TETES DE SECTION
			printf("Taille des en-têtes de section:\t\t\t\t%i (bytes)\n",h->e_shentsize);

		//E_SHNUM//
	//NOMBRE D'EN-TETES DES SECTION
			printf("Nombre d'en-têtes de section:\t\t\t\t%i\n",h->e_shnum);


		//E_SHSTRNDX//
	//TABLE D'INDEXES DES CHAINES D'EN-TETE DE SECTION
			printf("Table d'indexes des chaînes d'en-tête de section:\t%i\n",h->e_shstrndx);


	/*	switch (h->e_ident[EI_CLASS]) {
			
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
			ok=1;
		}

	*/


	return ok;

}