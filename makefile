



vpath %.hpp inc
vpath %.cpp src
vpath %.o bin

CC = g++
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, bin/%.o, $(SRC))

default: $(OBJ) 

clean: # delete all .o files
	rm bin/*.o
	rm *.out

run:
	$(CC) $(OBJ) -o reader-writer.out -pthread
	./reader-writer.out

#rules	
bin/%.o: %.cpp
	$(CC) -c -o $@ $< -pthread