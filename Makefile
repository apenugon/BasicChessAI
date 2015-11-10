CXX=g++
OBJECTS=main.o
DIR=$(shell pwd)
OBJDIR=$(DIR)/obj
SRCDIR=$(DIR)/src
HEADERDIR=$(SRCDIR)/header
CFLAGS=-O2 -std=c++11 -I$(HEADERDIR)

_DEPS=chess_board.h
DEPS=$(patsubst %,$(HEADERDIR)/%,$(_DEPS))

_OBJS=main.o chess_board.o
OBJS=$(patsubst %,$(OBJDIR)/%, $(_OBJS))

main: $(OBJS)
	$(CXX) -o AIProject $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	$(CXX) $(CFLAGS) -c -o $@ $<

clean: 
	rm -f $(OBJDIR)/*