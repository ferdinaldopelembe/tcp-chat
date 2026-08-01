#include <SFML/Network.hpp>
#include <iostream>
#include <atomic>
#include <thread>
#include "consts.hpp"

void receiveMessages(sf::TcpSocket& socket, std::atomic<bool>& running) {
    while (running) {
        sf::Packet packet;
        if (socket.receive(packet) == sf::Socket::Status::Done) {
            std::string message;
            packet >> message;
            std::cout << message << "\n";
        }
    }
}

int main() {
    sf::TcpSocket socket;
    std::string message;

    if (socket.connect(ip,port) != sf::Socket::Status::Done) {
        std::cout << "Erro ao conectar com servidor!\n\a";
        return 1;
    }

    std::cout << "Conexao feita com sucesso!\n";

    std::atomic<bool> running(true);
    std::thread receiveThread(receiveMessages, std::ref(socket), std::ref(running));

    do {
        sf::Packet packet;

        std::cout << "mensagem> ";
        std::getline(std::cin, message);

        packet << message;

        if (socket.send(packet) != sf::Socket::Status::Done) {
            std::cout << "Erro ao enviar mensagem!\n\a";
        }

    } while(message != "sair");

    running = false;
    
}