#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <atomic>

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

sf::TcpSocket createSocket() {
    sf::TcpSocket socket;
    return socket;
}

void tryToConnect(sf::TcpSocket& socket) {
    clearScreen();
    if (socket.connect(IP, PORT) == sf::Socket::Status::Done) {
        std::cout << "Connected to server!" << std::endl;
    } else {
        std::cerr << "Error: Could not connect to server." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void showMessages(const std::vector<std::string>& messageBuffer) {
    clearScreen();
    for (const auto& message : messageBuffer) {
        std::cout << message << std::endl;
    }
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
        sf::sleep(sf::milliseconds(20));
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