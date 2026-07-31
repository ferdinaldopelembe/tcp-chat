#include <SFML/Network.hpp>
#include <iostream>
#include "consts.hpp"

int main() {
    sf::TcpSocket socket;
    std::string message;

    if (socket.connect(ip,port) != sf::Socket::Status::Done) {
        std::cout << "Erro ao conectar com servidor!\n\a";
        return 1;
    }

    std::cout << "Conexao feita com sucesso!\n";

    do {
        sf::Packet packet;

        std::cout << "mensagem> ";
        std::getline(std::cin, message);

        packet << message;

        if (socket.send(packet) != sf::Socket::Status::Done) {
            std::cout << "Erro ao enviar mensagem!\n\a";
        }

    } while(message != "sair");
    
}