# ----------------------------------------------------------
# Makefile para la práctica 8 - Grammar2CNF
# Estructura:
#   lib/ → cabeceras (.h)
#   src/ → implementación (.cc)
#   raíz → main y makefile
# ----------------------------------------------------------

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
INCLUDES = -I./lib
SRC = src/chomsky.cc src/gramatica.cc p08_Grammar2CNF.cc
OBJ = $(SRC:.cc=.o)
TARGET = Grammar2CNF

# Regla por defecto
all: $(TARGET)

# Enlazado final
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compilación individual
src/%.o: src/%.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Limpieza de archivos objeto y binario
clean:
	rm -f src/*.o *.o $(TARGET)

# Limpieza total
mrproper: clean
	rm -f *~ lib/*~ src/*~

# Ejemplo de ejecución
run:
	./$(TARGET) input.gra output.gra
