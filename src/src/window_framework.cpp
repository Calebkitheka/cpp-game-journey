#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // 🔹 1. Window & Context Setup
    // Automatically initializes OpenGL/DirectX and creates an OS window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Day 17: SFML Graphical Loop");
    window.setFramerateLimit(60); // 🔒 Cap FPS to prevent GPU spin & ensure stable dt

    // 🔹 2. Placeholder Game Object
    sf::CircleShape player(30.0f);
    player.setFillColor(sf::Color::Cyan);
    player.setPosition(370.0f, 270.0f);

    // 🔹 3. Delta Time Clock
    sf::Clock clock;

    std::cout << "=== SFML Window Initialized ===\n";

    // 🔹 4. Main Game Loop
    while (window.isOpen()) {
        // 🔄 EVENT POLLING (Handle OS messages)
        // MUST drain all events every frame to avoid input lag / frozen windows
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        // 🔄 UPDATE (Game Logic)
        float dt = clock.restart().asSeconds(); // Time since last frame in seconds
        // (Player movement & physics will use 'dt' tomorrow)

        // 🔄 RENDER (Graphics Pipeline)
        window.clear(sf::Color(30, 30, 30)); // 🔹 1. Clear back buffer
        window.draw(player);                 // 🔹 2. Queue draw calls
        window.display();                    // 🔹 3. Swap buffers to screen
    }

    std::cout << "✅ Window closed cleanly.\n";
    return 0;
}