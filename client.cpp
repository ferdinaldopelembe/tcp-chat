#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <atomic>
#include <conio.h>

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

void showMessages(
        const std::vector<std::string>& messageBuffer,
        const std::string currentInput = ""
    ) {
    clearScreen();
    for (const auto& message : messageBuffer) {
        std::cout << message << std::endl;
    }
    std::cout << "\nType 'q!' to quit." << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "> " << currentInput << std::flush;
}

void sendMessage(
        sf::TcpSocket& socket,
        User user,
        std::string& input,
        std::vector<std::string>& messageBuffer
    ) {
    if (kbhit()) {
        char key = getch();
        switch(key) {
            case '\r':
                break;
            case '\b':
                if (!input.empty()) {
                    input.pop_back();
                    std::cout << "\b \b" << std::flush;
                }
                return;
            default:
                if (key != '\t') {
                    input += key;
                    putchar(key);
                }
                return;
        }
    } else {
        return;
    }
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
        input.clear();
    }

    showMessages(messageBuffer, input);
}

void receiveMessages(
        sf::TcpSocket& socket,
        const std::string& currentInput,
        std::vector<std::string>& messageBuffer,
        std::atomic<bool>& running
    ) {
    while (running) {
        sf::Packet packet;
    
        if (socket.receive(packet) == sf::Socket::Status::Done) {
            std::string message;

            packet >> message;
            messageBuffer.push_back(message);
            showMessages(messageBuffer, currentInput);
        }
        sf::sleep(sf::milliseconds(200));
    }
}

int main() {
    std::vector<std::string> messageBuffer;
    std::atomic<bool> running(true);
    std::string input;

    sf::TcpSocket socket = createSocket();
    tryToConnect(socket);

    User user = User::create();
    showMessages(messageBuffer);

    std::thread receiverThread(
        receiveMessages,
        std::ref(socket),
        std::ref(input),
        std::ref(messageBuffer),
        std::ref(running)
    );
    
    while (running) {
        sendMessage(socket, user, input, messageBuffer);
        sf::sleep(sf::milliseconds(10));
    }

    return 0;
}