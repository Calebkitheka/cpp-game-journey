#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>

struct Entity {
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    bool is_player = false;
};

// 🔹 Simple AABB Resolution: Push moving object out along shortest axis
void resolve_collision(sf::RectangleShape& mover, const sf::RectangleShape& target) {
    sf::FloatRect overlap;
    if (!mover.getGlobalBounds().intersects(target.getGlobalBounds(), overlap)) return;

    // Push along axis of least penetration
    if (overlap.width < overlap.height) {
        float dir = (mover.getPosition().x < target.getPosition().x) ? -1.0f : 1.0f;
        mover.move(overlap.width * dir, 0.0f);
    } else {
        float dir = (mover.getPosition().y < target.getPosition().y) ? -1.0f : 1.0f;
        mover.move(0.0f, overlap.height * dir);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Day 19: AABB Collision & Response");
    window.setFramerateLimit(60);

    // 🔹 Player
    Entity player;
    player.shape.setSize({40.f, 40.f});
    player.shape.setOrigin(20.f, 20.f);
    player.shape.setPosition(150.f, 150.f);
    player.shape.setFillColor(sf::Color::Cyan);
    player.is_player = true;

    // 🔹 Enemies (Static + Moving)
    std::vector<Entity> entities;
    
    entities.push_back({{sf::Vector2f(60.f, 60.f)}, {80.f, 60.f}, false}); // Patrol
    entities.back().shape.setFillColor(sf::Color(220, 50, 50));
    entities.back().shape.setPosition(400.f, 300.f);
    
    entities.push_back({{sf::Vector2f(80.f, 80.f)}, {0.f, 0.f}, false}); // Static wall
    entities.back().shape.setFillColor(sf::Color(100, 100, 120));
    entities.back().shape.setPosition(600.f, 150.f);

    const float SPEED = 300.f;
    sf::Clock clock;
    bool colliding = false;

    while (window.isOpen()) {
        // 1️⃣ EVENT LOOP
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
               (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        // 2️⃣ UPDATE
        float dt = clock.restart().asSeconds();
        colliding = false;

        // Player input
        sf::Vector2f input(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) input.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) input.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) input.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) input.x += 1.f;
        
        float len = std::hypot(input.x, input.y);
        if (len > 0) input /= len;
        player.shape.move(input * SPEED * dt);

        // Update enemies & check collisions
        for (auto& e : entities) {
            if (e.velocity.x != 0 || e.velocity.y != 0) {
                e.shape.move(e.velocity * dt);
                // Simple patrol bounds
                if (e.shape.getPosition().x < 50 || e.shape.getPosition().x > 750) e.velocity.x *= -1.f;
                if (e.shape.getPosition().y < 50 || e.shape.getPosition().y > 550) e.velocity.y *= -1.f;
            }

            // 🔍 AABB Overlap Test
            if (player.shape.getGlobalBounds().intersects(e.shape.getGlobalBounds())) {
                colliding = true;
                resolve_collision(player.shape, e.shape); // Push out
            }
        }

        // Visual feedback
        player.shape.setFillColor(colliding ? sf::Color::Yellow : sf::Color::Cyan);

        // 3️⃣ RENDER
        window.clear(sf::Color(25, 25, 35));
        for (const auto& e : entities) window.draw(e.shape);
        window.draw(player.shape);
        window.display();
    }
    return 0;
}