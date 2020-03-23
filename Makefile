CXX=g++
CXXFLAGS=-std=c++11
EXE=bin/todo
OBJS=$(patsubst %.cpp,%.o,$(wildcard src/*.cpp))
LIBS=$(wildcard libs/*.a)

all: $(EXE)

$(EXE): $(OBJS) $(LIBS)
	$(CXX) $^ -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(OBJS)
	rm $(EXE)
