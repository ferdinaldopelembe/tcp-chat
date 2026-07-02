.PHONY: compile run

CXX = g++
CXX_FLAGS = -std=c++17
LFLAGS = -L"./lib" -I"./include"
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SRC = main.cpp
TARGET = ./bin/main.exe
DEFINES = -DSFML_STATIC

all:
	$(CXX) $(CXX_FLAGS) $(DEFINES) $(SRC) -o $(TARGET) $(LFLAGS) $(SFML_FLAGS)
	$(TARGET)