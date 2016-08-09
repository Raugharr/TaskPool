CC=gcc
NAME=TaskPool
OBJFLD=obj/
FLAGS=-Wall -std=c99
DEBUG_FLAGS=$(FLAGS) -DDEBUG -ggdb 
RELEASE_FLAGS=$(FLAGS) -DNDEBUG -O3
COMPILE_FLAGS=$(DEBUG_FLAGS)
MAIN=main.c
DEPS=TaskPool.h MemoryPool.h
OBJS=$(OBJFLD)TaskPool.o $(OBJFLD)MemoryPool.o
LIB=-lm -lSDL2main -lSDL2

.PHONY: clean debug release 

$(OBJFLD)%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS)

all: main.c $(OBJS)
	$(CC) -o ../$(NAME) $(MAIN) $(DEBUG_FLAGS) $(OBJFLD)*.o $(LIB)

release: $(OBJS)
	$(CC) -o $(NAME) $(MAIN) $(RELEASE_FLAGS) $(OBJFLD)*.o $(LIB)

clean:
	rm -f $(OBJFLD)/*.o


