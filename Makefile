CC=gcc
SRC=$(wildcard ./src/*.c)
HEADERS=$(wildcard ./src/*.h)
CFLAGS=-O3 -Werror -Wall -Wextra -pedantic
LFLAGS=
BUILD=build
BIN=test
EXE=$(BUILD)/$(BIN)

.PHONY: clean run

all: $(EXE)

$(EXE): $(BUILD) $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(EXE) $(SRC) $(LFLAGS)

run: $(EXE)
	./$(EXE)

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)
