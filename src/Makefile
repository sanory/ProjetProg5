CC=gcc
CFLAGS=-ggdb3 -Wall -Werror

FILES=$(shell ls *.c)

projet: $(patsubst %.c, %.o, $(FILES))
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	@echo "Nettoyage !\n\n"
	rm -f *~
	rm -f *.o

superclean:
	@echo "Nettoyage !\n\n"
	rm -f *~
	rm -f *.o
	rm -f projet
	rm -f *.tgz
	
dist:
	@echo "Archvage des sources\n\n"
	tar zcvf groupe8.tgz *.c *.h Makefile testAff1.sh

