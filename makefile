C = g++
CFLAGS = -g -Wall -std=c++11
OBJ = p4.o parser.o scanner.o
BIN = comp

%.o: %.cpp
	$(C) $(CFLAGS) $< -c -o $@

$(BIN): $(OBJ)
	$(C) -g $(CFLAGS) $^ -o $@

clean:
	rm $(OBJ) $(BIN)

