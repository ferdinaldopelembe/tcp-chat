.PHONY: compile run

CXX = g++
CXX_FLAGS = -std=c++17
LFLAGS = -L"./lib" -I"./include"
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
SRC = main.cpp
SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp
SERVER_TARGET = ./bin/server.exe
CLIENT_TARGET = ./bin/client.exe
DEFINES = -DSFML_STATIC

all:
	$(CXX) $(CXX_FLAGS) $(DEFINES) $(SERVER_SRC) -o $(SERVER_TARGET) $(LFLAGS) $(SFML_FLAGS)
	cls
	$(SERVER_TARGET)

s:
	$(CXX) $(CXX_FLAGS) $(DEFINES) $(SERVER_SRC) -o $(SERVER_TARGET) $(LFLAGS) $(SFML_FLAGS)
	cls
	$(SERVER_TARGET)

c:
	$(CXX) $(CXX_FLAGS) $(DEFINES) $(CLIENT_SRC) -o $(CLIENT_TARGET) $(LFLAGS) $(SFML_FLAGS)
	cls
	$(CLIENT_TARGET)