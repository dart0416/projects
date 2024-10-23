CC := g++
CFLAGS := -Wall -Wextra
BIN := bin
SRC := src
INCLUDE := -lsfml-graphics -lsfml-window -lsfml-system
LIBS := -I/usr/include/SFML/
EXECUTABLE := main
TYPES := .cpp
EDITOR=nvim

all: $(BIN)/$(EXECUTABLE)

run: all
	clear
	@echo "Executing $(BIN)/$(EXECUTABLE)..."
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/States/*.cpp) $(wildcard $(SRC)/Entity/*.cpp) $(wildcard $(SRC)/Other/*.cpp) $(wildcard $(SRC)/Components/*.cpp)
	@echo "Building..."
	$(CC) $^ $(CFLAGS) $(INCLUDE) $(LIBS) -o $@

clean:
	@echo "Cleaning..."
	rm -rf $(BIN)/*

edit:
	$(EDITOR) $(SRC)/
