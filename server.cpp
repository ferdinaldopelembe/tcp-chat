#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <cstdlib>

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

sf::TcpListener createListener() {
    clearScreen();
    sf::TcpListener listener;
    listener.setBlocking(false);
    if (listener.listen(PORT) != sf::Socket::Status::Done) {
        std::cerr << "Error: Could not create listener on port " << PORT << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Listener created on port " << PORT << std::endl;
    return listener;
}

void sendBroadcastMessage(std::vector<std::unique_ptr<sf::TcpSocket>>& clients, Message message, int senderId) {
    for (int i = 0; i < clients.size(); i++) {
        if (i == senderId) {
            continue; // Skip the sender
        }

        auto& client = clients[i];
        std::string serializedMessage = message.getSender().getName() + "> " + message.getMessage();
        sf::Packet packet;
        packet << serializedMessage;
        if (client->send(packet) != sf::Socket::Status::Done) {
            std::cerr << "Error: Could not send message to client " << client->getRemoteAddress().value() << std::endl;
        }
    }
}

void handleClientsMessages(std::vector<std::unique_ptr<sf::TcpSocket>>& clients) {
    for (int i = 0; i < clients.size(); i++) {
        auto& client = clients[i];

        sf::Packet packet;
        sf::Socket::Status status = client->receive(packet);
        switch (status) {
            case sf::Socket::Status::Done: {
                    std::string senderName;
                    std::string messageContent;
         
                    packet >> senderName >> messageContent;

                    Message broadcastMessage(messageContent, User(senderName));
                    sendBroadcastMessage(clients, broadcastMessage, i);

                    std::cout << senderName << "> " << messageContent << std::endl;
                }
                break;
            case sf::Socket::Status::Disconnected:
                std::cout << "Client disconnected: " << client->getRemoteAddress().value() << std::endl;
                client->disconnect();
                clients.erase(clients.begin() + i);
                i--;
                break;
        }
    }
}

void tryAcceptNewClient(sf::TcpListener& listener, std::vector<std::unique_ptr<sf::TcpSocket>>& clients) {
    std::unique_ptr<sf::TcpSocket> client = std::make_unique<sf::TcpSocket>();
    client->setBlocking(false);
    if (listener.accept(*client) == sf::Socket::Status::Done) {
        std::cout << "New client connected: " << client->getRemoteAddress().value() << std::endl;
        clients.push_back(std::move(client));
    }
}

int main() {
    bool running = true;

    sf::TcpListener listener = createListener();
    std::vector<std::unique_ptr<sf::TcpSocket>> clients;

    while (running) {
        // Accept new clients
        tryAcceptNewClient(listener, clients);
        handleClientsMessages(clients);
       
        sf::sleep(sf::milliseconds(70));
    }

    return 0;
}