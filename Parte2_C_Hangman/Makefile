CC = gcc
CFLAGS = -Wall -Wextra
OBJS = main.o ahorcado.o
EJECUTABLE = hangman

$(EJECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EJECUTABLE) $(OBJS) -lncurses

main.o: main.c ahorcado.h
	$(CC) $(CFLAGS) -c main.c

ahorcado.o: ahorcado.c ahorcado.h
	$(CC) $(CFLAGS) -c ahorcado.c

clean:
	rm -f $(OBJS) $(EJECUTABLE)

run: $(EJECUTABLE)
	./$(EJECUTABLE)

.PHONY: clean run
