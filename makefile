CFLAGS=-g -Wall -Werror -Wconversion -Wtype-limits -pedantic -std=c99 -O0
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes
OUTPUT=abb

abb: abb.c pruebas.c
	gcc *.c $(CFLAGS) -o $(OUTPUT)

valgrind: abb
	valgrind $(VFLAGS) ./$(OUTPUT)

debug: abb
	gdb -tui $(OUTPUT)
