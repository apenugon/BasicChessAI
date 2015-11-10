CXX=g++
CFLAGS=-O2
OBJECTS=main.o
DIR=$(pwd)

main.o: $(DIR)/src/main.cpp
	$(CXX) $(CFLAGS) -c main.cpp