CXX=g++
CXXFLAGS=-g -Wall -std=c++11

OBJS=main.o sub_tree.o point_set.o

all: emst

emst: $(OBJS)
	$(CXX) $(CXXFLAGS) -o emst $(OBJS)

main.o: main.cpp point_types.hpp point_set.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

sub_tree.o: sub_tree.cpp sub_tree.hpp point_types.hpp
	$(CXX) $(CXXFLAGS) -c sub_tree.cpp

point_set.o: point_set.cpp point_set.hpp sub_tree.hpp point_types.hpp
	$(CXX) $(CXXFLAGS) -c point_set.cpp

clean:
	rm -f *.o emst *.dot *.pdf