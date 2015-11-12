CXX=g++
OBJECTS=main.o
DIR=$(shell pwd)
OBJDIR=$(DIR)/obj
SRCDIR=$(DIR)/src
HEADERDIR=$(SRCDIR)/header
CFLAGS=-O3 -std=c++11 -I$(HEADERDIR)
LDFLAGS=

_DEPS=chess_board.h piece.h
DEPS=$(patsubst %,$(HEADERDIR)/%,$(_DEPS))

_OBJS=main.o chess_board.o piece.o
OBJS=$(patsubst %,$(OBJDIR)/%, $(_OBJS))


default: main

debug: CFLAGS += -D DEBUG -g
debug: main

main: $(OBJS)
	$(CXX) -o AIProject $(OBJS) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CXX) $(CFLAGS) -c -o $@ $<


clean: 
	rm -f $(OBJDIR)/*