CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = p06_automata_simulator
SRC = p06_automata_simulator.cc src/automata.cc
INC = -Iinclude

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INC) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
