CC=gcc
CFLAGS+=-Wall -std=c99 -ggdb3 -fprofile-arcs -ftest-coverage
OBJFLD=obj/
SRCOBJFLD=../src/obj/
DEPS=TaskTest.h
OBJ=$(SRCOBJFLD)* TaskTest.o
LIB=-lm -lcheck -lpthread -lrt -lSDL2main -lSDL2
NAME=TaskPool-test

$(OBJFLD)%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

TaskPool-test: main.c $(OBJ)
	$(CC) -o ../$(NAME) main.c $(CFLAGS) $(LIB) $(OBJ)

all: TaskPool-test

.PHONY: all clean
clean: 
	rm ./$(OBJFLD)*.o



