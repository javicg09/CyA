# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Ilib

# Archivos y directorios
SRC = src
TARGET = p02_strings

# Busca todos los .cc en src
SRCS = $(wildcard $(SRC)/*.cc)
OBJS = $(SRCS:.cc=.o)

# Regla por defecto
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Regla genérica para compilar cada .cc en su .o
$(SRC)/%.o: $(SRC)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SRC)/*.o $(TARGET)

.PHONY: all clean
