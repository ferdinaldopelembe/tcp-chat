.PHONY: compile run

CXX = g++
CXX_FLAGS = -std=c++17
LFLAGS = -L"./lib" -I"./include"
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
DEFINES = -DSFML_STATIC

# source code files
REQUIRED_SRC = ./src/message.cpp ./src/user.cpp
SRC = main.cpp
SERVER_SRC = server.cpp
CLIENT_SRC = client.cpp

SERVER_TARGET = ./bin/server.exe
# clients for tests
C1_TARGET = ./bin/client1.exe
C2_TARGET = ./bin/client2.exe

s:
	$(CXX) $(CXX_FLAGS) $(DEFINES) $(SERVER_SRC) $(REQUIRED_SRC) -o $(SERVER_TARGET) $(LFLAGS) $(SFML_FLAGS)
	cls
	$(SERVER_TARGET)

c1:
	$(CXX) $(CXX_FLAGS) $(DEFINES) $(CLIENT_SRC) $(REQUIRED_SRC) -o $(C1_TARGET) $(LFLAGS) $(SFML_FLAGS)
	cls
	$(C1_TARGET)

c2:
	$(CXX) $(CXX_FLAGS) $(DEFINES) $(CLIENT_SRC) $(REQUIRED_SRC) -o $(C2_TARGET) $(LFLAGS) $(SFML_FLAGS)
	cls
	$(C2_TARGET)

all: s
