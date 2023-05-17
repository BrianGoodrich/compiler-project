C = g++
CFLAGS = -g -Wall -std=c++11
OBJ = codegenerator.o parser.o scanner.o
BIN = compiler

%.o: %.cpp
	$(C) $(CFLAGS) $< -c -o $@

$(BIN): $(OBJ)
	$(C) -g $(CFLAGS) $^ -o $@

clean:
	rm $(OBJ) $(BIN)

