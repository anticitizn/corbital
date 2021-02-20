TARGET=main.e

MAKEFLAGS += --no-builtin-rules

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

CC=g++
CCFLAGS=-std=c++17 -pedantic -Wall -w -Iinclude/ $(shell pkg-config --cflags sdl2)
LDFLAGS=-lGL $(shell pkg-config --libs sdl2) -ldl

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJECTS)
	@echo LD $@
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h
	@echo CC $*.cpp
	@$(CC) $(CCFLAGS) -c $<

%.o: %.cpp
	@echo CC $*.cpp
	@$(CC) $(CCFLAGS) -c $<

.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(TARGET)
