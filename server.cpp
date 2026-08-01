#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "consts.hpp"

void sendBroadCastMessage(std::vector<std::unique_ptr<sf::TcpSocket>>& clients, std::string message, int origin) {
    for (int i = 0; i < clients.size(); i++) {
        if (i == origin) {
            continue;
        }
        sf::Packet packet;
        packet << message;
        clients[i]->send(packet);
    }
}

int main() {

    bool close = false;
    std::vector<std::unique_ptr<sf::TcpSocket>> clients;

    sf::TcpListener listener;
    listener.setBlocking(false);

    if (listener.listen(port, ip) != sf::Socket::Status::Done) {
        std::cout << "Erro ao iniciar servidor!\n\a";
        return 1;
    }

    std::cout << "Servidor iniciado na porta " << listener.getLocalPort() << "...\n";

    while (not close) {

        std::unique_ptr<sf::TcpSocket> possibleClient = std::make_unique<sf::TcpSocket>();
        possibleClient->setBlocking(false);

        if (listener.accept(*possibleClient) == sf::Socket::Status::Done) {
            clients.push_back(std::move(possibleClient));
            std::cout << clients.size() << " conectado" << (clients.size() > 1 ? "s" : "") << "\n";
        }

        for (int i = 0; i < clients.size(); i++) {
            int clientId = i + 1;
            
            std::string  message;
            sf::Packet packet;

            switch (clients[i]->receive(packet)) {
                case sf::Socket::Status::Done:
                    packet >> message;

                    if (message == "sair") {
                        clients.erase(clients.begin() + i);
                        std::string broadMessage = "O cliente " + std::to_string(clientId) + " saiu...";
                        sendBroadCastMessage(clients, broadMessage, -1);
                        std::cout << "O cliente " << clientId << " saiu...\n";

                        --i;
                    } else {
                        std::string broadMessage = "client " + std::to_string(clientId) + "> " + message;
                        sendBroadCastMessage(clients, broadMessage, i);
                        std::cout << "client " << clientId << "> " << message << "\n";   
                    }
                    break;

                case sf::Socket::Status::Disconnected:
                    clients.erase(clients.begin() + i);

                    std::string broadMessage = "O cliente " + std::to_string(clientId) + " saiu...";
                    sendBroadCastMessage(clients, broadMessage, -1);
                    std::cout << "O cliente " << clientId << " saiu...\n";

                    --i;
                    break;

            }
        }

        sf::sleep(sf::milliseconds(20));
    }

    return 0;
}