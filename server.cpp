#include <SFML/Network.hpp>
#include <iostream>
#include "consts.hpp"

int main() {

    sf::TcpListener listener;
    sf::TcpSocket socket;
    
    std::string message = "";

    if (listener.listen(port, ip) != sf::Socket::Status::Done) {
        std::cout << "Erro ao iniciar servidor!\n\a";
        return 1;
    }

    std::cout << "Servidor iniciado na porta " << listener.getLocalPort() << "...\n";

    if (listener.accept(socket) != sf::Socket::Status::Done) {
        std::cout << "Erro de conexão com o cliente!\n";
        return 1;
    }

    std::cout << "Conexao feita com sucesso!\n";

    while (message != "sair") {
        sf::Packet packet;

        if (socket.receive(packet) == sf::Socket::Status::Done) {
            packet >> message;
            std::cout << "Cliente> " << message << "\n";
        }

        sf::sleep(sf::milliseconds(20));
    }

    return 0;
}