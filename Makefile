CXX=g++
CXXFLAGS=-std=c++11
EXE=bin/todo
OBJS=$(patsubst %.cpp,%.o,$(wildcard src/*.cpp))

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $^ -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(OBJS)
	rm $(EXE)
