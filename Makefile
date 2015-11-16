CXX=g++
OBJECTS=main.o
DIR=$(shell pwd)
OBJDIR=$(DIR)/obj
SRCDIR=$(DIR)/src
HEADERDIR=$(SRCDIR)/header
CFLAGS=-std=c++11 -I$(HEADERDIR) -fopenmp
LDFLAGS= -fopenmp

_DEPS=chess_board.h piece.h game_handler.h player.h human_player.h
DEPS=$(patsubst %,$(HEADERDIR)/%,$(_DEPS))

_OBJS=main.o chess_board.o piece.o game_handler.o human_player.o
OBJS=$(patsubst %,$(OBJDIR)/%, $(_OBJS))

default: CFLAGS += -O3
default: main

debug: CFLAGS += -D DEBUG -g -Wall
debug: main

main: $(OBJS)
	$(CXX) -o AIProject $(OBJS) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CXX) $(CFLAGS) -c -o $@ $<


clean: 
	rm -f $(OBJDIR)/*