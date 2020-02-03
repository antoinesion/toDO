CXX=g++
CXXFLAGS=-std=c++11
EXE=toDO
OBJS=task.o actions/create.o actions/list.o actions/show.o actions/close.o main.o

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $^ -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(OBJS)
	rm $(EXE)
