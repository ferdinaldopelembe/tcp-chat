#include <SFML/Network.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "consts.hpp"

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

            if (clients[i]->receive(packet) == sf::Socket::Status::Done) {
                packet >> message;

                if (message == "sair") {
                    clients.erase(clients.begin() + i);
                    std::cout << "O cliente " << clientId << " saiu...\n";
                } else {
                    std::cout << "client " << clientId << "> " << message << "\n";
                }
            } else if (clients[i]->receive(packet) == sf::Socket::Status::Disconnected) {
                clients.erase(clients.begin() + i);
                std::cout << "O cliente " << clientId << " saiu...\n";
            }
        }

        sf::sleep(sf::milliseconds(20));
    }

    return 0;
}