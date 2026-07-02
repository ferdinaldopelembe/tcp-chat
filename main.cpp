#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(
        sf::VideoMode({300,300}),
        "This Is The Title...",
        sf::Style::Close
    );
    
    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        window.clear(sf::Color::White);
        window.display();
    }

    return 0;
}