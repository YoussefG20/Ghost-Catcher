SRC = main.c room.c ghost.c hunter.c house.c logger.c utils.c init.c
OBJ = $(SRC:.c=.o)
LIBS = -lpthread
CFLAGS = -g -Wall -Wextra

all: main

fa: $(OBJ)
	gcc -o fa $(OBJ) $(LIBS) $(CFLAGS)

main: $(OBJ)
	gcc -o main $(OBJ) $(LIBS) $(CFLAGS)

%.o: %.c
	gcc -c -o $@ $< $(CFLAGS)


finalproject: main
	valgrind --leak-check=full ./main


clean:
	rm -f main fa $(OBJ)
