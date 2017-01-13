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
	
test: 
	./testAff1.sh ../initports.o

superclean:
	@echo "Nettoyage !\n\n"
	rm -f *~
	rm -f *.o
	rm -f projet
	rm -f dist.tgz
	
dist:
	@echo "Archvage des sources\n\n"
	tar zcvf dist.tgz *.c *.h Makefile

