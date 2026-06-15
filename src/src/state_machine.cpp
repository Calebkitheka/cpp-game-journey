#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

enum class StateID { Menu, Play, Pause };

// 🔹 Base Interface: Every state must implement these 3 methods
class GameState {
public:
    virtual void handleInput(sf::Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual ~GameState() = default; // 🔑 Required for polymorphic deletion
};

// 🔹 Menu State
class MenuState : public GameState {
    sf::RectangleShape bg, startBtn;
public:
    MenuState() {
        bg.setSize({800, 600}); bg.setFillColor(sf::Color(35, 35, 45));
        startBtn.setSize({200, 50}); startBtn.setFillColor(sf::Color::Green);
        startBtn.setPosition(300, 275);
    }
    void handleInput(sf::Event& event) override {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
            // Transition signal will be handled by the manager
            std::cout << "🚀 Transitioning to Play...\n";
        }
    }
    void update(float) override {}
    void render(sf::RenderWindow& w) override { w.draw(bg); w.draw(startBtn); }
};

// 🔹 Play State
class PlayState : public GameState {
    sf::CircleShape ball; sf::Vector2f vel;
public:
    PlayState() {
        ball.setRadius(15); ball.setFillColor(sf::Color::Cyan); ball.setOrigin(15, 15);
        ball.setPosition(400, 300);
        vel = {250.f, 180.f};
    }
    void handleInput(sf::Event& event) override {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            std::cout << "⏸️ Transitioning to Pause...\n";
        }
    }
    void update(float dt) override {
        ball.move(vel * dt);
        // Bounce off walls
        sf::Vector2f p = ball.getPosition();
        if (p.x < 15 || p.x > 785) vel.x *= -1;
        if (p.y < 15 || p.y > 585) vel.y *= -1;
        ball.setPosition(p); // Clamp to avoid edge clipping
    }
    void render(sf::RenderWindow& w) override { w.draw(ball); }
};

// 🔹 Pause State
class PauseState : public GameState {
    sf::RectangleShape overlay, resumeBtn;
public:
    PauseState() {
        overlay.setSize({800, 600}); overlay.setFillColor({0, 0, 0, 180});
        resumeBtn.setSize({180, 40}); resumeBtn.setFillColor(sf::Color::Yellow);
        resumeBtn.setPosition(310, 280);
    }
    void handleInput(sf::Event& event) override {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            std::cout << "▶️ Resuming Play...\n";
        }
    }
    void update(float) override {}
    void render(sf::RenderWindow& w) override { w.draw(overlay); w.draw(resumeBtn); }
};

// 🔹 COORDINATOR: State Manager & Main Loop
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Day 20: State Machine Architecture");
    window.setFramerateLimit(60);
    sf::Clock clock;

    StateID currentID = StateID::Menu;
    std::unique_ptr<GameState> state = std::make_unique<MenuState>();

    std::cout << "=== Controls ===\nSPACE: Start | ESC: Pause/Resume | X: Close\n\n";

    while (window.isOpen()) {
        StateID nextID = currentID; // Track transition request
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            state->handleInput(event);

            // Simple routing based on console input or key state
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space && currentID == StateID::Menu)
                    nextID = StateID::Play;
                else if (event.key.code == sf::Keyboard::Escape)
                    nextID = (currentID == StateID::Play) ? StateID::Pause : StateID::Play;
            }
        }

        // 🔒 State Transition (Safe swap & auto-cleanup)
        if (nextID != currentID) {
            switch (nextID) {
                case StateID::Menu:  state = std::make_unique<MenuState>();  break;
                case StateID::Play:  state = std::make_unique<PlayState>();  break;
                case StateID::Pause: state = std::make_unique<PauseState>(); break;
            }
            currentID = nextID;
        }

        // 🔹 Update & Render (Delegated to active state)
        state->update(clock.restart().asSeconds());
        window.clear(sf::Color(20, 20, 25));
        state->render(window);
        window.display();
    }
    return 0;
}