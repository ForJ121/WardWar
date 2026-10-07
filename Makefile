CXX := g++

CXXFLAGS := -std=c++17 -Wall -Wextra \
	-MMD -MP \
	-I./third_party/raylib/src

LDFLAGS := -L./third_party/raylib/src \
	-lraylib -lopengl32 -lgdi32 -lwinmm -limm32

TARGET := WardWar.exe

SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
DEPS := $(OBJECTS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(OBJECTS) $(DEPS)

.PHONY: all run clean