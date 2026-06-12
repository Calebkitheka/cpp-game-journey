#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Day 18: Real-Time Movement & Delta Time");
    window.setFramerateLimit(60);

    // 🔹 Player Object (Using RectangleShape; easily swappable to sf::Sprite later)
    sf::RectangleShape player(sf::Vector2f(40.0f, 40.0f));
    player.setFillColor(sf::Color::Cyan);
    player.setOrigin(20.0f, 20.0f); // 🔑 Center origin for future rotation/scaling
    player.setPosition(400.0f, 300.0f);

    const float SPEED = 400.0f; // Pixels per second (frame-rate independent)
    sf::Clock clock;

    std::cout << "=== Controls: WASD / Arrow Keys | ESC to quit ===\n";

    while (window.isOpen()) {
        // 1️⃣ EVENT LOOP (OS messages, window state)
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
               (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
        }

        // 2️⃣ UPDATE (Game Logic)
        float dt = clock.restart().asSeconds(); // Seconds since last frame
        sf::Vector2f input(0.0f, 0.0f);

        // Continuous input polling (doesn't wait for key repeat)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    input.y -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  input.y += 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  input.x -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) input.x += 1.0f;

        // 🔒 Normalize diagonal movement (prevents √2 speed boost)
        float length = std::sqrt(input.x * input.x + input.y * input.y);
        if (length > 0.0f) input /= length;

        // Apply delta-time movement
        player.move(input * SPEED * dt);

        // 🔒 Screen Boundary Clamping
        sf::Vector2f pos = player.getPosition();
        float halfSize = player.getOrigin().x;
        sf::Vector2u winSize = window.getSize();

        if (pos.x - halfSize < 0)           pos.x = halfSize;
        if (pos.x + halfSize > winSize.x)   pos.x = winSize.x - halfSize;
        if (pos.y - halfSize < 0)           pos.y = halfSize;
        if (pos.y + halfSize > winSize.y)   pos.y = winSize.y - halfSize;
        player.setPosition(pos);

        // 3️⃣ RENDER (Double-buffered pipeline)
        window.clear(sf::Color(20, 20, 30));
        window.draw(player);
        window.display();
    }

    std::cout << "✅ Window closed cleanly.\n";
    return 0;
}