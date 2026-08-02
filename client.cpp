#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <atomic>
#include <windows.h>

#include "message.hpp"
#include "user.hpp"
#include "constants.hpp"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

sf::TcpSocket createSocket() {
    sf::TcpSocket socket;
    socket.setBlocking(false);
    return socket;
}

void tryToConnect(sf::TcpSocket& socket) {
    clearScreen();
    sf::Socket::Status status;
    status = socket.connect(IP, PORT);
    socket.setBlocking(true);
    switch (status) {
        case sf::Socket::Status::Done:
            std::cout << "\rConnected to server!" << std::endl;
            break;

        case sf::Socket::Status::NotReady:
            std::cout << "\rConecting to server..." << std::endl << std::flush;
            sf::sleep(sf::milliseconds(200));
            break;
    
        default:
            std::cerr << "Error: Could not connect to server." << std::endl;
            exit(EXIT_FAILURE);
    }
    socket.setBlocking(false);
    clearScreen();
}

void showMessages(const std::vector<std::string>& messageBuffer) {
    clearScreen();
    // std::cout << "\n\n\n\n";
    for (const auto& message : messageBuffer) {
        std::cout << message << std::endl;
    }
    // gotoXY(0,0);
    std::cout << "\nType 'q!' to quit." << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "> ";
}

void sendMessage(sf::TcpSocket& socket, User user, std::vector<std::string>& messageBuffer) {
    std::string input = "";
    std::getline(std::cin, input);

    if (input == "q!") {
        exit(EXIT_SUCCESS);
    }

    std::string selfSerializedMessage = "me> " + input;

    sf::Packet packet = sf::Packet();
    packet << user.getName() << input;

    if (socket.send(packet) != sf::Socket::Status::Done) {
        std::cerr << "Error: Could not send message." << std::endl;
    } else {
        messageBuffer.push_back(selfSerializedMessage);
    }
}

void receiveMessages(sf::TcpSocket& socket, std::vector<std::string>& messageBuffer, std::atomic<bool>& running) {
    while (running) {
        sf::Packet packet;
    
        if (socket.receive(packet) == sf::Socket::Status::Done) {
            std::string message;

            packet >> message;
            messageBuffer.push_back(message);
            showMessages(messageBuffer);
        }
        sf::sleep(sf::milliseconds(200));
    }
}

int main() {
    std::vector<std::string> messageBuffer;
    std::atomic<bool> running(true);

    sf::TcpSocket socket = createSocket();
    tryToConnect(socket);

    User user = User::create();
    showMessages(messageBuffer);

    std::thread receiverThread(receiveMessages, std::ref(socket), std::ref(messageBuffer), std::ref(running));

    sf::sleep(sf::milliseconds(200));
    while (running) {
        sendMessage(socket, user, messageBuffer);
        showMessages(messageBuffer);
        sf::sleep(sf::milliseconds(70));
    }

    return 0;
}