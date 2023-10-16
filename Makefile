CXX = g++
CXXFLAGS = -std=c++11 -Wall

SFML_LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

SRC = main.cpp
EXECUTABLE = snake

OBJ = $(SRC:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(EXECUTABLE)
