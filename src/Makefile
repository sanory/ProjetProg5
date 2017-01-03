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