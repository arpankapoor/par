CC = gcc
CFLAGS = -Wall -std=c11

par: main.o
	$(CC) $(CFLAGS) -o $@ $^

install: par
	mv $^ /usr/local/bin

uninstall:
	rm -f /usr/local/bin/par

clean:
	rm -f par
