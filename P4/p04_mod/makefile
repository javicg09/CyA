# Compilador y opciones
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g

# Estructura de directorios
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = obj
BIN = p04_regex

# Archivos fuente y objetos
SRC = $(SRC_DIR)/regex.cc p04_regex.cc
OBJ = $(OBJ_DIR)/regex.o $(OBJ_DIR)/p04_regex.o

# Regla principal
$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(OBJ)

# Compilación de los archivos objeto
$(OBJ_DIR)/regex.o: $(SRC_DIR)/regex.cc $(LIB_DIR)/regex.h
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/regex.cc -o $(OBJ_DIR)/regex.o

$(OBJ_DIR)/p04_regex.o: p04_regex.cc $(LIB_DIR)/regex.h
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c p04_regex.cc -o $(OBJ_DIR)/p04_regex.o

# Limpiar archivos compilados
clean:
	rm -rf $(OBJ_DIR) $(BIN)

# Ejecutar con ejemplo (opcional)
run: $(BIN)
	./$(BIN) input.cc output.txt
