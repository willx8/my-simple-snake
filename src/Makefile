CXX=clang++
CXXFLAGS=-g	-std=c++11	-Wall	-pedantic
BIN=prog

SRC=$(wildcard *.cc)
OBJ=$(SRC:%.cc=%.o)

all:	$(OBJ)
	$(CXX) -lncurses	-o	$(BIN)	$^

%.o:	%.c
	$(CXX)	$@	-c	$<
clean:
	rm	-f	*.o
	rm	$(BIN)

