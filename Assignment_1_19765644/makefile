CC = gcc
CFLAGS =  -Wall -ansi -pedantic -g
OBJ = main.o game.o randomFunc.o terminal.o
EXEC = snake

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

main.o : main.c game.h randomFunc.h terminal.h
	$(CC) $(CFLAGS) main.c -c

ifeq ($(UNBEATABLE),1)
game.o : game_special.c game.h
	$(CC) $(CFLAGS) game_special.c -c
else
game.o : game.c game.h
	$(CC) $(CFLAGS) game.c -c
endif

	
	

randomFunc.o : randomFunc.c randomFunc.h
	$(CC) $(CFLAGS) randomFunc.c -c

terminal.o : terminal.c terminal.h
	$(CC) $(CFLAGS) terminal.c -c

clean :
	rm -f $(EXEC) $(OBJ)